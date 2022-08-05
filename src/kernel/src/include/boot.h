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

typedef struct s_boot_video_info {
	uint32_t* framebuffer_pointer;
	uint32_t horizontal_resolution;
	uint32_t vertical_resolution;
	uint32_t pixels_per_scaline;
} Kernel_Boot_Video_Mode_Info;

/**
 * @brief Boot info struct.
 * Contains information passed to the kernel at boot time by the bootloader.
 */
typedef struct s_boot_info {
	Memory_Map_Descriptor* memory_map;
	uint64_t mmap_size;
	uint64_t mmap_descriptor_size;
	Kernel_Boot_Video_Mode_Info video_mode_info;
} Boot_Info;

#endif

