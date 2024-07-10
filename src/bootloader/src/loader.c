/**
 * @file loader.c
 * @author ajxs
 * @date Aug 2019
 * @brief Functionality for loading the Kernel executable.
 * Contains functionality for loading the Kernel ELF executable.
 */

#include <efi.h>
#include <efilib.h>

#include <bootloader.h>
#include <debug.h>
#include <elf.h>
#include <error.h>
#include <loader.h>
#include <serial.h>

/**
 * Whether to prompt, and wait for user input before rebooting in the case
 * of an unrecoverable error.
 */
#define PROMPT_FOR_INPUT_BEFORE_REBOOT_ON_FATAL_ERROR 1

/**
 * load_segment
 */
EFI_STATUS load_segment(IN EFI_FILE* const kernel_img_file,
	IN EFI_PHYSICAL_ADDRESS const segment_file_offset,
	IN UINTN const segment_file_size,
	IN UINTN const segment_memory_size,
	IN EFI_PHYSICAL_ADDRESS const segment_physical_address)
{
	/** Program status. */
	EFI_STATUS status;
	/** Buffer to hold the segment data. */
	VOID* program_data = NULL;
	/** The amount of data to read into the buffer. */
	UINTN buffer_read_size = 0;
	/** The number of pages to allocate. */
	UINTN segment_page_count = EFI_SIZE_TO_PAGES(segment_memory_size);
	/** The memory location to begin zero filling empty segment space. */
	EFI_PHYSICAL_ADDRESS zero_fill_start = 0;
	/** The number of bytes to zero fill. */
	UINTN zero_fill_count = 0;

	#ifdef DEBUG
		debug_print_line(L"Debug: Setting file pointer to segment "
			"offset '0x%llx'\n", segment_file_offset);
	#endif

	status = uefi_call_wrapper(kernel_img_file->SetPosition, 2,
		kernel_img_file, segment_file_offset);
	if(check_for_fatal_error(status, L"Error setting file pointer to segment offset")) {
		return status;
	}

	#ifdef DEBUG
		debug_print_line(L"Debug: Allocating %lu pages at address '0x%llx'\n",
			segment_page_count, segment_physical_address);
	#endif

	status = uefi_call_wrapper(gBS->AllocatePages, 4,
		AllocateAddress, EfiLoaderData, segment_page_count,
		(EFI_PHYSICAL_ADDRESS*)&segment_physical_address);
	if(check_for_fatal_error(status, L"Error allocating pages for ELF segment")) {
		return status;
	}

	if(segment_file_size > 0) {
		buffer_read_size = segment_file_size;

		#ifdef DEBUG
			debug_print_line(L"Debug: Allocating segment buffer with size '0x%llx'\n",
				buffer_read_size);
		#endif

		status = uefi_call_wrapper(gBS->AllocatePool, 3,
			EfiLoaderCode, buffer_read_size, (VOID**)&program_data);
		if(check_for_fatal_error(status, L"Error allocating kernel segment buffer")) {
			return status;
		}

		#ifdef DEBUG
			debug_print_line(L"Debug: Reading segment data with file size '0x%llx'\n",
				buffer_read_size);
		#endif

		status = uefi_call_wrapper(kernel_img_file->Read, 3,
			kernel_img_file, &buffer_read_size, (VOID*)program_data);
		if(check_for_fatal_error(status, L"Error reading segment data")) {
			return status;
		}

		#ifdef DEBUG
			debug_print_line(L"Debug: Copying segment to memory address '0x%llx'\n",
				segment_physical_address);
		#endif

		status = uefi_call_wrapper(gBS->CopyMem, 3,
			segment_physical_address, program_data, segment_file_size);
		if(check_for_fatal_error(status, L"Error copying program section into memory")) {
			return status;
		}

		#ifdef DEBUG
			debug_print_line(L"Debug: Freeing program section data buffer\n");
		#endif

		status = uefi_call_wrapper(gBS->FreePool, 1, program_data);
		if(check_for_fatal_error(status, L"Error freeing program section")) {
			return status;
		}
	}

	// As per ELF Standard, if the size in memory is larger than the file size
	// the segment is mandated to be zero filled.
	// For more information on Refer to ELF standard page 34.
	zero_fill_start = segment_physical_address + segment_file_size;
	zero_fill_count = segment_memory_size - segment_file_size;

	if(zero_fill_count > 0) {
		#ifdef DEBUG
			debug_print_line(L"Debug: Zero-filling %llu bytes at address '0x%llx'\n",
				zero_fill_count, zero_fill_start);
		#endif

		status = uefi_call_wrapper(gBS->SetMem, 3,
			zero_fill_start, zero_fill_count, 0);
		if(check_for_fatal_error(status, L"Error zero filling segment")) {
			return status;
		}
	}

	return EFI_SUCCESS;
}


