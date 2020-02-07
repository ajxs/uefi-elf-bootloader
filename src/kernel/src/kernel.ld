/**
 *  Copyright (c) 2019, AJXS.
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Authors:
 *     Anthony <ajxs [at] panoptic.online>
 */

/** The physical starting address of the kernel. */
KERNEL_PHYS_START = 0x100000;
/**
 * The offset between the physical start and the start of the kernel's
 * mapping in virtual memory.
 */
KERNEL_VMA_OFFSET = 0xC0000000;
/** The size of the kernel stack. */
KERNEL_STACK_SIZE = 0x4000;

ENTRY (kernel_main)

SECTIONS
{
	. = KERNEL_PHYS_START;
	. += KERNEL_VMA_OFFSET;
	kernel_start = .;

	.text ALIGN (4K) : AT (ADDR (.text) - KERNEL_VMA_OFFSET)
	{
		*(.text*)
	}

	.rodata ALIGN (4K) : AT (ADDR (.rodata) - KERNEL_VMA_OFFSET)
	{
		*(.rodata*)
	}

	.data ALIGN (4K) : AT (ADDR (.data) - KERNEL_VMA_OFFSET)
	{
		*(.data*)
	}

	.bss ALIGN (4K) : AT (ADDR (.bss) - KERNEL_VMA_OFFSET)
	{
		*(COMMON)
		*(.bss*)

    . = ALIGN (16);
    stack_bottom = .;
    . += KERNEL_STACK_SIZE;
    stack_top = .;
	}

	kernel_end = .;
}
