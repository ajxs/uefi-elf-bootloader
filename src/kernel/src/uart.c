/**
 * @file uart.c
 * @author ajxs
 * @date Aug 2019
 * @brief UART functionality.
 * Contains the implementation of the UART.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <port_io.h>
#include <string.h>
#include <uart.h>

/* COM1 Port address*/
#define UART_PORT_COM1 0x3f8


/**
 * uart_initialize
 */
void uart_initialize(void)
{
	outb(UART_PORT_COM1 + 1, 0x00);    // Disable all interrupts
	outb(UART_PORT_COM1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
	outb(UART_PORT_COM1 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
	outb(UART_PORT_COM1 + 1, 0x00);    //                  (hi byte)
	outb(UART_PORT_COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
	outb(UART_PORT_COM1 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
	outb(UART_PORT_COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}


/**
 * uart_is_recieve_buffer_empty
 */
bool uart_is_recieve_buffer_empty(void)
{
	return inb(UART_PORT_COM1 + 5) & 1;
}


/**
 * uart_getchar
 */
char uart_getchar(void)
{
	while(!uart_is_recieve_buffer_empty());

	return inb(UART_PORT_COM1);
}


/**
 * uart_is_transmit_buffer_empty
 */
bool uart_is_transmit_buffer_empty(void)
{
	return (inb(UART_PORT_COM1 + 5) & 0x20) != 0;
}


/**
 * uart_putchar
 */
void uart_putchar(char a)
{
	while(!uart_is_transmit_buffer_empty());

	outb(UART_PORT_COM1, a);
}


/**
 * uart_puts
 */
void uart_puts(const char* str)
{
	/** The length of the string being written to serial out. */
	size_t str_len = strlen(str);
	for(size_t i = 0; i < str_len; i++) {
		uart_putchar(str[i]);
	}
}
