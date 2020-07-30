/**
 * @file port_io.h
 * @author ajxs
 * @date Aug 2019
 * @brief Port IO functionality.
 * Contains definitions for port IO functions.
 */

#ifndef PORT_IO_H
#define PORT_IO_H

#include <stdint.h>

/**
 * @brief Reads a byte from an IO port.
 * Reads a byte from a specified IO port.
 * @param port    The port to read the byte from.
 * @return        The byte read from the port.
 */
uint8_t inb(uint16_t port);

/**
 * @brief Writes a byte to an IO port.
 * Writes a byte to a specified IO port.
 * @param port    The port to write the byte to.
 * @param val     The byte to write.
 */
void outb(uint16_t port, uint8_t val);

#endif
