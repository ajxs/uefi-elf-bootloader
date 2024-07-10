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

#include <bootloader.h>
#include <debug.h>
#include <error.h>

/**
 * @brief Error message string buffer.
 * This is used for storing the error message string returned from the
 * `get_efi_error_message` function.
 * @note 256 is a more-than-suitable length for this buffer since we know
 * ahead of time the lengths of all of the GNU EFI error messages. Refer
 * to the GNU-EFI source:
 * `https://sourceforge.net/p/gnu-efi/code/ci/master/tree/lib/error.c`
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

/**
 * check_for_fatal_error
 */
bool check_for_fatal_error(IN EFI_STATUS const status,
	IN const CHAR16* error_message)
{
	if(EFI_ERROR(status)) {
		/** Input key type used to capture user input. */
		EFI_INPUT_KEY input_key;

		debug_print_line(L"Fatal Error: %s: %s\n", error_message,
			get_efi_error_message(status));

		#if PROMPT_FOR_INPUT_BEFORE_REBOOT_ON_FATAL_ERROR
			debug_print_line(L"Press any key to reboot...");
			wait_for_input(&input_key);
		#endif

		return TRUE;
	}

	return FALSE;
}
