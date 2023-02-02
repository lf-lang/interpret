#include <flexpret_csrs.h>
#include <flexpret_stdio.h>
#include <flexpret_io.h>
#include <flexpret_time.h>
#include <flexpret_thread.h>
#include <flexpret_assert.h>


void *t1(void *arg) {

    print_str("Hello from t1\n");
    print_int(read_hartid());
    print_str("\n");
}

void *t2(void *arg) {

    print_str("Hello from t2\n");
    print_int(read_hartid());
    print_str("\n");
}

void main() {
    int core_id = read_csr(CSR_COREID);
    int errno;
    if (core_id == 0) {
        thread_t tid[2] = {1,2};
        errno = thread_map(true, &tid[0], t1, 0);
        assert(errno == 0);
        
        errno = thread_map(true, &tid[1], t2, 0);
        assert(errno == 0);

        void * exit_code;
        print_str("Hello Cruel World!\n");
        thread_join(tid[0], &exit_code);
        thread_join(tid[1], &exit_code);
    }
}
