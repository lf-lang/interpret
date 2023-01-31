#include <flexpret_stdio.h>
#include <flexpret_io.h>
#include <flexpret_time.h>
#include <flexpret_thread.h>
#include <flexpret_assert.h>

void* t1(void* arg) {
    print_run();
}

void main() {
    int core_id = read_csr(CSR_COREID);
    if (core_id == 0) {
        thread_t tid[2];
        int errno = thread_create(true, &tid[0], t1, 0);
        assert(errno == 0);

        void * exit_code;
        print_str("Hello Cruel World!\n");
        thread_join(tid[0], &exit_code);
    }
}
