#include <flexpret_io.h>

int main0();
int main1();
int main2();
int main3();

int main() {
    _fp_print(13);
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
    _fp_print(40);
}

int main1() {
    _fp_print(41);
}

int main2() {
    _fp_print(42);
}

int main3() {
    _fp_print(43);
}