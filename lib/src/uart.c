#include "uart.h"

void uart_config()
{
    // Setup UART1 (TX=D5)
    UART1_CR2 |= UART_CR2_TEN; // Transmitter enable
    // UART1_CR2 |= UART_CR2_REN; // Receiver enable
    UART1_CR3 &= ~(UART_CR3_STOP1 | UART_CR3_STOP2); // 1 stop bit
    // 19200 baud: UART_DIV = 16000000/19200 ~ 833 = 0x0341
    UART1_BRR2 = 0x01; UART1_BRR1 = 0x34; // 0x0341 coded funky way (see page 365 and 336 of ref manual)
}

void uart_write_byte(uint8_t d)
{
    while(!(UART1_SR & UART_SR_TXE)); // !Transmit data register empty
        UART1_DR = d;
}

uint8_t uart_print(const char *txt)
{
    uint8_t bytes = 0;
    while(txt[bytes] != '\0' && bytes < 255) 
    {
        uart_write_byte(txt[bytes]);
        bytes++;
    }
    return bytes; // Bytes sent
}

uint8_t uart_println(const char *txt)
{
    uint8_t bytes = uart_print(txt);
    bytes += uart_print("\n\r");

    return bytes;
}

uint8_t uart_printnum(const int32_t num, uint8_t base)
{
    char buffer[32 + 1];
    int_to_str(num, buffer, base);
    return uart_print(buffer); //uart_puts replaced to cc1101_uart_print
}

// Helper function
void int_to_str(int32_t num, char *str_buf, uint8_t base)
{
    _itoa(num, str_buf, base);

    /* custom itoa() below: */

    // if (base != 2 && base != 8 && base != 10 && base != 16)
    // {
    //     return;
    // }
    // uint8_t is_unsigned = (num > 0);
    // str_buf[0] = '\0';
    // int8_t i = 0;

    // num = num < 0 ? num * (-1) : num;

    // do 
    // {
    //     str_buf[++i] = "0123456789abcdef"[num%base];
    //     num /= base;
        
    // } while(num != 0);
    
    // if (!is_unsigned)
    // {
    //     str_buf[++i] = '-';
    // }
    
    // uint8_t index_max = i;
    // while (i > index_max - i) // iterate the first half only
    // {
    //     SWAP(str_buf[i], str_buf[index_max - i]);
    //     --i;
    // }
}