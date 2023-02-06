#include <stdint.h>
#include <stdbool.h>
#include <flexpret_stdio.h>
#include <flexpret_csrs.h>

#include <flexpret_thread.h>
#include <flexpret_assert.h>
#include <flexpret_lock.h>

void* t1_do_work(void *) {
    print_str("Hello from t1\n");
}


int main() {
    int core_id = read_csr(CSR_COREID);

    if (core_id == 0) {


    print_str("Hello from main\n");
    uint32_t num; 
    thread_t tid=1;
    int errno = thread_map(HRTT, &tid, t1_do_work, 0);
    assert(errno == 0);

    void * exit_code_t1;
    thread_join(tid, &exit_code_t1);
    print_str("t1 done\n");

    return 0;
    }
}

