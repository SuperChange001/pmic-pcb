/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// ================
// Wiring, using I2C1
// ================
// PICO     PMIC
// 26    ->  SDA 
// 27    ->  SCL
// 
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "max77654.h"

int main() {
    // Enable UART so we can print status output
    stdio_init_all();

    // This example will use I2C0 on the default SDA and SCL pins (0, 1 on a Pico)
    // max77654_init(i2c1);

    while (1)
    {
        char c = getchar_timeout_us(10000);
        if (c == 't')
        {
            printf("\nStart test the MAX77654\n");
            int ret = max77654_init(i2c1);
            if (ret < 0)
            {
                printf("MAX77654 is not on the bus\n");
            }
            else
            {
                printf("MAX77654 is on the bus\n");
            }
            printf("Done.\n");
        }
        else if (c == '1')
        {
            printf("Enable all SSBs\n");
            SSBx_enable(1, true);
            SSBx_enable(0, true);
            SSBx_enable(2, true);
        }
        else if (c == '2')
        {
            printf("Disable all SSBs\n");
            SSBx_enable(1, false);
            SSBx_enable(0, false);
            SSBx_enable(2, false);
        }
        else if(c == '3')
        {
            printf("Set SSB1 to 3.3V, other SSB to 1.0V\n");
            SSBx_set_voltage(1, 3300);
            SSBx_set_voltage(0, 1000);
            SSBx_set_voltage(2, 1000);
        }
        else if(c == '4')
        {
            printf("Set SSB1 to 1.0V, other SSB to 3.3V\n");
            SSBx_set_voltage(1, 1000);
            SSBx_set_voltage(0, 3300);
            SSBx_set_voltage(2, 3300);
        }
        
    }
}