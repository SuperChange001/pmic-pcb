
#include <pico/stdlib.h>
#include "usb_dual_cdc.h"


int main() 
{
    uint8_t buf[64]; // temporary buffer for cdc read/write

    cdc_init();

    while (1) 
    {
        cdc_task();

        // When received data from cdc0, echo back
        if (cdc_available_bytes(0)){

            uint32_t len = cdc_read_buf(0, buf, 64);
            cdc_write_buf(0, "cdc0 receive: ", 13); // indicating which cdc port received data
            cdc_write_buf(0, buf, len);
            cdc_write_buf(0, "\r\n", 2);
        }

        // When received data from cdc1, echo back
        if((cdc_available_bytes(1)))
        {
            uint32_t len = cdc_read_buf(1, buf, 64);
            // 
            cdc_write_buf(1, "cdc1 receive: ", 13); // indicating which cdc port received data
            cdc_write_buf(1, buf, len);
            cdc_write_buf(1, "\r\n", 2);
        }
    }
}