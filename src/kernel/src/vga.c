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
#include <string.h>

/** VGA Screen dimensions. */
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

/** The VGA row cursor. */
static size_t vga_row;
/** The VGA column cursor. */
static size_t vga_column;
/** The currently selected VGA color. */
static uint8_t vga_color;
/** Pointer to the machine's VGA buffer. */
static uint16_t* vga_buffer;


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
 * vga_putchar
 */
void vga_putchar(char c)
{
	/** The index into the VGA buffer to place the entry at. */
	const size_t index = vga_row * VGA_WIDTH + vga_column;
	vga_buffer[index] = create_vga_entry(c, vga_color);

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
