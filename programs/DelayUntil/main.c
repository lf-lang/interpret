#include <flexpret_io.h>
#include <flexpret_csrs.h>
#include <flexpret_time.h>

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
    _fp_print(post-pre);
}