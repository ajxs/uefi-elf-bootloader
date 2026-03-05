/**
 * @file entry.S
 * @author ajxs
 * @brief Kernel entry point.
 * Sets up the kernel stack and transfers control to kernel_main.
 */

.extern kernel_main
.extern stack_top

.section .text

.global kernel_entry
kernel_entry:
	/* Set up the kernel stack. */
	leaq stack_top(%rip), %rsp

	call kernel_main

	/* kernel_main should never return. Halt if it does. */
.halt_loop:
	hlt
	jmp .halt_loop
