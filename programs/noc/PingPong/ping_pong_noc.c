#include <stdint.h>
#include <stdlib.h>
#include <flexpret_io.h>
#include <flexpret_noc.h>

#define PING_PONG_LIMIT 10

int main() {
    int core_id = read_csr(CSR_COREID);

    uint32_t ping_pong_count = 0;
    int partner_core_id;

    // Set the partner id
    if (core_id % 2 == 0) 
        partner_core_id = core_id + 1;
    else
        partner_core_id = core_id - 1;

    while (ping_pong_count < PING_PONG_LIMIT) {
        if ((core_id % 2 == 0 && ping_pong_count % 2 == 0) ||
            (core_id % 2 == 1 && ping_pong_count % 2 == 1)) {
            // Increment the ping pong count before you send it
            ping_pong_count += 1;
            
            // core_id sent and incremented ping_pong_count to partner_core_id
            noc_send(partner_core_id, ping_pong_count, TIMEOUT_FOREVER);
        } else {            
            // core_id received ping_pong_count from partner_core_id
            noc_receive(&ping_pong_count, TIMEOUT_FOREVER);
            _fp_print(ping_pong_count);
        }
    }
    return 1;
}
