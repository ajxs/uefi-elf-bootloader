/**
 * @file main.c
 * @author ajxs
 * @date Aug 2019
 * @brief Bootloader entry point and main application.
 * The entry point for the application. Contains the main bootloader code that
 * initiates the loading of the Kernel executable. The main application is
 * contained within the `efi_main` function.
 */

#include <efi.h>
#include <efilib.h>
#include <stdarg.h>

#include <bootloader.h>
#include <debug.h>
#include <error.h>
#include <memory_map.h>

/**
 * get_memory_map
 */
EFI_STATUS get_memory_map(OUT VOID** memory_map,
	OUT UINTN* memory_map_size,
	OUT UINTN* memory_map_key,
	OUT UINTN* descriptor_size,
	OUT UINT32* descriptor_version)
{
	/** Program status. */
	EFI_STATUS status;
	/** Input key type used to capture user input. */
	EFI_INPUT_KEY input_key;

	#ifdef DEBUG
		debug_print_line(L"Debug: Allocating memory map\n");
	#endif

	status = uefi_call_wrapper(gBS->GetMemoryMap, 5,
		memory_map_size, NULL, memory_map_key,
		descriptor_size, NULL);
	if(EFI_ERROR(status)) {
		// This will always fail on the first attempt, this call will return the
		// required buffer size.
		if(status != EFI_BUFFER_TOO_SMALL) {
			debug_print_line(L"Fatal Error: Error getting memory map size: %s\n",
				get_efi_error_message(status));

			#if PROMPT_FOR_INPUT_BEFORE_REBOOT_ON_FATAL_ERROR
				debug_print_line(L"Press any key to reboot...");
				wait_for_input(&input_key);
			#endif

			return status;
		}
	}

	#ifdef DEBUG
		debug_print_line(L"Debug: Memory map required size: %u\n", *memory_map_size);
	#endif

	// According to: https://stackoverflow.com/a/39674958/5931673
	// Up to two new descriptors may be created in the process of allocating the
	// new pool memory.
	*memory_map_size += (2 * (*descriptor_size));

	#ifdef DEBUG
		debug_print_line(L"Debug: Allocating memory map with size: %u\n",
			*memory_map_size);
	#endif

	status = uefi_call_wrapper(gBS->AllocatePool, 3,
		EfiLoaderData, *memory_map_size, (VOID**)memory_map);
	if(check_for_fatal_error(status, L"Error allocating memory map buffer")) {
		return status;
	}

	status = uefi_call_wrapper(gBS->GetMemoryMap, 5,
		memory_map_size, *memory_map, memory_map_key,
		descriptor_size, descriptor_version);
	if(check_for_fatal_error(status, L"Error getting memory map")) {
		return status;
	}

	return EFI_SUCCESS;
}
