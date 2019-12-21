/**
 * @file bootloader.h
 * @author ajxs
 * @date Aug 2019
 * @brief Core bootloader functionality.
 * Functions that are part of the core bootloader functionality.
 */

#ifndef BOOTLOADER_H
#define BOOTLOADER_H 1

#include <efi.h>
#include <efilib.h>

#include <fs.h>
#include <graphics.h>
#include <serial.h>

/**
 * @brief The main UEFI executable entry.
 * The main bootloader entry point.
 * @param[in] ImageHandle    The firmware allocated handle for the EFI image.
 * @param[in] SystemTable    A pointer to the EFI System Table.
 * @return                   The program status.
 * @retval EFI_SUCCESS       The function executed successfully.
 * @retval other             A fatal error occurred during the execution of the
 *                           main bootloader function.
 */
EFI_STATUS EFIAPI efi_main (EFI_HANDLE ImageHandle,
	EFI_SYSTEM_TABLE* SystemTable);

/**
 * @brief Loads the Kernel binary image into memory.
 * This will load the Kernel binary image and validates it. If the kernel binary
 * is valid its executable program segments are loaded into memory.
 * @param[in]   root_file_system The root file system FILE entity to load the
 *              kernel binary from.
 * @param[in]   kernel_image_filename The kernel filename on the boot partition.
 * @param[out]  kernel_entry_point The entry point memory address for
 *              the kernel.
 * @return The program status.
 * @retval EFI_SUCCESS    If the function executed successfully.
 * @retval other          Any other value is an EFI error code.
 */
EFI_STATUS load_kernel_image(IN EFI_FILE* const root_file_system,
	IN CHAR16* const kernel_image_filename,
	OUT EFI_PHYSICAL_ADDRESS* kernel_entry_point);

/**
 * @brief Pauses the program while waiting for input.
 * Pauses the program while waiting for a keystroke from console in, capturing
 * the input to a key parameter.
 * @param[out] key    A pointer to the entity to capture the keypress data in.
 * @return The program status.
 * @retval EFI_SUCCESS    If the function executed successfully.
 * @retval other          Any other value is an EFI error code.
 */
EFI_STATUS wait_for_input(EFI_INPUT_KEY* key);

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
EFI_STATUS debug_print_line(IN CHAR16* fmt,
	...);

/**
 * @brief The global graphics service entity.
 * Contains a collection of EFI handles supporting the Graphics Output Protocol.
 * Used for accessing the framebuffers of each supporting device.
 */
Uefi_Graphics_Service graphics_service;
/**
 * @brief The Simple File System service.
 * Contains the Simple File System protocol, used to interact with file system entities.
 */
Uefi_File_System_Service file_system_service;
/**
 * @brief The Serial IO service.
 * The Serial IO. Used for outputting text to a serial port.
 */
Uefi_Serial_Service serial_service;

#endif
