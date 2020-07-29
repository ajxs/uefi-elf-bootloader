/**
 * @file string.c
 * @author ajxs
 * @date Aug 2019
 * @brief String functionality.
 * Contains implementation for string functions.
 */

#include <string.h>

/**
 * strlen
 */
size_t strlen(const char* str)
{
	/** The length of the string to be output. */
	size_t len = 0;
	while(str[len]) {
		len++;
	}

	return len;
}
