/**
 * @file usb_stdio_cdc.h
 * @brief This file contains the declaration of the function for initializing a CDC interface for standard input/output (stdio).
 *
 * The CDC_STDIO_ITF macro defines which CDC interface to use for stdio. It can be 0 or 1.
 * The usb_stdio_cdc_init function is declared, which is used to initialize this interface.
 *
 * This file is part of the usb_dual_cdc_lib.
 */

#ifndef __USB_STDIO_CDC_H__
#define __USB_STDIO_CDC_H__ 1

#define CDC_STDIO_ITF 0 // which CDC interface to use for stdio, can be 0 or 1

void usb_stdio_cdc_init(void);

#endif /* __USB_STDIO_CDC_H__ */
