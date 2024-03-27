/**
 * @file usb_dual_cdc.c
 * @brief This file contains the definitions of functions for handling USB communication via CDC interfaces.
 *
 * The functions defined in this file allow for initializing the CDC interfaces, handling the tasks related to the CDC interfaces,
 * writing to the CDC interfaces' buffers, checking the number of bytes available in the CDC interfaces' buffers, and reading from the CDC interfaces' buffers.
 * It also contains private functions for reading and writing bytes to a USB interface.
 *
 * This file is part of the usb_dual_cdc_lib.
 */

#include <pico/stdlib.h>
#include <string.h>
#include <tusb.h>
#include "usb_dual_cdc.h"

#define BUFFER_SIZE 1024 // it will be occupied 4 times for two cdc interfaces

typedef struct {
	uint8_t recv_buffer[BUFFER_SIZE];
	uint32_t recv_pos;

    uint8_t write_buffer[BUFFER_SIZE];
    uint32_t write_pos;
} cdc_data_t;

cdc_data_t CDC_DATA[2];

// Private functions
#if !defined(MIN)
#define MIN(a, b) ((a > b) ? b : a)
#endif /* MIN */

static void usb_write_bytes(uint8_t itf);
static void usb_read_bytes(uint8_t itf);


/**
 * @brief Initializes the CDC (Communication Device Class) interfaces.
 *
 * This function initializes the CDC interfaces for USB communication.
 * It first initializes the USB device ID to ensure it is unique.
 * Then, it initializes the TinyUSB stack.
 * Finally, it iterates over all CDC interfaces defined in the configuration,
 * and resets the read and write positions in their respective buffers to 0.
 */
void cdc_init()
{
    usbd_id_init(); // for unique device ID

    tusb_init();

    for(uint8_t itf=0; itf<CFG_TUD_CDC; itf++)
    {
        CDC_DATA[itf].recv_pos = 0;
        CDC_DATA[itf].write_pos = 0;
    }
}

/**
 * @brief Handles the CDC (Communication Device Class) tasks.
 *
 * This function handles the tasks related to the CDC interfaces for USB communication.
 * It must be called periodically to keep the USB connection alive.
 * It first calls the TinyUSB task function, which handles the low-level USB tasks.
 * Then, it iterates over all CDC interfaces defined in the configuration.
 * For each connected interface, it reads any available bytes from the interface and writes any bytes in the buffer to the interface.
 */
void cdc_task()
{   
    // Must be periodically called to keep USB alive
    tud_task();

    for (uint8_t itf = 0; itf < CFG_TUD_CDC; itf++) {
        if (tud_cdc_n_connected(itf)) {
            usb_read_bytes(itf);
            usb_write_bytes(itf);
        }
    }
}


/**
 * @brief Returns the number of bytes available to read from a CDC interface's buffer.
 *
 * This function checks the number of bytes that have been read from a specified CDC interface and are available in the buffer.
 * It returns the current position in the buffer, which is the number of bytes available to read.
 *
 * @param itf The CDC interface to check.
 * @return The number of bytes available to read from the buffer.
 */
uint32_t cdc_available_bytes(uint8_t itf)
{
    cdc_data_t *cd = &CDC_DATA[itf];
    return cd->recv_pos;
}


/**
 * @brief Reads a specified number of bytes from a CDC interface's buffer.
 *
 * This function reads a specified number of bytes from a specified CDC interface's buffer into a provided data array.
 * If the interface is connected, it checks if the number of bytes available in the buffer is less than the requested number.
 * If so, it reads all available bytes and resets the buffer's position.
 * If not, it reads the requested number of bytes and moves the remaining bytes to the beginning of the buffer, updating the buffer's position accordingly.
 * If the interface is not connected, it does not read any bytes.
 *
 * @param itf The CDC interface to read from.
 * @param data The array to store the read bytes.
 * @param len The number of bytes to read.
 * @return The number of bytes actually read.
 */
uint32_t cdc_read_buf(uint8_t itf, uint8_t *data, uint32_t len)
{   
    uint32_t read_n_bytes = len;

    if (tud_cdc_n_connected(itf)) 
    {
        cdc_data_t *cd = &CDC_DATA[itf];

        if (cd->recv_pos < len)
        {
            read_n_bytes = cd->recv_pos;
            memcpy(data, cd->recv_buffer, read_n_bytes);
            cd->recv_pos = 0;
        }
        else 
        {
            read_n_bytes = len;
            memcpy(data, cd->recv_buffer, read_n_bytes);
            memmove(cd->recv_buffer, &cd->recv_buffer[len], cd->recv_pos - len);
            cd->recv_pos -= len;
        }
    }
    else
    {
        read_n_bytes = 0;
    }
    return read_n_bytes;
}


/**
 * @brief Writes a specified number of bytes to a CDC interface's buffer.
 *
 * This function writes a specified number of bytes from a provided data array to a specified CDC interface's buffer.
 * If the interface is connected, it checks if the buffer has enough space for the new data.
 * If not, it does not write any data.
 * If there is enough space, it copies the data to the buffer at the current position and updates the position accordingly.
 *
 * @param itf The CDC interface to write to.
 * @param data The array of bytes to write.
 * @param len The number of bytes to write.
 */
void cdc_write_buf(uint8_t itf, uint8_t *data, uint32_t len)
{
    if (tud_cdc_n_connected(itf)) 
    {
        cdc_data_t *cd = &CDC_DATA[itf];

        if (cd->write_pos + len > BUFFER_SIZE)
            return;

        memcpy(&cd->write_buffer[cd->write_pos], data, len);
        cd->write_pos += len;
    }
}


// ================================================================================
// Private functions
// ================================================================================

/**
 * @brief Reads bytes from a USB interface and stores them in a buffer.
 *
 * This function reads available bytes from a specified USB interface and stores them in a buffer.
 * The function first checks how many bytes are available on the interface. If there are bytes available,
 * it calculates the minimum between the available bytes and the remaining space in the buffer.
 * Then, it reads the calculated number of bytes from the interface and stores them in the buffer,
 * updating the buffer's current position accordingly.
 *
 * @param itf The USB interface from which to read bytes.
 */
static void usb_read_bytes(uint8_t itf)
{
    cdc_data_t *cd = &CDC_DATA[itf];

    uint32_t len = tud_cdc_n_available(itf);

    if (len)
    {
        len = MIN(len, BUFFER_SIZE - cd->recv_pos);
        if (len) 
        {
            uint32_t count;
            count = tud_cdc_n_read(itf, &cd->recv_buffer[cd->recv_pos], len);
            cd->recv_pos += count;
        }
    }
}


/**
 * @brief Writes bytes to a USB interface from a buffer.
 *
 * This function writes bytes to a specified USB interface from a buffer.
 * If there are bytes in the buffer, it writes as many bytes as possible to the interface.
 * If not all bytes could be written, it moves the remaining bytes to the beginning of the buffer.
 * After writing, it updates the buffer's current position accordingly.
 * Finally, it flushes the write buffer to ensure all data is sent.
 *
 * @param itf The USB interface to which to write bytes.
 */
static void usb_write_bytes(uint8_t itf)
{
    cdc_data_t *cd = &CDC_DATA[itf];
    
    if (cd->write_pos>0)
    {
        uint32_t count;
        count = tud_cdc_n_write(itf, cd->write_buffer, cd->write_pos);
        if (count < cd->write_pos)
            memmove(cd->write_buffer, &cd->write_buffer[count], cd->write_pos - count);
        cd->write_pos -= count;
    }

    tud_cdc_n_write_flush(itf);
}
