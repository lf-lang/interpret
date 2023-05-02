#include <stdlib.h>
#include <stdint.h>
#include <flexpret_io.h>

int main0();
int main1();
int main2();
int main3();

int main() {
    int core_id = read_csr(CSR_COREID);
    _fp_print(core_id);

    switch(core_id) {
        case 0: main0(); break;
        case 1: main1(); break;
        case 2: main2(); break;
        case 3: main3(); break;
        default: _fp_print(66); //ERROR
    }
}


int main0() {
    uint32_t* a = malloc(sizeof(uint32_t));
    *a = 100;
    _fp_print(*a);
    free(a);
}

int main1() {
    uint32_t* b = malloc(sizeof(uint32_t));
    *b = 100;
    _fp_print(*b);
    free(b);
}

int main2() {
    uint32_t* c = malloc(sizeof(uint32_t));
    *c = 100;
    _fp_print(*c);
    free(c);
}

int main3() {
    uint32_t* d = malloc(sizeof(uint32_t));
    *d = 100;
    _fp_print(*d);
    free(d);
}