/**
 * @file usb_dual_cdc.h
 * @brief This file contains the declarations of functions for handling USB communication via CDC interfaces.
 *
 * The functions declared in this file allow for initializing the CDC interfaces, handling the tasks related to the CDC interfaces,
 * writing to the CDC interfaces' buffers, checking the number of bytes available in the CDC interfaces' buffers, and reading from the CDC interfaces' buffers.
 *
 * This file is part of the usb_dual_cdc_lib.
 */

#include <tusb.h>

#ifndef USB_DUAL_CDC_H
#define USB_DUAL_CDC_H

/**
 * @brief Initializes the CDC interfaces for USB communication.
 */
void cdc_init();

/**
 * @brief Handles the tasks related to the CDC interfaces for USB communication.
 */
void cdc_task();

/**
 * @brief Writes a specified number of bytes from a provided data array to a specified CDC interface's buffer.
 * @param itf The CDC interface to write to.
 * @param data The array of bytes to write.
 * @param len The number of bytes to write.
 */
void cdc_write_buf(uint8_t itf, uint8_t *data, uint32_t len);

/**
 * @brief Checks the number of bytes that have been read from a specified CDC interface and are available in the buffer.
 * @param itf The CDC interface to check.
 * @return The number of bytes available to read from the buffer.
 */
uint32_t cdc_available_bytes(uint8_t itf);

/**
 * @brief Reads a specified number of bytes from a specified CDC interface's buffer into a provided data array.
 * @param itf The CDC interface to read from.
 * @param data The array to store the read bytes.
 * @param len The number of bytes to read.
 * @return The number of bytes actually read.
 */
uint32_t cdc_read_buf(uint8_t itf, uint8_t *data, uint32_t len);

#endif