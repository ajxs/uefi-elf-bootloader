#include <stdint.h>
#include <graphics.h>

/**
 * draw_rect
 */
void draw_rect(uint32_t* framebuffer_pointer,
	const uint32_t pixels_per_scaline,
	const uint16_t _x,
	const uint16_t _y,
	const uint16_t width,
	const uint16_t height,
	const uint32_t color)
{
	/** Pointer to the current pixel in the buffer. */
	uint32_t* at;

	uint16_t row = 0;
	uint16_t col = 0;
	for(row = 0; row < height; row++) {
		for(col = 0; col < width; col++) {
			at = framebuffer_pointer + _x + col;
			at += ((_y + row) * pixels_per_scaline);

			*at = color;
		}
	}
}
