#include <flexpret_csrs.h>
#include <flexpret_stdio.h>
#include <flexpret_io.h>

void main() {
    int core_id = read_csr(CSR_COREID);
    int hart_id = read_hartid();
    int errno;
    if (core_id == 0) {
        
        print_int(69);
        print_int(hart_id);
        print_str("Hello World!\n");
    }
}

