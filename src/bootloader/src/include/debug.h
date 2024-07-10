#ifndef DEBUG_H
#define DEBUG_H

/**
 * @brief Prints to the default debug output.
 * Prints a null terminated format string to a the default debug output device.
 * If the serial service has been initialised, this will be used by default,
 * otherwise the default GNU-EFI `Print` function will be used.
 * Accepts all standard format specifiers as used in `string.h` functions.
 * @param[in] fmt    The format line to print.
 * @param[in] ...    Arguments for the format line.
 * @return The program status.
 * @retval EFI_SUCCESS            If the function executed successfully.
 * @retval EFI_INVALID_PARAMETER  If the supplied protocol is not properly
 *                                loaded or the supplied string is empty.
 * @retval EFI_BAD_BUFFER_SIZE    If the string is over the maximum length or
 *                                not properly null terminated.
 * @retval other                  Any other value is an EFI error code.
 * @warn If the string is not null terminated, this will result in an error.
 */
EFI_STATUS debug_print_line(IN CHAR16* fmt, ...);

void debug_print_memory_map(
	IN EFI_MEMORY_DESCRIPTOR* memory_map,
	IN UINTN memory_map_size,
	IN UINTN descriptor_size
);

#endif
