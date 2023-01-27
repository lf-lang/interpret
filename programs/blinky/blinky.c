/**
Example program where core0 blinks the LED which should be connected to port0.
*/
#include <flexpret_io.h>
#include <flexpret_time.h>

#define BLINK_PERIOD 500000000UL

int main() {
    int core_id = read_csr(CSR_COREID);
    if (core_id == 0) {
        unsigned int i=0;
        while(1) {
            gpo_write_0(i++);
            delay_for(BLINK_PERIOD);
        }
    }
}