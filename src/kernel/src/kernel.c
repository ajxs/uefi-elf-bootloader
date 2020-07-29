/**
 * @file kernel.c
 * @author ajxs
 * @date Aug 2019
 * @brief Kernel entry.
 * Contains the kernel entry point.
 */

#include <stdbool.h>
#include <stddef.h>
#include <uart.h>

/**
 * @brief The kernel main program.
 * This is the kernel main entry point and its main program.
 */
void kernel_main(void)
{
	// Initialise the UART.
	uart_init();
	uart_puts("Kernel: Initialised.\n");

	while(1);
}
