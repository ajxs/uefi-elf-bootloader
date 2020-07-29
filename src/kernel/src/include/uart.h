/**
 * @file uart.h
 * @author ajxs
 * @date Aug 2019
 * @brief UART functionality.
 * Contains definitions for serial IO.
 */

#include <stdbool.h>

/**
 * @brief Initialises the UART.
 * Initialises the system UART for output.
 */
void uart_initialize(void);

/**
 * @brief Checks whether the UART receive buffer is empty.
 * This function checks whether the UART receive buffer is empty.
 * @return A boolean indicating whether the recieve buffer is empty.
 */
bool uart_is_recieve_buffer_empty(void);

/**
 * @brief UART getchar.
 * Gets a character from the UART input buffer.
 * @return The read character.
 */
char uart_getchar(void);

/**
 * @brief Checks whether the UART transmit buffer is empty.
 * This function checks whether the UART transmit buffer is empty.
 * @return A boolean indicating whether the transmit buffer is empty. 
 */
bool uart_is_transmit_buffer_empty(void);

/**
 * @brief UART putchar.
 * Writes a character to the UART.
 * @param a    The char to write.
 */
void uart_putchar(char a);

/**
 * @brief UART puts.
 * Writes a string to the UART.
 * @param str    The string to write to the UART.
 */
void uart_puts(const char* str);
