#include <stdint.h>
#include <stdlib.h>
#include <flexpret_io.h>
#include <flexpret_noc.h>

#define PING_PONG_LIMIT 10

int main()
{
    int core_id = read_csr(CSR_COREID);
    _fp_print(core_id);
    // _fp_print(N_CORES);

    int ping_pong_count = 0;
    int partner_core_id;

    while (ping_pong_count < PING_PONG_LIMIT)
    {
        if (core_id % 2 == 0)
        {
            // Increment the ping pong count before you send it
            ping_pong_count++;
            partner_core_id = core_id + 1;
            noc_send(partner_core_id, ping_pong_count, TIMEOUT_FOREVER);
            // core_id sent and incremented ping_pong_count to partner_core_id
            _fp_print(ping_pong_count);
        }
        else
        {
            partner_core_id = core_id - 1;
            fp_ret_t ret = noc_receive(&ping_pong_count, TIMEOUT_FOREVER);
            // core_id received ping_pong_count from partner_core_id
            _fp_print(ping_pong_count);
        }
    }
    return 1;
}
