/**
 * A multicore, threaded version of of merge sort, where the array is defined
 * in the static memory.
 *
 * This example follows the scatter-gather pattern. The `main` function
 * divides the indexes among the `N_CORES` cores. And each core devides the
 * iondexes among the threads. Individual results are then gathered per
 * core, and then at Core 0.
 * The number of working threads is set in `nb_threads=NUM_THREADS -1`.
 *
 * Since all cores do not share a memory, copies of `array` will exist for
 * every core. Core0 will start by initializing `array`. Then it will assign
 * chuncks to the other cores.
 *
 * Each core will then distribute the work among the threads `nb_threads`.
 * In each core, thread 0 is responsible for the communication.
 * Once all threads of Core0 are done with the computation, the communication
 * thread will gather the data. Before recieving, it sends a ready to receive
 * message to the core.
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

// Number of elements
#define SIZE 60

// Array of size SIZE
int32_t array[SIZE];

// Struct to save the index range per core
typedef struct {
    int i_start;
    int i_end;
} range_per_core;

// Main functions per core
int main0();
int mainX();

// Merge Sort routine, that will be executed by each thread
void* merge_sort_thread(void*);

// Routines for sorting and merging
void merge_sort(int left, int right);
void merge(int left, int middle, int right);
void gather(int number, int level, range_per_core rpc, int ratio);

// Utility function
range_per_core get_range_per_core(int);

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

// Core 0 will start by initialyzing array, and sending to each core
// the chuncks of data it will use.
// It will then create the threads for the performing computation.
// Data is collected at the end.
int main0() {
    int i, k, core;
    uint32_t start_time, stop_time;
    range_per_core rpc;
    
    // Initialize `array` with random numbers
    for (i = 0; i < SIZE; i++) {
        array[i] = rand() % SIZE;
    }

    // Communication is held by thread 0
    // Send chuncks of `array` to cores 1, 2 and 3
    for (core = 1 ; core < N_CORES ; core++) {
        rpc = get_range_per_core(core);

        // Send a elements
        for (i = rpc.i_start; i < rpc.i_end; i++)
            noc_send(core, array[i]);
    }

    // Thread ids
    thread_t tid[nb_threads];
    int errno[nb_threads];

    start_time = rdtime();

    // Create the threads and scatter the work based on the indexes
    range_per_core range_core0 = get_range_per_core(0);
    for (i = 0; i < nb_threads; i++) {
        errno[i] = thread_create(&tid[i], merge_sort_thread, &range_core0);
        if (errno[i] != 0)
            _fp_print(666);
    }

    // Join once the job is done
    void *exit_code[nb_threads];
    for (i = 0; i < nb_threads; i++) {
        thread_join(tid[i], &exit_code[i]);
    }

    // Now, merge (gather) the results from the threads, in Core 0
    gather(nb_threads, 1, range_core0, nb_threads);

    // Be ready to hear from other cores
    for (core = 1; core < N_CORES; core++) {
        rpc = get_range_per_core(core);

        // Tell core that core0 is ready to recieve
        noc_send(core, 99999);

        // Receive elements
        for (i = rpc.i_start; i < rpc.i_end; i++)
            array[i] = noc_receive();
    }

    // Finally, merge (gather) the results from all cores
    range_per_core all = {0, SIZE};
    gather(N_CORES, 1, all, N_CORES);

    stop_time = rdtime();
    _fp_print(stop_time - start_time);
    
    // Print the result 
    for (i = 0; i < SIZE; i++) {
        _fp_print(array[i]);
    }

    return (0);
}

// Main function to be executed by cores 1, 2, and 3.
// It starts by receiveing the data, afterwhich it creates the threads for the
// merge and sort operation. Finally, it waits for the clear to send signal
// and sends the results to core0.
int mainX() {
    // Start by recieving the chunck of data
    // Communication is held by thread 0
    int core_id = read_csr(CSR_COREID);
    int i, j;
    uint32_t start_time, stop_time;

    // Get the range for this core
    range_per_core rpc = get_range_per_core(core_id);

    // Receive `array` elements
    for (i = rpc.i_start; i < rpc.i_end; i++) {
        array[i] = noc_receive();
    }

    // Set the parameters to pass to the thread
    start_time = rdtime();

    // Create the threads to perform the computation of the matrix multiply
    thread_t tid[nb_threads];
    int errno[nb_threads];
    for (i = 0; i < nb_threads; i++) {
        errno[i] = thread_create(&tid[i], merge_sort_thread, &rpc);
        if (errno[i] != 0)
            _fp_print(666);
    }

    // Join once the local job is done
    void *exit_code[nb_threads];
    for (i = 0; i < nb_threads; i++) {
        thread_join(tid[i], &exit_code[i]);
    }

    // Finally, merge (gather) the results from this core
    gather(nb_threads, 1, rpc, nb_threads);

    stop_time = rdtime();
    _fp_print(stop_time - start_time);

    // Wait for the clear to send signal, which is equal to 99999
    int clear = noc_receive();
    // Send sorted elements
    for (i = rpc.i_start; i < rpc.i_end; i++) {
        noc_send(0, array[i]);
    }

    // Terminate by having thread 0 send
    // cancellation requests to all hardware threads.
    for (int i = 0; i < NUM_THREADS; i++)
        thread_cancel(i);

    return (0);
}

// Routine executed by each thread.
// It starts by deriving the sub-array to merge sort. 
void* merge_sort_thread(void* rpc) {
    range_per_core * rpc_ = (range_per_core*) rpc; 
    int size_per_core = rpc_->i_end - rpc_->i_start;
    int size_per_thread = size_per_core / nb_threads;
    int size_remaining = size_per_core % nb_threads;

    // Compute the left and right indexes from the thread id amd the range.
    // Thread 0, which runs `main()` function is excluded.
    uint32_t tid = read_hartid() - 1;
    int left = tid * (size_per_thread) + rpc_->i_start;
    int right = (tid + 1) * (size_per_thread)-1 + rpc_->i_start;
    if (tid == nb_threads - 1) {
        right += size_remaining;
    }
    int middle = left + (right - left) / 2;

    if (left < right) {
        merge_sort(left, middle);
        merge_sort(middle + 1, right);
        merge(left, middle, right);
    }
}

// Recursive call on the two sub-arrays to sort and merge
void merge_sort(int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;
        merge_sort(left, middle);
        merge_sort(middle + 1, right);
        merge(left, middle, right);
    }
}

// Merge of two ordered sub-arrays
void merge(int left, int middle, int right) {
    int i, j, k;
    int left_size = middle - left + 1;
    int right_size = right - middle;
    int left_array[left_size];
    int right_array[right_size];
    // int *left_array = (int *)malloc(left_size * sizeof(int));
    // int *right_array = (int *)malloc(right_size * sizeof(int));

    // Save the elements in the left in a separate array
    for (int i = 0; i < left_size; i++) {
        left_array[i] = array[left + i];
    }

    // Save the elements in the right in a separate array
    for (int j = 0; j < right_size; j++) {
        right_array[j] = array[middle + 1 + j];
    }

    i = 0;
    j = 0;
    k = left;
    // Ordered copy of elements from left to right
    while (i < left_size && j < right_size) {
        if (left_array[i] <= right_array[j]) {
            array[k++] = left_array[i++];
        } else {
            array[k++] = right_array[j++];
        }
    }

    // Copy the remaining elements
    while (i < left_size) {
        array[k++] = left_array[i++];
    }
    while (j < right_size) {
        array[k++] = right_array[j++];
    }

    // free(left_array);
    // free(right_array);
}

// Gathering the nbr sub-arrays from the range per core  
void gather(int nbr, int level, range_per_core rpc, int ratio) {
    int size = rpc.i_end - rpc.i_start;
    int size_chunck = size / ratio;

    for (int i = 0 ; i < nbr ; i = i + 2) {
        int left = i * (size_chunck * level) + rpc.i_start;
        int right = ((i + 2) * size_chunck * level) - 1 + rpc.i_start;
        if (right >= rpc.i_end) {
            right = rpc.i_end - 1;
        }
        int middle = left + (size_chunck * level) - 1;
        merge(left, middle, right);
    }

    if (nbr / 2 >= 1) {
        gather(nbr / 2, level * 2, rpc, ratio);
    }
}

// Utility function to find the start and end indexes per core.
range_per_core get_range_per_core(int core) {
    range_per_core rpc;

    rpc.i_start = core * SIZE / N_CORES;
    if (core == N_CORES - 1) {
        rpc.i_end = SIZE;
    } else {
        rpc.i_end = (core + 1) * SIZE / N_CORES;
    }

    return rpc;
}