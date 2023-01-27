
#include <flexpret_io.h>

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
        default: _fp_print(66); //ERROR
    }
}


int main0() {
    gpo_set_0(1);
}

int main1() {
    gpo_set_0(2);
}

int main2() {
    gpo_set_0(3);
}

int main3() {
    gpo_set_0(4);
}