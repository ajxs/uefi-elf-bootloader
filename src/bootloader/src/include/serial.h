/**
 * @file serial.h
 * @author ajxs
 * @date Aug 2019
 * @brief Functionality for working with the serial port.
 * Contains functionality for initiating and working with the serial IO service.
 */

#ifndef BOOTLOADER_SERIAL_H
#define BOOTLOADER_SERIAL_H 1

#include <efi.h>
#include <efilib.h>

/** The maximum string length able to be printed to the serial. */
#define MAX_SERIAL_OUT_STRING_LENGTH 512

/**
 * @brief The serial service.
 * Contains the variables necessary to use the UEFI serial service.
 */
typedef struct s_uefi_serial_service {
	EFI_SERIAL_IO_PROTOCOL* protocol;
} Uefi_Serial_Service;

/**
 * @brief Configures an individual Serial IO protocol instance.
 * Configures an individual Serial IO protocol instance. Sets the baud rate and
 * other device-specific options.
 * @param[in] protocol    The serial IO protocol instance to configure.
 * @return The program status.
 * @retval EFI_SUCCESS    If the function executed successfully.
 * @retval other          Any other value is an EFI error code.
 */
EFI_STATUS configure_serial_protocol(IN EFI_SERIAL_IO_PROTOCOL* const protocol);

/**
 * @brief Initialise the serial service.
 * Initialises the serial IO service, used for interacting with serial devices.
 * @return The program status.
 * @retval EFI_SUCCESS    If the function executed successfully.
 * @retval other          Any other value is an EFI error code.
 */
EFI_STATUS init_serial_service(void);

/**
 * @brief Prints to a serial protocol.
 * Prints a null terminated string to a particular serial protocol.
 * @param[in] protocol    The serial IO protocol instance to configure.
 * @param[in] line        The line to print.
 * @return The program status.
 * @retval EFI_SUCCESS            If the function executed successfully.
 * @retval EFI_INVALID_PARAMETER  If the supplied protocol is not properly
 *                                loaded or the supplied string is empty.
 * @retval EFI_BAD_BUFFER_SIZE    If the string is over the maximum length or
 *                                not properly null terminated.
 * @retval other                  Any other value is an EFI error code.
 * @warn If the string is not null terminated, this will result in an error.
 */
EFI_STATUS print_to_serial_out(IN EFI_SERIAL_IO_PROTOCOL* const protocol,
	IN CHAR16* line);

#endif
