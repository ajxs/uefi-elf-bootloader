/**
 * @file graphics.h
 * @author ajxs
 * @date Aug 2019
 * @brief Functionality for working with the graphics protocol.
 * Contains functionality for initiating and working with the graphics protocol and
 * its associated framebuffers.
 */

#ifndef BOOTLOADER_GRAPHICS_H
#define BOOTLOADER_GRAPHICS_H 1

#include <efi.h>
#include <efilib.h>

/**
 * @brief Graphics output service.
 * Holds variables necesssary for using the UEFI graphics output service.
 */
typedef struct s_uefi_graphics_service {
	EFI_HANDLE* handle_buffer;
	UINTN handle_count;
} Uefi_Graphics_Service;

/**
 * @brief Draws a rectangle onto the framebuffer.
 * Draws a rectangle onto the frame buffer of a particular protocol.
 * @param[in]    protocol The protocol containing the framebuffer to draw to.
 * @param[in]    _x The x coordinate to draw the rect to.
 * @param[in]    _y The y coordinate to draw the rect to.
 * @param[in]    width The width of the rectangle to draw.
 * @param[in]    height The height of the rectangle to draw.
 * @param[in]    color The color to draw.
 */
VOID draw_rect(IN EFI_GRAPHICS_OUTPUT_PROTOCOL* const protocol,
	IN const UINT16 _x,
	IN const UINT16 _y,
	IN const UINT16 _width,
	IN const UINT16 _height,
	IN const UINT32 color);

/**
 * @brief Draws a test screen.
 * Draws a test screen which can be used to assess that the graphics output
 * protocol is functioning correctly. Draws to the provided framebuffer.
 * @param[in] protocol The protocol containing the framebuffer to draw to.
 */
VOID draw_test_screen(IN EFI_GRAPHICS_OUTPUT_PROTOCOL* const protocol);

/**
 * @brief Closes the graphics output service.
 * Closes the graphics output service, freeing the handle buffer used by the
 * service.
 * @return The program status.
 * @retval EFI_SUCCESS    If the function executed successfully.
 * @retval other          Any other value is an EFI error code.
 */
EFI_STATUS close_graphic_output_service(void);

/**
 * @brief Initialises the Graphics output service.
 * Initialises the graphics output service. Populates the handle buffer in the
 * graphics output service.
 * @return The program status.
 * @retval EFI_SUCCESS    If the function executed successfully.
 * @retval other          Any other value is an EFI error code.
 */
EFI_STATUS init_graphics_output_service(void);

/**
 * @brief Set the graphics mode for a particular protocol.
 * Sets the graphics mode for a particular protocol handle. Sets the graphics
 * mode by searching all available modes on this protocol for one that matches
 * the target width/height.
 * @param[in]    protocol The protocol to set the mode for.
 * @param[in]    target_width The target width.
 * @param[in]    target_height The target height.
 * @param[in]    target_pixel_format The target pixel format.
 * @return The program status.
 * @retval EFI_SUCCESS        If the function executed successfully.
 * @retval EFI_UNSUPPORTED    if the specified mode cannot be found.
 * @retval other              Any other value is an EFI error code.
 */
EFI_STATUS set_graphics_mode(IN EFI_GRAPHICS_OUTPUT_PROTOCOL* const protocol,
	IN const UINT32 target_width,
	IN const UINT32 target_height,
	IN const EFI_GRAPHICS_PIXEL_FORMAT target_pixel_format);

#endif
