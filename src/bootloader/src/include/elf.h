/**
 * @file elf.h
 * @author ajxs
 * @date Aug 2019
 * @brief Functionality for working with ELF executable files.
 * Contains functionality to assist in loading and validating ELF executable
 * files. This functionality is essential to the ELF executable loader.
 */

#ifndef BOOTLOADER_ELF_H
#define BOOTLOADER_ELF_H 1

#include <efi.h>
#include <efilib.h>


#define EI_NIDENT        16

#define EI_MAG0          0
#define EI_MAG1          0x1
#define EI_MAG2          0x2
#define EI_MAG3          0x3
#define EI_CLASS         0x4
#define EI_DATA          0x5
#define EI_VERSION       0x6
#define EI_OSABI         0x7
#define EI_ABIVERSION    0x8

#define PT_NULL            0
#define PT_LOAD            1
#define PT_DYNAMIC         2
#define PT_INTERP          3
#define PT_NOTE            4
#define PT_SHLIB           5
#define PT_PHDR            6
#define PT_TLS             7


/**
 * @brief The ELF file class.
 */
typedef enum e_file_class {
	ELF_FILE_CLASS_NONE = 0,
	ELF_FILE_CLASS_32 = 1,
	ELF_FILE_CLASS_64 = 2,
} Elf_File_Class;

/**
 * @brief The 32-bit ELF header.
 */
typedef struct s_elf32_hdr {
	unsigned char	e_ident[EI_NIDENT];
	UINT16 e_type;
	UINT16 e_machine;
	UINT32 e_version;
	UINT32 e_entry;
	UINT32 e_phoff;
	UINT32 e_shoff;
	UINT32 e_flags;
	UINT16 e_ehsize;
	UINT16 e_phentsize;
	UINT16 e_phnum;
	UINT16 e_shentsize;
	UINT16 e_shnum;
	UINT16 e_shstrndx;
} Elf32_Ehdr;

/**
 * @brief The 64-bit ELF header.
 */
typedef struct s_elf64_hdr {
	unsigned char	e_ident[EI_NIDENT];
	UINT16 e_type;
	UINT16 e_machine;
	UINT32 e_version;
	UINT64 e_entry;
	UINT64 e_phoff;
	UINT64 e_shoff;
	UINT32 e_flags;
	UINT16 e_ehsize;
	UINT16 e_phentsize;
	UINT16 e_phnum;
	UINT16 e_shentsize;
	UINT16 e_shnum;
	UINT16 e_shstrndx;
} Elf64_Ehdr;

/**
 * @brief The 32-bit ELF program header.
 */
typedef struct s_elf32_phdr {
	UINT32 p_type;
	UINT32 p_offset;
	UINT32 p_vaddr;
	UINT32 p_paddr;
	UINT32 p_filesz;
	UINT32 p_memsz;
	UINT32 p_flags;
	UINT32 p_align;
} Elf32_Phdr;

/**
 * @brief The 64-bit ELF program header.
 */
typedef struct s_elf64_phdr {
	UINT32 p_type;
	UINT32 p_flags;
	UINT64 p_offset;
	UINT64 p_vaddr;
	UINT64 p_paddr;
	UINT64 p_filesz;
	UINT64 p_memsz;
	UINT64 p_align;
} Elf64_Phdr;

/**
 * @brief Prints ELF file information.
 * Prints information on the ELF file, as well as its program headers.
 * @param[in]   header_ptr A pointer to the ELF header buffer.
 * @param[in]   program_headers_ptr A pointer to the ELF program headers buffer.
 */
VOID print_elf_file_info(IN VOID* const header_ptr,
	IN VOID* const program_headers_ptr);

/**
 * @brief Reads the ELF file headers.
 * Reads the ELF file header and program headers into memory.
 * @param[in]    kernel_img_file The Kernel EFI file entity to read from.
 * @param[in]    file_class The ELF file class, whether the program
 *               is 32 or 64bit.
 * @param[out]   kernel_header_buffer The buffer to read the kernel header into.
 * @param[out]   kernel_program_headers_buffer The buffer to read the
 *               kernel program headers into.
 * @return The program status.
 * @retval EFI_SUCCESS    If the function executed successfully.
 * @retval other          Any other value is an EFI error code.
 */
EFI_STATUS read_elf_file(IN EFI_FILE* const kernel_img_file,
	IN Elf_File_Class const file_class,
	OUT VOID** kernel_header_buffer,
	OUT VOID** kernel_program_headers_buffer);

/**
 * @brief Reads the identity buffer of an ELF file.
 * Reads the identity buffer from the ELF header, which is used to both validate
 * that the file is a valid ELF executable, as well as read the ELF file class
 * value.
 * @param[in]   kernel_img_file The kernel binary to read the file class from.
 * @param[out]  elf_identity_buffer A pointer-to-pointer to the identity buffer
 *              to read the buffer into.
 * @return The program status.
 * @retval EFI_SUCCESS    If the function executed successfully.
 * @retval other          Any other value is an EFI error code.
 */
EFI_STATUS read_elf_identity(IN EFI_FILE* const kernel_img_file,
	OUT UINT8** elf_identity_buffer);

/**
 * @brief Validates the ELF file identity.
 * Validates whether the ELF identity correctly identifies an ELF file.
 * @param[in]  elf_identity_buffer The ELF identity buffer to validate.
 * @return The result of the validation.
 * @retval EFI_SUCCESS               Indicates a valid ELF file.
 * @retval EFI_INVALID_PARAMETER     If the ELF file identity buffer is invalid.
 * @retval EFI_UNSUPPORTED           If the identity buffer indicates an unsupported
 *                                   filetype.
 * @retval EFI_INCOMPATIBLE_VERSION  If the identity buffer indicates an unsupported
 *                                   file type.
 */
EFI_STATUS validate_elf_identity(IN UINT8* const elf_identity_buffer);

#endif
