#include <stdbool.h>

#include "flexpret_io.h"
#include "flexpret_stdio.h"
#include "flexpret_uart.h"
#include <flexpret_assert.h>

#define SYNC_ID_LEN 2
#define LEN_FIELD_LEN 2
#define APP_START 0x00001000
// #define DBG

#ifdef DBG
#define DBG_PRINT(x) do {_fp_print(x);} while(0)
#else
#define DBG_PRINT(x) do {} while(0)
#endif
void (*application)(void) = (void (*)())APP_START;
int bootloader(void);

typedef enum {
    RECV_SYNC_ID,
    RECV_LEN,
    RECV_DATA,
    RECV_END_SYNC,
    FAULT
} app_recv_states_t;

// Global flag indicating that bootloading is done
// hart0 will set it to true. Other harts wait on it
int boot_done = 0;

void main(void) {
    uint32_t hartid = read_hartid();
    DBG_PRINT(21);
    DBG_PRINT(hartid);
    if (hartid == 0) {
        gpo_set(0, 1);
        int res = bootloader();
        if (res == 0) {
            boot_done = 1;
            gpo_clear(0, 0);
            DBG_PRINT(42);
            DBG_PRINT(*((uint32_t *) APP_START));
        } else {
            gpo_set(0, 0xF);
            assert(false);
            while(1) {}
        }
    }

    // Remaining harts wait on global flag here
    while (boot_done == 0) {};
    DBG_PRINT(22);
    DBG_PRINT(hartid);

    // Jump to start.S
    application();
    return;
}

int bootloader(void) {
    unsigned char sync_id[SYNC_ID_LEN] = {0xC0, 0xDE};
    app_recv_states_t app_recv_state = RECV_SYNC_ID;
    unsigned int idx=0;
    unsigned int byte_idx=0;
    unsigned int *app_ptr = (unsigned int *) APP_START;
    unsigned char recv_buffer[2];
    unsigned char recv;
    unsigned int len;
    unsigned int instr;

    while (1) {
        switch (app_recv_state) {

            case RECV_SYNC_ID: {
                DBG_PRINT(1);
                gpo_set(0, 2);
                recv=uart_receive();
                DBG_PRINT(recv);
                recv_buffer[1] = recv_buffer[0];
                recv_buffer[0] = recv;

                if (recv_buffer[0] == sync_id[0] && recv_buffer[1] == sync_id[1]) {
                    app_recv_state = RECV_LEN;
                    idx=0;
                    byte_idx=3;
                    instr = 0;
                    gpo_clear(0, 2);
                }
                break;
            }

            case RECV_LEN: {
                DBG_PRINT(2);
                gpo_set(0, 2);
                recv=uart_receive();
                DBG_PRINT(recv);
                recv_buffer[idx++] = recv;
                if (idx == LEN_FIELD_LEN) {
                    len = recv_buffer[1] << 8 | recv_buffer[0];        
                    app_recv_state = RECV_DATA;
                    idx = 0;
                    gpo_clear(0, 2);
                }
                break;
            }

            case RECV_DATA: {
                gpo_set(0, 4);
                recv = uart_receive();
                instr = instr | (((unsigned int) recv) << 8*byte_idx);
                if (byte_idx-- == 0) {
                    DBG_PRINT(3);
                    DBG_PRINT(app_ptr);
                    DBG_PRINT(instr);
                    *(app_ptr++) = instr;
                    instr = 0;
                    byte_idx=3;
                }

                if (++idx == len) {
                    gpo_clear(0, 4);
                    app_recv_state = RECV_END_SYNC;
                    idx=0;
                }
                break;
            }

            case RECV_END_SYNC: {
                DBG_PRINT(4);
                gpo_set(0, 8);
                recv = uart_receive();
                recv_buffer[1] = recv_buffer[0];
                recv_buffer[0] = recv;
                DBG_PRINT(recv);
                if (++idx == SYNC_ID_LEN) {
                    if (recv_buffer[0] == sync_id[0] && recv_buffer[1] == sync_id[1]) {
                        // Sucessfully received the program
                        gpo_clear(0, 8);
                        return 0;
                    }
                    else {
                        app_recv_state = FAULT;
                    }
                }
                break;
            }

            case FAULT: {
                return -1;
            }

            default: {
                app_recv_state = FAULT;
            }
        }
    }
}
