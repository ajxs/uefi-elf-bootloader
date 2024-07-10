/**
 * @file serial.c
 * @author ajxs
 * @date Aug 2019
 * @brief Serial IO functionality.
 * Contains functionality for serial IO.
 */

#include <efi.h>
#include <efilib.h>
#include <stdarg.h>

#include <bootloader.h>
#include <debug.h>
#include <error.h>
#include <serial.h>


/**
 * configure_serial_protocol
 */
EFI_STATUS configure_serial_protocol(IN EFI_SERIAL_IO_PROTOCOL* const protocol)
{
	/** The program status. */
	EFI_STATUS status;

	#ifdef DEBUG
		debug_print_line(L"Debug: Configuring serial IO protocol\n");
	#endif

	status = uefi_call_wrapper(protocol->SetAttributes, 7,
		protocol, 0, 0, 0, 0, 0, DefaultStopBits);
	if(EFI_ERROR(status)) {
		debug_print_line(L"Error: Error configuring Serial Protocol: %s\n",
			get_efi_error_message(status));

		return status;
	}

	return EFI_SUCCESS;
}


/**
 * init_serial_service
 */
EFI_STATUS init_serial_service(void)
{
	/** The program status. */
	EFI_STATUS status;

	#ifdef DEBUG
		debug_print_line(L"Debug: Initialising Serial service\n");
	#endif

	status = uefi_call_wrapper(gBS->LocateProtocol, 3,
		&gEfiSerialIoProtocolGuid, NULL, &serial_service.protocol);
	if(EFI_ERROR(status)) {
		debug_print_line(L"Error: Error locating Serial Protocol: %s\n",
			get_efi_error_message(status));

		return status;
	}

	#ifdef DEBUG
		debug_print_line(L"Debug: Located Serial Protocol\n");
	#endif

	status = configure_serial_protocol(serial_service.protocol);
	if(EFI_ERROR(status)) {
		return status;
	}

	return EFI_SUCCESS;
}


/**
 * print_to_serial_out
 */
EFI_STATUS print_to_serial_out(IN EFI_SERIAL_IO_PROTOCOL* const protocol,
	IN CHAR16* line)
{
	/** The program status. */
	EFI_STATUS status;
	/** The size of the buffer being printed. */
	UINTN buffer_size = 0;
	/** The length of the line to be printed. */
	UINTN line_length = 0;

	// If the supplied protocol has not been loaded properly or the supplied
	// string is empty, raise an exception.
	if(protocol == NULL ||
		line == NULL) {
		return EFI_INVALID_PARAMETER;
	}

	line_length = StrLen(line);

	// Check if the string length will overflowed the maximum buffer size.
	if(line_length > MAX_SERIAL_OUT_STRING_LENGTH) {
		return EFI_BAD_BUFFER_SIZE;
	}

	// If the string is 0 length, raise an exception.
	if(line_length == 0) {
		return EFI_INVALID_PARAMETER;
	}

	// Set the buffer size to be printed to the line length.
	// This will be checked afterwards to determine if the full line was printed.
	// Buffer size is double the line length to take into account the double
	// width unicode characters.
	buffer_size = line_length * 2;

	status = uefi_call_wrapper(protocol->Write, 3,
		protocol, &buffer_size, (VOID*)line);
	if(EFI_ERROR(status)) {
		// Fall back to using EFI built-in `debug_print_line` function in this case.
		debug_print_line(L"Error: Error writing to serial protocol: %s\n",
			get_efi_error_message(status));

		return status;
	}

	// Check if the actual printed length is equal to the expected length.
	if(buffer_size != (line_length * 2)) {
		debug_print_line(L"Error: Full string not printed to serial protocol\n");
		return EFI_DEVICE_ERROR;
	}

	return EFI_SUCCESS;
}
