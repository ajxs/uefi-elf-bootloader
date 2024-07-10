/**
 * @file graphics.h
 * @author ajxs
 * @date Aug 2019
 * @brief Functionality for working with the graphics protocol.
 * Contains functionality for initiating and working with the graphics protocol and
 * its associated framebuffers.
 */

#ifndef BOOTLOADER_MEMORY_MAP_H
#define BOOTLOADER_MEMORY_MAP_H 1

#include <efi.h>
#include <efilib.h>

/**
 * @brief Allocates the memory map.
 * Allocates the memory map. This function needs to be run prior to exiting
 * UEFI boot services.
 * @param[out] memory_map            A pointer to pointer to the memory map
 *                                   buffer to be allocated in this function.
 * @param[out] memory_map_size       The size of the allocated buffer.
 * @param[out] memory_map_key        They key of the allocated memory map.
 * @param[out] descriptor_size       A pointer to the size of an individual
 *                                   EFI_MEMORY_DESCRIPTOR.
 * @param[out] descriptor_version    A pointer to the version number associated
 *                                   with the EFI_MEMORY_DESCRIPTOR.
 * @return                   The program status.
 * @retval EFI_SUCCESS       The function executed successfully.
 * @retval other             A fatal error occurred getting the memory map.
 * @warn    After this function has been run, no other boot services may be used
 *          otherwise the memory map key will have changed, and the memory map
 *          will be considered invalid.
 */
EFI_STATUS get_memory_map(OUT VOID** memory_map,
	OUT UINTN* memory_map_size,
	OUT UINTN* memory_map_key,
	OUT UINTN* descriptor_size,
	OUT UINT32* descriptor_version);

#endif