/**
 * load_program_segments
 */
EFI_STATUS load_program_segments(IN EFI_FILE* const kernel_img_file,
	IN Elf_File_Class const file_class,
	IN VOID* const kernel_header_buffer,
	IN VOID* const kernel_program_headers_buffer)
{
	/** Program status. */
	EFI_STATUS status;
	/** The number of program headers. */
	UINT16 n_program_headers = 0;
	/** The number of segments loaded. */
	UINT16 n_segments_loaded = 0;
	/** Program section iterator. */
	UINTN p = 0;

	if(file_class == ELF_FILE_CLASS_32) {
		n_program_headers = ((Elf32_Ehdr*)kernel_header_buffer)->e_phnum;
	} else if(file_class == ELF_FILE_CLASS_64) {
		n_program_headers = ((Elf64_Ehdr*)kernel_header_buffer)->e_phnum;
	}

	// Exit if there are no executable sections in the kernel image.
	if(n_program_headers == 0) {
		debug_print_line(
			L"Fatal Error: No program segments to load in Kernel image\n"
		);

		return EFI_INVALID_PARAMETER;
	}

	#ifdef DEBUG
		debug_print_line(L"Debug: Loading %u segments\n", n_program_headers);
	#endif


	if(file_class == ELF_FILE_CLASS_32) {
		/** Program headers pointer. */
		Elf32_Phdr* program_headers = (Elf32_Phdr*)kernel_program_headers_buffer;

		for(p = 0; p < n_program_headers; p++) {
			if(program_headers[p].p_type == PT_LOAD) {
				status = load_segment(kernel_img_file,
					program_headers[p].p_offset,
					program_headers[p].p_filesz,
					program_headers[p].p_memsz,
					program_headers[p].p_paddr);
				if(EFI_ERROR(status)) {
					// Error has already been printed in the case that loading an
					// individual segment failed.
					return status;
				}

				// Increment the number of segments lodaed.
				n_segments_loaded++;
			}
		}
	} else if(file_class == ELF_FILE_CLASS_64) {
		/** Program headers pointer. */
		Elf64_Phdr* program_headers = (Elf64_Phdr*)kernel_program_headers_buffer;

		for(p = 0; p < n_program_headers; p++) {
			if(program_headers[p].p_type == PT_LOAD){
				status = load_segment(kernel_img_file,
					program_headers[p].p_offset,
					program_headers[p].p_filesz,
					program_headers[p].p_memsz,
					program_headers[p].p_paddr);
				if(EFI_ERROR(status)) {
					return status;
				}

				n_segments_loaded++;
			}
		}
	}

	// If we have found no loadable segments, raise an exception.
	if(n_segments_loaded == 0) {
		debug_print_line(L"Fatal Error: No loadable program segments ");
		debug_print_line(L"found in Kernel image\n");

		return EFI_NOT_FOUND;
	}

	return EFI_SUCCESS;
}


/**
 * load_kernel_image
 */
