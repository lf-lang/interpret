#include "flexpret_lock.h"
#include "interpret_stdio.h"
#include "interpret_uart.h"

#define STDIO_MAX_DIGITS 32

lock_t lock = {.locked = false};

void print_str(const char *str) {
  lock_acquire(&lock);
  while (*str != '\0') {
    uart_send(*str);
    str++;
  }

  lock_release(&lock);
}

void print_int(int val) {
  lock_acquire(&lock);

  char buf[STDIO_MAX_DIGITS];
  int n_digits = 0;
  if (val == 0) {
    buf[n_digits++] = '0';
  }

  while (val && n_digits < STDIO_MAX_DIGITS) {
    char digit = '0' + (val % 10);
    buf[n_digits++] = digit;
    val = val / 10;
  }

  for (int i = n_digits - 1; i >= 0; i--) {
    uart_send(buf[i]);
  }
  // Print newline
  uart_send('\n');
  lock_release(&lock);
}