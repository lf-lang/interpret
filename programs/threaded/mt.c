#include <stdint.h>
#include <stdbool.h>
#include <flexpret_stdio.h>
#include <flexpret_csrs.h>

#include <flexpret_thread.h>
#include <flexpret_assert.h>
#include <flexpret_lock.h>

void* t1(void *) {
    print_str("Hello from t1\n");
}
void* t2(void *) {
    print_str("Hello from t2\n");
}
void* t3(void *) {
    print_str("Hello from t3\n");
}


int main() {
    int core_id = read_csr(CSR_COREID);

    if (core_id == 0) {
        int errno;

    print_str("Hello from main\n");
    uint32_t num; 
    thread_t tid1, tid2, tid3;
    void *etid1, *etid2, *etid3;
    
    tid1=1;
    errno = thread_map(HRTT, &tid1, t1, 0);
    assert(errno == 0);
    
    tid2=2;
    errno = thread_map(HRTT, &tid2, t2, 0);
    assert(errno == 0);
    
    tid3=3;
    errno = thread_map(HRTT, &tid3, t3, 0);
    assert(errno == 0);

    thread_join(tid1, &etid1);
    print_str("t1 done\n");
    thread_join(tid2, &etid2);
    print_str("t2 done\n");
    thread_join(tid3, &etid3);
    print_str("t3 done\n");

    return 0;
    }
}

