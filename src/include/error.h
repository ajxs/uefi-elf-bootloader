/**
 * @file error.h
 * @author ajxs
 * @date Aug 2019
 * @brief Functionality for printing error messages.
 * Contains functionality for printing error messages that occur within the application
 * and assisting with error handling.
 */

#ifndef BOOTLOADER_ERROR_H
#define BOOTLOADER_ERROR_H 1

#include <efi.h>
#include <efilib.h>

/**
 * @brief Gets a formatted string explaining an EFI error.
 * Returns a C string representing an EFI error encountered in a human
 * interpretable format.
 * @param[in] status    The EFI_STATUS encountered.
 * @return              A const string explaining the error message.
 */
const CHAR16* get_efi_error_message(IN EFI_STATUS const status);

#endif