EFI_STATUS load_kernel_image(IN EFI_FILE* const root_file_system,
	IN CHAR16* const kernel_image_filename,
	OUT EFI_VIRTUAL_ADDRESS* kernel_entry_point)
{
	/** Program status. */
	EFI_STATUS status;
	/** The kernel file handle. */
	EFI_FILE* kernel_img_file;
	/** The kernel ELF header buffer. */
	VOID* kernel_header = NULL;
	/** The kernel program headers buffer. */
	VOID* kernel_program_headers = NULL;
	/** The ELF file identity buffer. */
	UINT8* elf_identity_buffer = NULL;
	/** The ELF file class. */
	Elf_File_Class file_class = ELF_FILE_CLASS_NONE;

	#ifdef DEBUG
		debug_print_line(L"Debug: Reading kernel image file\n");
	#endif

	status = uefi_call_wrapper(root_file_system->Open, 5,
		root_file_system, &kernel_img_file, kernel_image_filename,
		EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
	if(check_for_fatal_error(status, L"Error opening kernel file")) {
		return status;
	}

	// Read ELF Identity.
	// From here we can validate the ELF executable, as well as determine the
	// file class.
	status = read_elf_identity(kernel_img_file, &elf_identity_buffer);
	if(check_for_fatal_error(status, L"Error reading executable identity")) {
		return status;
	}

	file_class = elf_identity_buffer[EI_CLASS];

	// Validate the ELF file.
	status = validate_elf_identity(elf_identity_buffer);
	if(EFI_ERROR(status)) {
		// Error message printed in validation function.
		return status;
	}

	#ifdef DEBUG
		debug_print_line(L"Debug: ELF header is valid\n");
	#endif

	// Free identity buffer.
	status = uefi_call_wrapper(gBS->FreePool, 1, elf_identity_buffer);
	if(check_for_fatal_error(status, L"Error freeing kernel identity buffer")) {
		return status;
	}


	// Read the ELF file and program headers.
	status = read_elf_file(kernel_img_file, file_class,
		&kernel_header, &kernel_program_headers);
	if(check_for_fatal_error(status, L"Error reading ELF file")) {
		return status;
	}

	#ifdef DEBUG
		print_elf_file_info(kernel_header, kernel_program_headers);
	#endif

	// Set the kernel entry point to the address specified in the ELF header.
	if(file_class == ELF_FILE_CLASS_32) {
		*kernel_entry_point = ((Elf32_Ehdr*)kernel_header)->e_entry;
	} else if(file_class == ELF_FILE_CLASS_64) {
		*kernel_entry_point = ((Elf64_Ehdr*)kernel_header)->e_entry;
	}

	status = load_program_segments(kernel_img_file, file_class,
		kernel_header, kernel_program_headers);
	if(EFI_ERROR(status)) {
		// In the case that loading the kernel segments failed, the error message will
		// have already been printed.
		return status;
	}

	// Cleanup.
	#ifdef DEBUG
		debug_print_line(L"Debug: Closing kernel binary\n");
	#endif

	status = uefi_call_wrapper(kernel_img_file->Close, 1, kernel_img_file);
	if(check_for_fatal_error(status, L"Error closing kernel image")) {
		return status;
	}

	#ifdef DEBUG
		debug_print_line(L"Debug: Freeing kernel header buffer\n");
	#endif

	status = uefi_call_wrapper(gBS->FreePool, 1, (VOID*)kernel_header);
	if(check_for_fatal_error(status, L"Error freeing kernel header buffer")) {
		return status;
	}

	#ifdef DEBUG
		debug_print_line(L"Debug: Freeing kernel program header buffer\n");
	#endif

	status = uefi_call_wrapper(gBS->FreePool, 1, (VOID*)kernel_program_headers);
	if(check_for_fatal_error(status, L"Error freeing kernel program headers buffer")) {
		return status;
	}


	return status;
}
