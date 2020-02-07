/**
 * @file error.c
 * @author ajxs
 * @date Aug 2019
 * @brief Functionality for printing error messages.
 * Contains functionality for printing error messages that occur within the
 * application and assisting with error handling.
 */

#include <efi.h>
#include <efilib.h>
#include <error.h>

/**
 * @brief Error message string buffer.
 * This is used for storing the error message string returned from the
 * `get_efi_error_message` function.
 */
CHAR16 error_message_buffer[256];


/**
 * get_efi_error_message
 */
const CHAR16* get_efi_error_message(IN EFI_STATUS const status)
{
	// Copy the status into the error message buffer.
	StatusToString(error_message_buffer, status);
	return error_message_buffer;
}
