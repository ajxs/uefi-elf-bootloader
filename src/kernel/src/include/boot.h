/**
 * @file boot.h
 * @author ajxs
 * @date Aug 2019
 * @brief Boot functionality.
 * Contains definitions for boot structures.
 */

#ifndef BOOT_H
#define BOOT_H

#include <stdint.h>

/**
 * @brief Memory region descriptor.
 * Describes a region of memory. This is passed to the kernel by the bootloader.
 */
typedef struct s_memory_region_desc {
	uint32_t type;
	uintptr_t physical_start;
	uintptr_t virtual_start;
	uint64_t count;
	uint64_t attributes;
} Memory_Map_Descriptor;

/**
 * @brief Boot info struct.
 * Contains information passed to the kernel at boot time by the bootloader.
 */
typedef struct s_boot_info {
	Memory_Map_Descriptor* memory_map;
	uint64_t mmap_size;
	uint64_t mmap_descriptor_size;
} Boot_Info;

#endif

