/**
 * @file string.h
 * @author ajxs
 * @date Aug 2019
 * @brief String functionality.
 * Contains definitions for string functions.
 */

#include <stddef.h>

/**
 * @brief Gets the size of a string.
 * Returns the number of chars making up a constant string.
 * @param str   The string to get the length of.
 * @return      The size of the string.
 */
size_t strlen(const char* str);
