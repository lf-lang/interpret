#include <flexpret_csrs.h>
#include <flexpret_io.h>
#include <flexpret_time.h>
#include <flexpret_stdio.h>


void main(void) {
    int core_id = read_coreid();
    int cnt = 0;
    if (core_id == 0) {
        while(1) {
            if (gpi_read(2) & 0x1) {
                cnt++;
                gpo_write(0, cnt);
                print_str("Button Press: ");
                print_int(cnt);
                // FIXME: Implement button debouncing. It would be a nice example 
                delay_for(MSEC(500));
            }
        }
    }
}