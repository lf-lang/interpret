#include <flexpret_io.h>
#include <flexpret_csrs.h>
#include <flexpret_exceptions.h>

int flag=0;

void ie_isr(void) {
    uint32_t now = rdtime();
    _fp_print(42);
    _fp_print(now);
    flag=1;
}

int main() {
    _fp_print(1);
    int core_id = read_csr(CSR_COREID);
    _fp_print(core_id);

    if (core_id != 0) {
        return 0;
    }

    enable_interrupts();
    register_isr(EXC_CAUSE_INTERRUPT_EXPIRE, ie_isr);
    uint32_t now = rdtime();
    uint32_t expire = now + 100000;
    _fp_print(expire);
    interrupt_on_expire(expire);

    while (flag==0) {}
}