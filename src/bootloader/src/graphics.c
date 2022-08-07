/**
 * @file graphics.c
 * @author ajxs
 * @date Aug 2019
 * @brief Functionality for working with the graphics protocol.
 * Contains functionality for initiating and  working with the graphics protocol and
 * its associated framebuffers.
 */

#include <efi.h>
#include <efilib.h>

#include <bootloader.h>
#include <error.h>
#include <graphics.h>

#define TEST_SCREEN_COL_NUM             4
#define TEST_SCREEN_ROW_NUM             3
#define TEST_SCREEN_TOTAL_TILES         TEST_SCREEN_COL_NUM * TEST_SCREEN_ROW_NUM
#define TEST_SCREEN_PRIMARY_COLOUR      0x00FF4000
#define TEST_SCREEN_SECONDARY_COLOUR    0x00FF80BF

/**
 * @brief Finds a video mode.
 * Finds a particular video mode by its width, height and pixel format.
 * Tests all video modes copatible with the provided protocol, populating the
 * `video_mode` variable on success.
 * @param[in]     protocol The protocol to find the video mode in.
 * @param[in]     target_width The target width to search for.
 * @param[in]     target_height The target height to search for.
 * @param[in]     target_pixel_format The target pixel format to search for.
 * @param[out]    video_mode The video mode variable to populate.
 * @return The program status.
 * @retval EFI_SUCCESS    If the function executed successfully.
 * @retval other          Any other value is an EFI error code.
 */
EFI_STATUS find_video_mode(IN EFI_GRAPHICS_OUTPUT_PROTOCOL* const protocol,
	IN const UINT32 target_width,
	IN const UINT32 target_height,
	IN const EFI_GRAPHICS_PIXEL_FORMAT target_pixel_format,
	OUT UINTN* video_mode);


/**
 * close_graphic_output_service
 */
EFI_STATUS close_graphic_output_service()
{
	EFI_STATUS status = uefi_call_wrapper(gBS->FreePool, 1,
		graphics_service.handle_buffer);

	return status;
}


/**
 * draw_rect
 */
VOID draw_rect(IN EFI_GRAPHICS_OUTPUT_PROTOCOL* const protocol,
	IN const UINT16 _x,
	IN const UINT16 _y,
	IN const UINT16 width,
	IN const UINT16 height,
	IN const UINT32 color)
{
	/** Pointer to the current pixel in the buffer. */
	UINT32* at;

	UINT16 row = 0;
	UINT16 col = 0;
	for(row = 0; row < height; row++) {
		for(col = 0; col < width; col++) {
			at = (UINT32*)protocol->Mode->FrameBufferBase + _x + col;
			at += ((_y + row) * protocol->Mode->Info->PixelsPerScanLine);

			*at = color;
		}
	}
}


/**
 * draw_test_screen
 */
VOID draw_test_screen(IN EFI_GRAPHICS_OUTPUT_PROTOCOL* const protocol)
{
	const UINT16 tile_width = protocol->Mode->Info->HorizontalResolution /
		TEST_SCREEN_COL_NUM;
	const UINT16 tile_height = protocol->Mode->Info->VerticalResolution /
		TEST_SCREEN_ROW_NUM;

	UINT8 p = 0;
	for(p = 0; p < TEST_SCREEN_TOTAL_TILES; p++) {
		UINT8 _x = p % TEST_SCREEN_COL_NUM;
		UINT8 _y = p / TEST_SCREEN_COL_NUM;

		UINT32 color = TEST_SCREEN_PRIMARY_COLOUR;
		if(((_y % 2) + _x) % 2) {
			color = TEST_SCREEN_SECONDARY_COLOUR;
		}

		draw_rect(protocol, tile_width * _x, tile_height * _y,
			tile_width, tile_height, color);
	}
}


/**
 * find_video_mode
 */
EFI_STATUS find_video_mode(IN EFI_GRAPHICS_OUTPUT_PROTOCOL* const protocol,
	IN const UINT32 target_width,
	IN const UINT32 target_height,
	IN const EFI_GRAPHICS_PIXEL_FORMAT target_pixel_format,
	OUT UINTN* video_mode)
{
	/** Program status. */
	EFI_STATUS status;
	/** The size of the video mode info struct. */
	UINTN size_of_mode_info;
	/** The video mode info struct. */
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* mode_info;

	UINTN i = 0;
	for(i = 0; i < protocol->Mode->MaxMode; i++) {
		#ifdef DEBUG
			debug_print_line(L"Debug: Testing video mode: '%llu'\n", i);
		#endif

		status = uefi_call_wrapper(protocol->QueryMode, 4,
			protocol, i, &size_of_mode_info, &mode_info);
		if(EFI_ERROR(status)) {
			debug_print_line(L"Error: Error querying video mode: %s\n",
				get_efi_error_message(status));

			return status;
		}

		if(mode_info->HorizontalResolution == target_width &&
			mode_info->VerticalResolution == target_height &&
			mode_info->PixelFormat == target_pixel_format) {

			#ifdef DEBUG
				debug_print_line(L"Debug: Matched video mode: '%llu' for '%lu*%lu*%u'\n", i,
					target_width, target_height, target_pixel_format);
			#endif

			*video_mode = i;
			return EFI_SUCCESS;
		}
	}

	return EFI_UNSUPPORTED;
}


/**
 * init_graphics_output_service
 */
EFI_STATUS init_graphics_output_service(void)
{
	/** Program status. */
	EFI_STATUS status;

	#ifdef DEBUG
		debug_print_line(L"Debug: Initialising Graphics Output Service\n");
	#endif

	// Populate graphics service handle buffer.
	status = uefi_call_wrapper(gBS->LocateHandleBuffer, 5,
		ByProtocol, &gEfiGraphicsOutputProtocolGuid, NULL,
		&graphics_service.handle_count, &graphics_service.handle_buffer);
	if(EFI_ERROR(status)) {
		debug_print_line(L"Error: Error locating GOP handle buffer: %s\n",
			get_efi_error_message(status));

		return status;
	}

	#ifdef DEBUG
		debug_print_line(L"Debug: Located GOP handle buffer with %u handles\n",
			graphics_service.handle_count);
	#endif

	return EFI_SUCCESS;
}


/**
 * set_graphics_mode
 */
EFI_STATUS set_graphics_mode(IN EFI_GRAPHICS_OUTPUT_PROTOCOL* const protocol,
	IN const UINT32 target_width,
	IN const UINT32 target_height,
	IN const EFI_GRAPHICS_PIXEL_FORMAT target_pixel_format)
{
	/** Program status. */
	EFI_STATUS status;
	/** The graphics mode number. */
	UINTN graphics_mode_num = 0;

	status = find_video_mode(protocol, target_width, target_height,
		target_pixel_format, &graphics_mode_num);
	if(EFI_ERROR(status)) {
		// Error will already have been printed.
		return status;
	}

	status = uefi_call_wrapper(protocol->SetMode, 2,
		protocol, graphics_mode_num);
	if(EFI_ERROR(status)) {
		debug_print_line(L"Error: Error setting graphics mode: %s\n",
			get_efi_error_message(status));

		return status;
	}

	return EFI_SUCCESS;
}
