/**
 * @file vga.h
 * @author ajxs
 * @date Aug 2019
 * @brief VGA functionality.
 * Contains definitions for VGA functionality.
 */

#ifndef VGA_H
#define VGA_H

#include <stdint.h>

/**
 * @brief VGA Colors.
 * VGA Color codes. These are combined together to form a VGA color entry.
 */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};


/**
 * @brief Creates a VGA color entry.
 * Encodes two color codes into a VGA color entry, consisting of a fore
 * and background color.
 * @param fg The foreground color.
 * @param bg The background color.
 * @return The encoded VGA color entry.
 */
uint8_t create_vga_color_entry(enum vga_color fg,
	enum vga_color bg);

/**
 * @brief Creates a VGA character entry.
 * Encodes a VGA character entry, consisting of a character and its color entry.
 * @param uc       The character to encode.
 * @param color    The color to encode into the entry.
 * @retrn          The encoded VGA entry.
 */
uint16_t create_vga_entry(unsigned char uc,
	uint8_t color);

/**
 * @brief Initialises VGA for the system.
 * Initialises the VGA buffer.
 */
void vga_initialize(void);

/**
 * @brief Sets the VGA color.
 * Sets the VGA terminal color.
 * @param color    The encoded VGA color code to set the terminalk to.
 */
void vga_set_color(uint8_t color);

/**
 * @brief Prints a char to the screen.
 * Prints a char to the VGA buffer.
 * @param c The char to print to the VGA buffer.
 */
void vga_putchar(char c);

/**
 * @brief Writes a string to screen.
 * Writes a string to the VGA buffer.
 * @param str The string to write to the buffer.
 */
void vga_puts(const char* str);

#endif
