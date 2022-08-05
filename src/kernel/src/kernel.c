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
#include <vga.h>

/**
 * Whether to draw a test pattern to video output.
 */
#define DRAW_TEST_SCREEN 1

#define TEST_SCREEN_COL_NUM             4
#define TEST_SCREEN_ROW_NUM             3
#define TEST_SCREEN_TOTAL_TILES         TEST_SCREEN_COL_NUM * TEST_SCREEN_ROW_NUM
#define TEST_SCREEN_PRIMARY_COLOUR      0x00FF40FF
#define TEST_SCREEN_SECONDARY_COLOUR    0x00FF00CF

/**
 * @brief Draws a test screen to the framebuffer.
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
	const uint16_t tile_width = boot_info->video_mode_info.horizontal_resolution /
		TEST_SCREEN_COL_NUM;
	const uint16_t tile_height = boot_info->video_mode_info.vertical_resolution /
		TEST_SCREEN_ROW_NUM;

	uint8_t p = 0;
	for(p = 0; p < TEST_SCREEN_TOTAL_TILES; p++) {
		uint8_t _x = p % TEST_SCREEN_COL_NUM;
		uint8_t _y = p / TEST_SCREEN_COL_NUM;

		uint32_t color = TEST_SCREEN_PRIMARY_COLOUR;
		if(((_y % 2) + _x) % 2) {
			color = TEST_SCREEN_SECONDARY_COLOUR;
		}

		draw_rect(boot_info->video_mode_info.framebuffer_pointer,
			boot_info->video_mode_info.pixels_per_scaline,
			tile_width * _x, tile_height * _y,
			tile_width, tile_height, color);
	}
}


/**
 * kernel_main
 */
void kernel_main(Boot_Info* boot_info)
{
	// Initialise VGA output.
	vga_initialize();

	// Initialise the UART.
	uart_initialize();
	uart_puts("Kernel: Initialised.\n");

	#if DRAW_TEST_SCREEN
		draw_test_screen(boot_info);
	#endif

	while(1);
}
