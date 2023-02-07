/**
 * A multicore, threaded version of matrix multiplication, where the matrices are 
 * defined in the static memory: a * b = c.
 *
 * Currently, the number of cores is set in `N_CORES` (4 here). Each core has 
 * `NUM_THREADS` hardware threads. For each core, the hardware thread 0 will be
 * responsible for communication, while the remaining (`nb_threads`) will
 * execute the parallel matrix multiplication.
 * 
 * Since all cores do not share a memory, copies of the matrices will exist for
 * every core. Core0 will start by initializing a and b. Then it will send a chunk
 * of matrix a, together with matrix b to the other cores.
 * Each core will then distribute the work among the threads (`NUM_THREADS -1).
 * In each core, thread 0 is responsible for the communication.
 * Once all threads of Core0 are done with the computation, the communication 
 * thread will gother the data. Before recieveing, it sends a ready to receive 
 * message to the core. 
 * 
 * Make sure to pass the hardware thread number to the `riscv-compile.sh`. It
 * should be at least `nb_threads + 1`.
 * Make sure that the FlexPRET was built wih the right number of hardware threads.
 * and the Makefile is initialized with the correct number of threads.
 *
 **/

#include <stdlib.h>
#include <stdint.h>
#include <flexpret_io.h>
#include <flexpret_csrs.h>
#include <flexpret_lock.h>
#include <flexpret_thread.h>
#include <flexpret_noc.h>


// Hardware configuration values
#define N_CORES 4
#define nb_threads (NUM_THREADS - 1)

// Matrixes dimensions
#define NRA 24 // number of rows in matrix a
#define NCA 5 // number of columns in matrix a
#define NCB 5 // number of columns in matrix b

// Struct to save the start and end indexes among NRA per core
typedef struct  {
    int i_start;
    int i_end;
} local_NRA;

// Matrixes a, b, and c, to be statically allocated.
uint32_t a[NRA][NCA], b[NCA][NCB], c[NRA][NCB];

// Utility function
local_NRA get_NRA_range_per_core(int);

// Main functions per core
int main0();
int mainX();

// Matrix Multiplication routine, that will be executed by each thread, except
// thread 0 (main in that core), because it will be in charge of communication.
void* matrix_multiply_thread(void *);


////////////////////////////////////////////////////
int main(int argc, char *argv[]) {
    int core_id = read_csr(CSR_COREID);
    // _fp_print(core_id);

    if (core_id == 0) {
        main0();
    } else if (core_id < N_CORES) {
        mainX();
    } else {
        _fp_print(66666); // ERROR
    }
}


// Core 0 will start by initialyzing matcrices a and b, and sending to each core
// the chuncks of data it will use.
// It will then create the threads for the performing computation.
// Data is collected at the end. 
int main0() {
    int i, j, k;
    uint32_t start_time, stop_time;
    int core;
    local_NRA l_nra;

    // Initialize a, b, and c matrices
    for (i = 0; i < NRA; i++) 
        for (j = 0; j < NCA; j++)
            a[i][j] = (int32_t) 1;
    for (i = 0; i < NCA; i++)
        for (j = 0; j < NCB; j++)
            b[i][j] = (int32_t) j;

    // Communication is held by thread 0
    // Send chuncks of a and b to cores 1, 2 and 3
    for (core = 1 ; core < N_CORES ; core++) {
        l_nra = get_NRA_range_per_core(core);

        // Send a elements
        for (i = l_nra.i_start; i < l_nra.i_end; i++)
            for (j = 0; j < NCA; j++)
                noc_send(core, a[i][j], TIMEOUT_FOREVER);
        
        // Then send b elements
        for (i = 0; i < NCA; i++)
            for (j = 0; j < NCB; j++)
                noc_send(core, b[i][j], TIMEOUT_FOREVER);
    }

    // Thread creation to perform the matrix multiply
    // Thread ids
    thread_t tid[nb_threads];
    int errno[nb_threads];

    start_time = rdtime();
    
    // Create the threads
    l_nra = get_NRA_range_per_core(0);
    for (i = 0; i < nb_threads; i++) {
        errno[i] = thread_create(HRTT, &tid[i], matrix_multiply_thread, &l_nra);
        if (errno[i] != 0)
            _fp_print(666);
    }

    // Be ready to hear from other cores
    for (core = 1; core < N_CORES; core++) {
        l_nra = get_NRA_range_per_core(core);

        // Tell core that core0 is ready to recieve
        noc_send(core, 99999, TIMEOUT_FOREVER);

        // Receive elements
        for (i = l_nra.i_start; i < l_nra.i_end; i++)
            for (j = 0; j < NCB; j++)
                noc_receive(&c[i][j], TIMEOUT_FOREVER);
    }

    // Join once the local job is done
    void *exit_code[nb_threads];
    for (i = 0; i < nb_threads; i++) {
        thread_join(tid[i], &exit_code[i]);
    }

    stop_time = rdtime();
    _fp_print(stop_time - start_time);

    // Print the result
    for (i = 0; i < NRA; i++)
        for (j = 0; j < NCB; j++)
            _fp_print(c[i][j]);

    // Terminate by having thread 0 send
    // cancellation requests to all hardware threads.
    for (int i = 0; i < NUM_THREADS ; i++)
        thread_cancel(i);

    return (0);
}

