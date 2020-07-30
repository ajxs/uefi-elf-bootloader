/**
 * @file kernel.c
 * @author ajxs
 * @date Aug 2019
 * @brief Kernel entry.
 * Contains the kernel entry point.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <boot.h>
#include <uart.h>
#include <vga.h>

/**
 * @brief The kernel main program.
 * This is the kernel main entry point and its main program.
 */
void kernel_main(Boot_Info boot_info)
{
	// Initialise VGA output.
	vga_initialize();

	// Initialise the UART.
	uart_initialize();
	uart_puts("Kernel: Initialised.\n");

	while(1);
}
