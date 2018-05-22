#ifndef __UART_H
#define __UART_H

void uart_init(void);
void uart_putc(char c);
int uart_getc(void);
void uart_write(const char *s);

#endif
