/**
 * @file debug.c
 * @author ajxs
 * @date Jul 2024
 * @brief Bootloader debug functions.
 */

#include <efi.h>
#include <efilib.h>
#include <stdarg.h>

#include <bootloader.h>
#include <debug.h>
#include <elf.h>
#include <error.h>
#include <serial.h>

EFI_STATUS debug_print_line(IN CHAR16* fmt,
	...)
{
	/** Main bootloader application status. */
	EFI_STATUS status;
	/** The variadic argument list passed to the VSPrintf function. */
	va_list args;
	/**
	 * The output message buffer.
	 * The string content is copied into this buffer. The maximum length is set
	 * to the maximum serial buffer length.
	 */
	CHAR16 output_message[MAX_SERIAL_OUT_STRING_LENGTH];

	va_start(args, fmt);

	// If the serial service has been initialised, use this as the output medium.
	// Otherwise use the default GNU-EFI output.
	if(serial_service.protocol) {
		VSPrint(output_message, MAX_SERIAL_OUT_STRING_LENGTH, fmt, args);

		status = print_to_serial_out(serial_service.protocol, output_message);
		if(EFI_ERROR(status)) {
			Print(L"Error: Error printing to serial output: %s\n",
				get_efi_error_message(status));

			return status;
		}
	} else {
		VPrint(fmt, args);
	}

	va_end(args);

	return EFI_SUCCESS;
};

void debug_print_memory_map(
	IN EFI_MEMORY_DESCRIPTOR* memory_map,
	IN UINTN memory_map_size,
	IN UINTN descriptor_size
)
{
	/** The number of UEFI memory descriptors. */
	const UINT16 n_descriptors = memory_map_size / descriptor_size;
	/** Pointer used to iterate through the descriptors. */
	EFI_MEMORY_DESCRIPTOR* current_descriptor = NULL;

	// The firmware will likely not return descriptors in the same format as
	// GNU-EFI's definition, so it's necessary to iterate through them using
	// pointer arithmetic.
	// Refer to: https://stackoverflow.com/a/39674958/5931673
	for (UINTN i = 0; i < n_descriptors; i++) {
		// The cast here is to ensure that the pointer arithmetic works correctly.
		// Refer to: https://stackoverflow.com/a/74791136/5931673
		current_descriptor = (VOID*)memory_map + (i * descriptor_size);

		if (current_descriptor->Attribute > 16) {
			#ifdef DEBUG
				debug_print_line(
					L"Descriptor:\n"
					L"  Type: %u\n"
					L"  Physical Address: 0x%llx\n"
					L"  Virtual Address: 0x%llx\n"
					L"  Size In Pages: %u\n"
					L"  Attributes: 0x%llx\n\n",
					current_descriptor->Type,
					current_descriptor->PhysicalStart,
					current_descriptor->VirtualStart,
					current_descriptor->NumberOfPages,
					current_descriptor->Attribute
				);
			#endif
		}
	}
}
