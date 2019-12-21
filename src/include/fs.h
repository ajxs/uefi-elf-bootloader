/**
 * @file fs.h
 * @author ajxs
 * @date Aug 2019
 * @brief Functionality for working with the file system.
 * Contains functionality for initiating and working with the file system service.
 */

#ifndef BOOTLOADER_FS_H
#define BOOTLOADER_FS_H 1

#include <efi.h>
#include <efilib.h>

/**
 * @brief The file system service.
 * Holds the protocol variables necessary to use the simple file system protocol.
 */
typedef struct s_uefi_simple_file_system_service {
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* protocol;
} Uefi_File_System_Service;

/**
 * @brief Initialise the file system service.
 * Initialises the UEFI simple file system service, used for interacting with
 * the file system.
 * Refer to: https://mjg59.dreamwidth.org/18773.html?thread=768085#cmt768085
 * @return The program status.
 * @retval EFI_SUCCESS    If the function executed successfully.
 * @retval other          Any other value is an EFI error code.
 */
EFI_STATUS init_file_system_service(void);

#endif
