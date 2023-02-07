/** Ping pong program, where cores having even ids are the senders, and cores
 *  with odd ids are the receivers.
 *
 *  After setting the start time, the sender, respectively the receiver, will
 *  start sending, respectly receiving, periodically.
 *
 *  In order to highlignt time precision, non-blocking send and receive over
 *  the NoC are used. Receiving, however, need to use a timeout. This mechanism 
 *  accomodates for hiting the correct TDMA time slot, as well as the difference 
 *  in number of assembly instructions when calling send and/or receive.
 **/

#include <stdint.h>
#include <stdlib.h>
#include <flexpret_io.h>
#include <flexpret_noc.h>
#include <flexpret_time.h>


#define PING_PONG_LIMIT 10
#define DELTA 10000         // Used for synchronizing the start
#define PERIOD 100000       // Period to send and receive
#define TRIP_DELAY 2500     // Timeout, so that the TDMA slot is not missed, also
                            // due to instructions variability

// Functions
int main_sender(int);
int main_receiver(int);

uint32_t array[PING_PONG_LIMIT] = {9999};

int main() {
    int core_id = read_csr(CSR_COREID);
    if (core_id % 2 == 0)
        main_sender(core_id);
    else
        main_receiver(core_id);
}


int main_sender(int core_id) {
    int partner_core_id = core_id + 1;
    int ping_pong_count = 0;

    // Set the time at which the loop will start
    // This relies on the fact that clocks are synchronized, and that the start
    // code is quit similar in lenght
    uint32_t release_time = rdtime();
    release_time = (release_time / DELTA) + DELTA + 1000;
    delay_until(release_time);

    // Loop to send every PERIOD time 
    while (ping_pong_count < PING_PONG_LIMIT) {
        noc_send(partner_core_id, ping_pong_count, TIMEOUT_NEVER);
        ping_pong_count++;

        release_time += PERIOD;
        delay_until(release_time);
    }

    _fp_print(array[0]);
    _fp_print(array[1]);
    _fp_print(array[PING_PONG_LIMIT - 1]);
}

int main_receiver(int core_id) {
    int partner_core_id = core_id - 1;
    int ping_pong_count = 0; int i = 0;    

    // Set the time at which the loop will start
    // This relies on the fact that clocks are synchronized, and that the start
    // code is quit similar in lenght 
    uint32_t release_time = rdtime();
    release_time = (release_time / DELTA) + DELTA + 1000;
    delay_until(release_time);

    // Loop to receive every PERIOD time
    while (ping_pong_count < PING_PONG_LIMIT) {
        noc_receive(&array[ping_pong_count], TRIP_DELAY);
        ping_pong_count++;
        
        release_time += PERIOD;
        delay_until(release_time);
    }

    _fp_print(array[0]);
    _fp_print(array[1]);
    _fp_print(array[PING_PONG_LIMIT - 1]);
}
