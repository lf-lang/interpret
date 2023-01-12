#include <flexpret_io.h>
#include <flexpret_time.h>

#define BLINK_PERIOD 500000000UL

#define DELAY_ITER 10000000
volatile int flag=0;

void delay() {
    flag = 0;
    while(++flag < DELAY_ITER);
}

int main() {
    unsigned int i=0;
    while(1) {
        gpo_write_0(i++);
        delay_for(BLINK_PERIOD);
    }
}