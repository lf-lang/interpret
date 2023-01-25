#include <flexpret_io.h>
#include <flexpret_csrs.h>
#include <flexpret_time.h>
#include <flexpret_assert.h>

int main() {
    _fp_print(1);
    int core_id = read_csr(CSR_COREID);
    _fp_print(core_id);

    if (core_id != 0) {
        return 0;
    }

    unsigned int pre = rdtime();
    delay_for(10000);
    unsigned int post = rdtime();
    unsigned int diff = post-pre;
    _fp_print(diff);
    ASSERT(diff < 20000);
    _fp_print(14);
}