/**
 * @file vga.h
 * @author ajxs
 * @date Aug 2019
 * @brief VGA functionality.
 * Contains definitions for VGA functionality.
 */

#include <vga.h>
#include <stddef.h>
#include <stdint.h>

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

static size_t vga_row;
static size_t vga_column;
static uint8_t vga_color;
static uint16_t* vga_buffer;

/**
 * @brief Gets the size of a string.
 * Returns the number of chars making up a constant string.
 * @param str   The string to get the length of.
 * @return      The size of the string.
 */
static size_t strlen(const char* str)
{
	/** The length of the string to be output. */
	size_t len = 0;
	while(str[len]) {
		len++;
	}

	return len;
}

/**
 * create_vga_color_entry
 */
inline uint8_t create_vga_color_entry(enum vga_color fg,
	enum vga_color bg)
{
	return fg | bg << 4;
}


/**
 * create_vga_entry
 */
inline uint16_t create_vga_entry(unsigned char uc,
	uint8_t color)
{
	return (uint16_t) uc | (uint16_t) color << 8;
}


/**
 * vga_initialize
 */
void vga_initialize(void)
{
	vga_row = 0;
	vga_column = 0;
	vga_color = create_vga_color_entry(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	vga_buffer = (uint16_t*)0xC03FF000;

	for(size_t y = 0; y < VGA_HEIGHT; y++) {
		for(size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			vga_buffer[index] = create_vga_entry(' ', vga_color);
		}
	}
}


/**
 * vga_set_color
 */
void vga_set_color(uint8_t color)
{
	vga_color = color;
}


/**
 * @brief Places a VGA entry onto the screen.
 * Places an encoded VGA entry into a specified place in the buffer.
 * @param c        The character to place.
 * @param color    The color for the character entry.
 * @param x        The x coordinate for the entry.
 * @param y        The y coordinate for the entry.
 */
static void vga_putentryat(char c,
	uint8_t color,
	size_t x,
	size_t y)
{
	/** The index into the VGA buffer to place the entry at. */
	const size_t index = y * VGA_WIDTH + x;
	vga_buffer[index] = create_vga_entry(c, color);
}


/**
 * vga_putchar
 */
void vga_putchar(char c)
{
	vga_putentryat(c, vga_color, vga_column, vga_row);
	if(++vga_column == VGA_WIDTH) {
		vga_column = 0;

		if(++vga_row == VGA_HEIGHT) {
			vga_row = 0;
		}
	}
}


/**
 * vga_puts
 */
void vga_puts(const char* str)
{
	/** The length of the string. */
	const size_t len = strlen(str);

	for (size_t i = 0; i < len; i++) {
		vga_putchar(str[i]);
	}
}