// Main function to be executed by cores 1, 2, and 3.
// It starts by receiveing the data, afterwhich it creates the threads for the 
// parallel matrix multiplication. Finally, it waits for the clear to send signal
// and sends the results to core0. 
int mainX() {
    // Start by recieving a chunk and then b
    // Communication is held by thread 0
    int core_id = read_csr(CSR_COREID);
    int i,j;
    local_NRA l_nra = get_NRA_range_per_core(core_id);

    uint32_t start_time, stop_time;

    // Receive a elements
    for (i = l_nra.i_start; i < l_nra.i_end; i++)
        for (j = 0; j < NCA; j++)
            noc_receive(&a[i][j], TIMEOUT_FOREVER);

    // Receive b elements
    for (i = 0; i < NCA; i++)
        for (j = 0; j < NCB; j++)
            noc_receive(&b[i][j], TIMEOUT_FOREVER);

    // Set the parameters to pass to the thread
    start_time = rdtime();

    // Create the threads to perform the computation of the matrix multiply
    thread_t tid[nb_threads];
    int errno[nb_threads];
    for (i = 0; i < nb_threads; i++) {
        errno[i] = thread_create(HRTT, &tid[i], matrix_multiply_thread, &l_nra);
        if (errno[i] != 0)
            _fp_print(666);
    }

    // Join once the local job is done
    void *exit_code[nb_threads];
    for (i = 0; i < nb_threads; i++) {
        thread_join(tid[i], &exit_code[i]);
    }

    stop_time = rdtime();
    _fp_print(stop_time - start_time);

    // Wait for the clear to send signal, which is equal to 99999
    uint32_t clear;
    noc_receive(&clear, TIMEOUT_FOREVER);
    // Send computed c elements
    for (i = l_nra.i_start; i < l_nra.i_end; i++)
        for (j = 0 ; j<NCB ; j++)
            noc_send(0, c[i][j], TIMEOUT_FOREVER);

    return (0);
}


// Matrix multiply implementation.
// The index range of the outer loop is derived from both: the core id and the
// thread id.  
void* matrix_multiply_thread (void* lnra) {
    uint32_t tid = read_hartid();
    local_NRA* l_nra = (local_NRA*) lnra;
    int start = l_nra->i_start;
    int end = l_nra->i_end;
    
    int i_start, i_end, i, j, k;
    i_start = start + ((tid - 1) * (end - start) / nb_threads);
    if (tid == nb_threads)
        i_end = end;
    else
        i_end = start + (tid * (end - start) / nb_threads);

    // Do the computation
    for (i = i_start; i < i_end; i++) {
        for (j = 0; j < NCB; j++) {
            for (k = 0, c[i][j] = 0; k < NCA; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

// Utility function to find the start and end indexes among NRA, per core. 
local_NRA get_NRA_range_per_core(int core){
    local_NRA l_nra;

    l_nra.i_start = core * NRA / N_CORES;
    if (core == N_CORES - 1) {
        l_nra.i_end = NRA;
    } else {
        l_nra.i_end = (core + 1) * NRA / N_CORES;
    }

    return l_nra;
}