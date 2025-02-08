/**
 * @file kernel.c
 * @author ajxs
 * @date Aug 2019
 * @brief Kernel entry.
 * Contains the kernel entry point.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <boot.h>
#include <graphics.h>
#include <uart.h>

/** Whether to draw a test pattern to video output. */
#define DRAW_TEST_SCREEN 1

#define TEST_SCREEN_COL_NUM             4
#define TEST_SCREEN_ROW_NUM             3
#define TEST_SCREEN_TOTAL_TILES         TEST_SCREEN_COL_NUM * TEST_SCREEN_ROW_NUM
#define TEST_SCREEN_PRIMARY_COLOUR      0x00FF40FF
#define TEST_SCREEN_SECONDARY_COLOUR    0x00FF00CF

/**
 * @brief Draws a test screen to the framebuffer.
 * Paints the XOR test texture to the screen.
 * Refer to: https://lodev.org/cgtutor/xortexture.html
 * @param[in] boot_info The boot information passed to the kernel. This contains
 * a pointer to the framebuffer, and the associated video mode information
 * needed to draw the test screen.
 */
static void draw_test_screen(Boot_Info* boot_info);

/**
 * @brief The kernel main program.
 * This is the kernel main entry point and its main program.
 */
void kernel_main(Boot_Info* boot_info);


/**
 * draw_test_screen
 */
static void draw_test_screen(Boot_Info* boot_info)
{
	uint8_t c = 0;
	uint32_t colour = 0;
	uint16_t x = 0;
	uint16_t y = 0;

	for(y = 0; y < boot_info->video_mode_info.vertical_resolution; y++) {
		for(x = 0; x < boot_info->video_mode_info.horizontal_resolution; x++) {
			c = (x ^ y) % 256;
			colour = convert_rgb_to_32bit_colour(255 - (c % 128), c, c % 128);

			draw_pixel(
				boot_info->video_mode_info.framebuffer_pointer,
				boot_info->video_mode_info.pixels_per_scaline,
				x,
				y,
				colour
			);
		}
	}
}


/**
 * kernel_main
 */
void kernel_main(Boot_Info* boot_info)
{
	// Initialise the UART.
	uart_initialize();
	uart_puts("Kernel: Initialised.\n");

	#if DRAW_TEST_SCREEN
		draw_test_screen(boot_info);
	#endif

	while(1);
}
