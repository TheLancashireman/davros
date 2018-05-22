#include "uart.h"

#define BCMIO_BASE	0x3F000000

/* GPIO */
#define GPFSEL1		((volatile unsigned int*)(BCMIO_BASE+0x00200004))
#define GPPUD		((volatile unsigned int*)(BCMIO_BASE+0x00200094))
#define GPPUDCLK0	((volatile unsigned int*)(BCMIO_BASE+0x00200098))

/* Auxilary mini UART registers */
#define AUX_ENABLE	((volatile unsigned int*)(BCMIO_BASE+0x00215004))
#define UART_DATA	((volatile unsigned int*)(BCMIO_BASE+0x00215040))
#define UART_IER	((volatile unsigned int*)(BCMIO_BASE+0x00215044))
#define UART_IIR	((volatile unsigned int*)(BCMIO_BASE+0x00215048))
#define UART_LCR	((volatile unsigned int*)(BCMIO_BASE+0x0021504C))
#define UART_MCR	((volatile unsigned int*)(BCMIO_BASE+0x00215050))
#define UART_CTRL	((volatile unsigned int*)(BCMIO_BASE+0x00215060))
#define UART_BAUD	((volatile unsigned int*)(BCMIO_BASE+0x00215068))

#define uart_ctrl	(*UART_CTRL)
#define uart_data	(*UART_DATA)

#define UART_DR		0x01		/* At least one char in Rx fifo */
#define UART_TE		0x20		/* At least one space in Tx fifo */


void uart_init()
{
	unsigned int r;
	
	*AUX_ENABLE |= 1;			// enable UART1, AUX mini uart
	*UART_IER = 0;
	*UART_CTRL = 0;
	*UART_LCR = 3;				// 8 bits
	*UART_MCR = 0;
	*UART_IER = 0;
	*UART_IIR = 0xc6;			// disable interrupts
	*UART_BAUD = 270;			// 115200 baud

	r = *GPFSEL1;
	r &= ~((7<<12)|(7<<15));	// gpio14, gpio15
	r |= (2<<12)|(2<<15);		// alt5
	*GPFSEL1 = r;

	*GPPUD = 0;					// enable pins 14 and 15

	for ( r = 0; r < 150; r++ )
	{
		/* Wait 150 cycles */
	}

	*GPPUDCLK0 = (1<<14)|(1<<15);

	for ( r = 0; r < 150; r++ )
	{
		/* Wait 150 cycles */
	}

	*GPPUDCLK0 = 0;				// flush GPIO setup
	*UART_CTRL = 3;			// enable Tx, Rx
}

void uart_putc(char c)
{
	while ( (uart_ctrl & UART_TE) == 0 )
	{
		/* Nothing */
	}
	uart_data = (unsigned char)c;
}

int uart_getc()
{
	int c = -1;

	if ( (uart_ctrl & UART_DR) != 0 )
	{
		c = (int)uart_data;
	}

	return c;
}

void uart_write(const char *s)
{
	while ( *s != '\0' )
	{
		if ( *s == '\n' )
			uart_putc('\r');
		uart_putc(*s);
		s++;
	}
}
