/**
 * @file main.c
 * @author ajxs
 * @date Aug 2019
 * @brief Bootloader entry point and main application.
 * The entry point for the application. Contains the main bootloader code that
 * initiates the loading of the Kernel executable. The main application is
 * contained within the `efi_main` function.
 */

#include <efi.h>
#include <efilib.h>
#include <stdarg.h>
#include <elf.h>

#include <bootloader.h>
#include <debug.h>
#include <error.h>
#include <fs.h>
#include <graphics.h>
#include <serial.h>
#include <memory_map.h>

#define TARGET_SCREEN_WIDTH     1024
#define TARGET_SCREEN_HEIGHT    768
#define TARGET_PIXEL_FORMAT     PixelBlueGreenRedReserved8BitPerColor

/**
 * Graphics Service instance.
 * Refer to definition in bootloader.h
 */
Uefi_Graphics_Service graphics_service;
/**
 * File System Service instance.
 * Refer to definition in bootloader.h
 */
Uefi_File_System_Service file_system_service;
/**
 * Serial IO Service instance.
 * Refer to definition in bootloader.h
 */
Uefi_Serial_Service serial_service;

/**
 * Whether to draw a test pattern to video output to test the graphics output
 * service.
 */
#define DRAW_TEST_SCREEN 1


/**
 * efi_main
 */
EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle,
	EFI_SYSTEM_TABLE* SystemTable)
{
	/** Main bootloader application status. */
	EFI_STATUS status;
	/**
	 * Graphics output protocol instance.
	 * This protocol instance will be opened from the active console out handle.
	 */
	EFI_GRAPHICS_OUTPUT_PROTOCOL* graphics_output_protocol = NULL;
	/**
	 * The root file system entity.
	 * This is the file root from which the kernel binary will be loaded.
	 */
	EFI_FILE* root_file_system;
	/** The kernel entry point address. */
	EFI_PHYSICAL_ADDRESS* kernel_entry_point = 0;
	/** The EFI memory map descriptor. */
	EFI_MEMORY_DESCRIPTOR* memory_map = NULL;
	/** The memory map key. */
	UINTN memory_map_key = 0;
	/** The size of the memory map buffer. */
	UINTN memory_map_size = 0;
	/** The memory map descriptor size. */
	UINTN descriptor_size;
	/** The memory map descriptor. */
	UINT32 descriptor_version;
	/** Function pointer to the kernel entry point. */
	void (*kernel_entry)(Kernel_Boot_Info* boot_info);
	/** Boot info struct, passed to the kernel. */
	Kernel_Boot_Info boot_info;
	/** Input key type used to capture user input. */
	EFI_INPUT_KEY input_key;

	// Initialise service protocols to NULL, so that we can detect if they are
	// properly initialised in service functions.
	serial_service.protocol = NULL;
	file_system_service.protocol = NULL;

	// Initialise the UEFI lib.
	InitializeLib(ImageHandle, SystemTable);

	// Disable the watchdog timer.
	status = uefi_call_wrapper(gBS->SetWatchdogTimer, 4, 0, 0, 0, NULL);
	if(check_for_fatal_error(status, L"Error setting watchdog timer")) {
		return status;
	}

	// Reset console input.
	status = uefi_call_wrapper(ST->ConIn->Reset, 2, SystemTable->ConIn, FALSE);
	if(check_for_fatal_error(status, L"Error resetting console input")) {
		return status;
	}

	// Initialise the serial service.
	status = init_serial_service();
	if(EFI_ERROR(status)) {
		if(status == EFI_NOT_FOUND) {
			#ifdef DEBUG
				debug_print_line(L"Debug: No serial device found\n");
			#endif
		} else {
			debug_print_line(L"Fatal Error: Error initialising Serial IO service\n");

			#if PROMPT_FOR_INPUT_BEFORE_REBOOT_ON_FATAL_ERROR
				debug_print_line(L"Press any key to reboot...");
				wait_for_input(&input_key);
			#endif

			return status;
		}
	}

	// Initialise the graphics output service.
	status = init_graphics_output_service();
	if(EFI_ERROR(status)) {
		if(status == EFI_NOT_FOUND) {
			#ifdef DEBUG
				debug_print_line(L"Debug: No graphics device found\n");
			#endif
		} else {
			debug_print_line(L"Fatal Error: Error initialising Graphics service\n");

			#if PROMPT_FOR_INPUT_BEFORE_REBOOT_ON_FATAL_ERROR
				debug_print_line(L"Press any key to reboot...");
				wait_for_input(&input_key);
			#endif

			return status;
		}
	}


	// Open the graphics output protocol from the handle for the active console
	// output device and use it to draw the boot screen.
	// The console out handle exposed by the System Table is documented in the
	// UEFI Spec page 92.
	status = uefi_call_wrapper(gBS->OpenProtocol, 6,
		ST->ConsoleOutHandle, &gEfiGraphicsOutputProtocolGuid,
		&graphics_output_protocol, ImageHandle,
		NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
	if(EFI_ERROR(status)) {
		debug_print_line(L"Error: Failed to open the graphics output protocol on "
			L"the active console output device: %s\n", get_efi_error_message(status));
	}

	// If we were able to obtain a protocol on the current output device handle
	// set the graphics mode to the target and draw the boot screen.
	if(graphics_output_protocol) {
		status = set_graphics_mode(graphics_output_protocol, TARGET_SCREEN_WIDTH,
			TARGET_SCREEN_HEIGHT, TARGET_PIXEL_FORMAT);
		if(EFI_ERROR(status)) {
			// Error has already been printed.
			return status;
		}

		#if DRAW_TEST_SCREEN != 0
			draw_test_screen(graphics_output_protocol);
		#endif
	}

	// Initialise the simple file system service.
	// This will be used to load the kernel binary.
	status = init_file_system_service();
	if(EFI_ERROR(status)) {
		// Error has already been printed.
		return status;
	}

	status = uefi_call_wrapper(file_system_service.protocol->OpenVolume, 2,
		file_system_service.protocol, &root_file_system);
	if(check_for_fatal_error(status, L"Error opening root volume")) {
		return status;
	}

	#ifdef DEBUG
		debug_print_line(L"Debug: Loading Kernel image\n");
	#endif

	status = load_kernel_image(root_file_system, KERNEL_EXECUTABLE_PATH,
		kernel_entry_point);
	if(EFI_ERROR(status)) {
		// In the case that loading the kernel image failed, the error message will
		// have already been printed.
		return status;
	}

	#ifdef DEBUG
		debug_print_line(L"Debug: Set Kernel Entry Point to: '0x%llx'\n",
			*kernel_entry_point);
	#endif

	boot_info.video_mode_info.framebuffer_pointer =
		(VOID*)graphics_output_protocol->Mode->FrameBufferBase;
	boot_info.video_mode_info.horizontal_resolution =
		graphics_output_protocol->Mode->Info->HorizontalResolution;
	boot_info.video_mode_info.vertical_resolution =
		graphics_output_protocol->Mode->Info->VerticalResolution;
	boot_info.video_mode_info.pixels_per_scaline =
		graphics_output_protocol->Mode->Info->PixelsPerScanLine;

	#ifdef DEBUG
		debug_print_line(L"Debug: Closing Graphics Output Service handles\n");
	#endif

	status = close_graphic_output_service();
	if(check_for_fatal_error(status, L"Error closing Graphics Output service")) {
		return status;
	}

	#ifdef DEBUG
		debug_print_line(L"Debug: Getting memory map and exiting boot services\n");
	#endif

	// Get the memory map prior to exiting the boot service.
	status = get_memory_map((VOID**)&memory_map, &memory_map_size,
		&memory_map_key, &descriptor_size, &descriptor_version);
	if(EFI_ERROR(status)) {
		// Error has already been printed.
		return status;
	}

	debug_print_memory_map(memory_map, memory_map_size, descriptor_size);

	// Get the memory map prior to exiting the boot service.
	status = get_memory_map((VOID**)&memory_map, &memory_map_size,
		&memory_map_key, &descriptor_size, &descriptor_version);
	if(EFI_ERROR(status)) {
		// Error has already been printed.
		return status;
	}

	status = uefi_call_wrapper(gBS->ExitBootServices, 2,
		ImageHandle, memory_map_key);
	if(check_for_fatal_error(status, L"Error exiting boot services")) {
		return status;
	}

	// Set kernel boot info.
	boot_info.memory_map = memory_map;
	boot_info.memory_map_size = memory_map_size;
	boot_info.memory_map_descriptor_size = descriptor_size;

	// Cast pointer to kernel entry.
	kernel_entry = (void (*)(Kernel_Boot_Info*))*kernel_entry_point;
	// Jump to kernel entry.
	kernel_entry(&boot_info);

	// Return an error if this code is ever reached.
	return EFI_LOAD_ERROR;
}


/**
 * wait_for_input
 */
EFI_STATUS wait_for_input(OUT EFI_INPUT_KEY* key) {
	/** The program status. */
	EFI_STATUS status;
	do {
		status = uefi_call_wrapper(ST->ConIn->ReadKeyStroke, 2, ST->ConIn, key);
	} while(status == EFI_NOT_READY);

	return status;
}
