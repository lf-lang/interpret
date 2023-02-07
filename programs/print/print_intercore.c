#include <flexpret_csrs.h>
#include <flexpret_printer.h>
#include <flexpret_io.h>
#include <flexpret_time.h>
#include <flexpret_thread.h>
#include <flexpret_assert.h>
#include <flexpret_noc.h>

void* t1(void* arg) {
    fp_printer_run();
}

void *t2(void *arg) {

    fp_printer_str("Hello from t2\n");
    fp_printer_int(read_hartid());
    fp_printer_str("\n");
}

void main0() {
    int errno;
    thread_t tid[2] = {1,2};
    errno = thread_map(true, &tid[0], t1, 0);
    assert(errno == 0);
    
    errno = thread_map(true, &tid[1], t2, 0);
    assert(errno == 0);

    void * exit_code;
    fp_printer_str("Hello Cruel World!\n");
    thread_join(tid[1], &exit_code);
    thread_cancel(tid[0]);
}

void main1() {
    fp_printer_str("Hello from Core1!\n");
}

void main() {
    int core_id = read_csr(CSR_COREID);
    switch(read_coreid()) {
        case 0: main0(); break;
        case 1: main1(); break;
    }
}
