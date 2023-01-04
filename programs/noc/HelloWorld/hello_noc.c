#include <stdint.h>
#include <flexpret_io.h>
#include <flexpret_noc.h>

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
    uint32_t send_values[] = {1,2,3,4,5,6,7,8,9,10};
    uint32_t read;

    // Send values to listener
    for (int i = 0; i<10; i++) {
        noc_send(2, send_values[i]);
    }
}

int main1() {
    uint32_t send_values[] = {10,20,30,40,50,60,70,80,90,100};
    uint32_t read;

    // Send values to listener
    for (int i = 0; i<10; i++) {
        noc_send(3, send_values[i]);
    }
}

int main2() {
    uint32_t read;
    for (int i=0; i<10; i++) {
        read = noc_receive();
        _fp_print(read);
    }
}

int main3() {
    uint32_t read;
    for (int i=0; i<10; i++) {
        read = noc_receive();
        _fp_print(read);
    }
}