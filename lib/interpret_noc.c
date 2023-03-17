#include "interpret_noc.h"
#include <flexpret_csrs.h>
#include "flexpret.h"

#define NOC_BASE 0x40000020UL
#define NOC_CSR (*( (volatile uint32_t *) (NOC_BASE + 0x0UL)))
#define NOC_DATA (*( (volatile uint32_t *) (NOC_BASE + 0x4UL)))
#define NOC_SOURCE (*( (volatile uint32_t *) (NOC_BASE + 0x8UL)))
#define NOC_DEST (*( (volatile uint32_t *) (NOC_BASE + 0x8UL)))

// Macros for parsing the CSR register value
#define NOC_TX_READY(val) (val & 0x01)
#define NOC_DATA_AVAILABLE(val) (val & 0x02)

fp_ret_t noc_send(uint32_t addr, uint32_t data, timeout_t timeout) {
    if (timeout == TIMEOUT_FOREVER) {
        while (!NOC_TX_READY(NOC_CSR));
        NOC_DEST = addr;
        NOC_DATA = data;
        return FP_SUCCESS;
    }
    if (timeout == TIMEOUT_NEVER) {
        if (NOC_TX_READY(NOC_CSR)) {
            NOC_DEST = addr;
            NOC_DATA = data;
            return FP_SUCCESS;
        } else {
            return FP_FAILURE;
        }
    }
    timeout_t time = rdtime() + timeout;
    while (rdtime() < time) {
        if (NOC_TX_READY(NOC_CSR)) {
            NOC_DEST = addr;
            NOC_DATA = data;
            return FP_SUCCESS;
        }
    }
    return FP_FAILURE;
}


fp_ret_t noc_receive(uint32_t* data, timeout_t timeout) {
    if (timeout == TIMEOUT_FOREVER) {
        while (!NOC_DATA_AVAILABLE(NOC_CSR));
        *data = NOC_DATA;
        return FP_SUCCESS;
    }
    if (timeout == TIMEOUT_NEVER) {
        if (NOC_DATA_AVAILABLE(NOC_CSR)) {
            *data = NOC_DATA;
            return FP_SUCCESS;
        }
        return FP_FAILURE;
    }
    uint32_t time = rdtime() + timeout;
    while (rdtime() < time) {
        if (NOC_DATA_AVAILABLE(NOC_CSR)) {
            *data = NOC_DATA;
            return FP_SUCCESS;
        }
    }
    return FP_FAILURE;
}