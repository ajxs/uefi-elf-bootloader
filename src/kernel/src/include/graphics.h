/**
 * @file graphics.h
 * @author ajxs
 * @date Aug 2022
 * @brief Functionality for working with the framebuffer.
 * Contains functionality for working with the framebuffer.
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H 1

#include <stdint.h>

/**
 * @brief Draws a rectangle onto the framebuffer.
 * Draws a rectangle onto the video frame buffer.
 * @param[in] framebuffer_pointer A pointer to the video framebuffer.
 * @param[in] pixels_per_scaline The number of pixels per scanline. Also known as 'pitch'.
 * In some more exotic video modes this may be different to the visible screen width.
 * @param[in] _x The x coordinate to draw the rect to.
 * @param[in] _y The y coordinate to draw the rect to.
 * @param[in] width The width of the rectangle to draw.
 * @param[in] height The height of the rectangle to draw.
 * @param[in] color The color to draw.
 */
void draw_rect(uint32_t* framebuffer_pointer,
	const uint32_t pixels_per_scaline,
	const uint16_t _x,
	const uint16_t _y,
	const uint16_t width,
	const uint16_t height,
	const uint32_t color);

#endif
