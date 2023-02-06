#include <flexpret_stdio.h>
#include <flexpret_io.h>
#include <flexpret_time.h>

void main() {
    int core_id = read_csr(CSR_COREID);
    if (core_id == 0) {
        gpo_set_0(2);
        gpo_set_1(1);
        print_init();
        gpo_set_0(4);
        print_str("Hello World");


        delay_for(1000000);


        gpo_set_0(8);
        print_int(12345);
        delay_for(1000000);
        gpo_set_0(16);
        print_str("\n");
        delay_for(1000000);
        gpo_set_0(32);
    }
}