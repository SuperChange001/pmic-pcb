/**
 * @file usb_stdio_cdc.c
 * @brief This file contains the definitions of functions for handling standard input/output (stdio) via a CDC interface.
 *
 * The functions defined in this file allow for writing to and reading from the CDC interface, flushing the output buffer of the CDC interface,
 * and setting a callback function for when characters are available. It also defines the stdio driver for the CDC interface and a function for initializing this driver.
 *
 * This file is part of the usb_dual_cdc_lib.
 */

#include "usb_stdio_cdc.h"
#include "usb_dual_cdc.h"
#include "pico/stdio/driver.h"

// Function to write characters to the CDC interface
void cdc_out_func(const char *buf, int len) 
{
    // Write the buffer to the CDC interface
    cdc_write_buf(CDC_STDIO_ITF, (uint8_t *)buf, len);
}

// Function to flush the output buffer of the CDC interface
void cdc_out_flush(void) {
    // Currently, this function does nothing
    return;
}

// Function to read characters from the CDC interface
int cdc_in_chars(char *buf, int len) {
    // Read the buffer from the CDC interface
    return cdc_read_buf(CDC_STDIO_ITF, buf, len);
}

// Function to set a callback function for when characters are available
void cdc_set_chars_available_callback(void (*fn)(void*), void *param) {
    // Currently, this function does nothing
    return;
}

// Define the stdio driver for the CDC interface
stdio_driver_t usb_stdio_cdc = {
    .out_chars = cdc_out_func,
    .out_flush = cdc_out_flush,
    .in_chars = cdc_in_chars,
    .set_chars_available_callback=cdc_set_chars_available_callback,
    (stdio_driver_t *) 0,
    false,
    false,
};

// Function to initialize the CDC interface for stdio
void usb_stdio_cdc_init(void) {
    // Enable the stdio driver for the CDC interface
    stdio_set_driver_enabled(&usb_stdio_cdc, true);
    // Filter the stdio driver for the CDC interface
    stdio_filter_driver(&usb_stdio_cdc);
}