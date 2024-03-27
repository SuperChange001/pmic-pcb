
#include <pico/stdlib.h>
#include "usb_dual_cdc.h"
#include "usb_stdio_cdc.h"

#define LED0_PIN 22
#define LED1_PIN 24
#define LED2_PIN 25

// Helper functions for debug
void leds_ctrl(uint8_t led, uint8_t on);
void leds_init(void);
void debug_interface(void);

#define CDC_APP_ITF 1 // take 1 since 0 is used for stdio

int main() 
{
    uint8_t buf[64]; // temporary buffer for cdc read/write
    
    leds_init();

    cdc_init();
    
    usb_stdio_cdc_init();
    
    // By now we have a stdio driver that uses cdc0
    // and we can use printf() to send data to host via cdc0

    // and cdc1 is for communication with host

    while (1) 
    {
        cdc_task(); // ! Always call cdc_task() in main loop

        // Example Application, echo back data from cdc1 to cdc1
        // When received data from cdc1, echo back
        if((cdc_available_bytes(1)))
        {
            // try to read 64 bytes from cdc1
            // len indicates how many bytes are read actually
            uint32_t len = cdc_read_buf(CDC_APP_ITF, buf, 64);

            // echo back to cdc1
            cdc_write_buf(CDC_APP_ITF, buf, len);

            // log cdc1 activity to stdio
            printf("CDC1 receive %d Bytes data: ",len);
            for(int i=0;i<len;i++)
                printf("%02x ",buf[i]);
            printf("\r\n");
        }

        debug_interface();
    }
}

void leds_init(void)
{
    gpio_init(LED0_PIN);
    gpio_set_dir(LED0_PIN, GPIO_OUT);
    gpio_put(LED0_PIN, 0);

    gpio_init(LED1_PIN);
    gpio_set_dir(LED1_PIN, GPIO_OUT);
    gpio_put(LED1_PIN, 0);

    gpio_init(LED2_PIN);
    gpio_set_dir(LED2_PIN, GPIO_OUT);
    gpio_put(LED2_PIN, 0);
}

void leds_ctrl(uint8_t led, uint8_t on)
{
    switch(led)
    {
        case 0:
            gpio_put(LED0_PIN, on);
            break;
        case 1:
            gpio_put(LED1_PIN, on);
            break;
        case 2:
            gpio_put(LED2_PIN, on);
            break;
        default:
            break;
    }
}

void debug_interface(void)
{
    uint8_t c=getchar_timeout_us(1);

    if(c!=0xff)
    {
        switch(c)
        {
            case 'L':
                leds_ctrl(0,1);
                leds_ctrl(1,1);
                leds_ctrl(2,1);
                printf("Turn on all leds\r\n");
                break;
            case 'l':
                leds_ctrl(0,0);
                leds_ctrl(1,0);
                leds_ctrl(2,0);
                printf("Turn off all leds\r\n");
                break;
            default:
                printf("This is the interface for debug\r\n");
                printf("1. You can type 'L' to turn on all leds\r\n");
                printf("2. You can type 'l' to turn off all leds\r\n");
                printf("3. =====================================\r\n");
                break;
        }
    }
}