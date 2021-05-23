#ifndef _UART_H
#define _UART_H

#include "stm8s.h"
#include "gpio.h"
#include <stdint.h>

void uart_config();
void uart_write_byte(uint8_t d);
uint8_t uart_print(const char *txt);
uint8_t uart_println(const char *txt);
uint8_t uart_printnum(const int32_t num, uint8_t base);
// helper function
void int_to_str(int32_t num, char *str_buf, uint8_t base);

#endif