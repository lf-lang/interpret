#include <stdint.h>
#include <flexpret_io.h>

#include "asm_utils.h"
#include "low_level_interface.h"
#include "c_api.h"

/* | NE | _ | N | E | _ | */

/***********************
 * core 0 / N \ core 1 *
 *        W + E        *
 * core 2 \ S / core 3 *
 ***********************/

int main0();
int main1();
int main2();
int main3();

int main() {
    int core_id = read_csr(CSR_COREID);
    switch(core_id) {
        case 0: main0(); break;
        case 1: main1(); break;
        case 2: main2(); break;
        case 3: main3(); break;
        default: _fp_print(666); //ERROR
    }
}

int main0() {
    broadcast_count(0, 64);
    read_n_words_and_print(1, EAST_INT);
    read_n_words_and_print(2, NORTH_INT);
    read_n_words_and_print(3, NORTHEAST_INT);
}

int main1() {
    read_n_words_and_print(0, EAST_INT);
    broadcast_count(1, 17);
    read_n_words_and_print(2, NORTHEAST_INT);
    read_n_words_and_print(3, NORTH_INT);
}

int main2() {
    read_n_words_and_print(0, NORTH_INT);
    read_n_words_and_print(1, NORTHEAST_INT);
    broadcast_count(2, 42);
    read_n_words_and_print(3, EAST_INT);
}

int main3() {
    read_n_words_and_print(0, NORTHEAST_INT);
    read_n_words_and_print(1, NORTH_INT);
    read_n_words_and_print(2, EAST_INT);
    broadcast_count(3, 3);
}
