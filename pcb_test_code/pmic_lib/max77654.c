#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "max77654.h"
#include "max77654_types.h"
#include <stdio.h>

#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)
#define PRINT(fmt,...) {printf("[%s:%d <%s>]",__FILENAME__,__LINE__, __FUNCTION__); printf(fmt, ##__VA_ARGS__);printf("\n");}


#define MAX77654_SLAVE_ADDR 0x48
#define REG_ADDR_ERCFLAG 0x05
#define REG_ADDR_CNFG_SSBx_A 0x29 //0x29,0x2A for SSB0, 
#define REG_ADDR_CNFG_SSBx_B 0x2A //0x2B,0x2C for SSB1, 
                                  //0x2D,0x2E for SSB2

#define REG_ADDR_CNFG_LDOx_A 0x38 //0x38,0x39 for LDO0, 
#define REG_ADDR_CNFG_LDOx_B 0x39 //0x3A,0x3B for LDO1

#define OFF_IRRESPECTIVE_OF_FPS 0x04
#define ON_IRRESPECTIVE_OF_FPS 0x07


static i2c_inst_t *I2C_CH_MAX77654;
static reg_map_max77654_t reg_map_max77654;


typedef enum {
    TOVLD = 1 << 0,  // Thermal Overload
    SYSOVLO = 1 << 1,  // overvoltage-lockout
    SYSUVLO = 1 << 2,  // undervoltage-lockout
    // Add more error codes as needed
} ercflag_type;

uint8_t calculate_ssb_voltage_reg(int16_t voltage_in_mV)
{
    if(voltage_in_mV < 800)
        voltage_in_mV = 800;
    else if (voltage_in_mV > 5500)
        voltage_in_mV = 5500;

    int reg = 0;
    reg = (voltage_in_mV - 800) / 50;
    return reg;
}

uint8_t calculate_ldo_voltage_reg(int16_t voltage_in_mV)
{
    if(voltage_in_mV < 800)
        voltage_in_mV = 800;
    else if (voltage_in_mV > 3975)
        voltage_in_mV = 3975;

    int reg = 0;
    reg = (voltage_in_mV - 800) / 25;
    return reg;
}

void default_configure_max77654(void)
{
    reg_map_max77654.ssbs[0].reg_a.target_voltage = calculate_ssb_voltage_reg(3100);
    reg_map_max77654.ssbs[0].reg_b.enable_control = ON_IRRESPECTIVE_OF_FPS; // 0x00 = ON
    reg_map_max77654.ssbs[0].reg_b.active_discharge = 0x00; // 0x00 = Disable
    reg_map_max77654.ssbs[0].reg_b.peak_current_limit = 0x03; // 330mA
    reg_map_max77654.ssbs[0].reg_b.operation_mode = 0x00; // 0x00 = Buck-boost

    reg_map_max77654.ssbs[1].reg_a.target_voltage = calculate_ssb_voltage_reg(3200);
    reg_map_max77654.ssbs[1].reg_b.enable_control = ON_IRRESPECTIVE_OF_FPS; // 0x00 = ON
    reg_map_max77654.ssbs[1].reg_b.active_discharge = 0x00; // 0x00 = Disable
    reg_map_max77654.ssbs[1].reg_b.peak_current_limit = 0x03; // 330mA
    reg_map_max77654.ssbs[1].reg_b.operation_mode = 0x00; // 0x00 = Buck-boost

    reg_map_max77654.ssbs[2].reg_a.target_voltage = calculate_ssb_voltage_reg(3300);
    reg_map_max77654.ssbs[2].reg_b.enable_control = ON_IRRESPECTIVE_OF_FPS; // 0x00 = ON
    reg_map_max77654.ssbs[2].reg_b.active_discharge = 0x00; // 0x00 = Disable
    reg_map_max77654.ssbs[2].reg_b.peak_current_limit = 0x03; // 330mA
    reg_map_max77654.ssbs[2].reg_b.operation_mode = 0x00; // 0x00 = Buck-boost
   

    reg_map_max77654.ldos[0].reg_a.target_voltage = calculate_ldo_voltage_reg(1100); // set to 1100mV
    reg_map_max77654.ldos[0].reg_b.enable_control = ON_IRRESPECTIVE_OF_FPS; // 0x00 = ON
    reg_map_max77654.ldos[0].reg_b.active_discharge = 0x00; // 0x00 = Disable
    reg_map_max77654.ldos[0].reg_b.operation_mode = 0x00; // 0x00 = LDO

    reg_map_max77654.ldos[1].reg_a.target_voltage = calculate_ldo_voltage_reg(900); // set to 900mV
    reg_map_max77654.ldos[1].reg_b.enable_control = ON_IRRESPECTIVE_OF_FPS; // 0x00 = ON
    reg_map_max77654.ldos[1].reg_b.active_discharge = 0x00; // 0x00 = Disable
    reg_map_max77654.ldos[1].reg_b.operation_mode = 0x00; // 0x00 = LDO
}


int max77654_on_bus(void)
{
    int ret;
    uint8_t readbuffer[1];
    ret = i2c_read_blocking(I2C_CH_MAX77654, MAX77654_SLAVE_ADDR, readbuffer, 1, false);
    return ret;
}


int max77654_init(i2c_inst_t *i2c) 
{
    int ret;
    
    default_configure_max77654(); // does nothing but change the reg_map_max77654 on MCU

    I2C_CH_MAX77654 = i2c;

    i2c_init(i2c, 100 * 1000);
    gpio_set_function(26, GPIO_FUNC_I2C);
    gpio_set_function(27, GPIO_FUNC_I2C);
    gpio_pull_up(26);
    gpio_pull_up(27);

    ret = max77654_on_bus();
    if (ret < 0) 
    {
        return -1;
    }
    // PRINT("MAX77654 is on the bus\n");
    
    uint8_t cmd = REG_ADDR_ERCFLAG;
    i2c_write_blocking(I2C_CH_MAX77654, MAX77654_SLAVE_ADDR, &cmd, 1, false);
    
    ercflag_type erc;
    ret = i2c_read_blocking(I2C_CH_MAX77654, MAX77654_SLAVE_ADDR, &erc, 1, false);

    if (ret < 0) 
    {
        return -1;
    }
    else
    {   
        // PRINT("ERCFLAG: 0x%02x\n", erc);
        if (erc & TOVLD || erc & SYSOVLO || erc & SYSUVLO)
        {
             return -1;
        }
    }

    SSBx_set_voltage(0, 3100);
    SSBx_set_voltage(1, 3200);
    SSBx_set_voltage(2, 3300);
    SSBx_enable(0, true);
    SSBx_enable(1, true);
    SSBx_enable(2, true);

    LDOx_set_voltage(0, 1200);
    LDOx_set_voltage(1, 900);
    LDOx_enable(0, true);
    LDOx_enable(1, true);

    return 0;
}



int SSBx_enable(int ch, bool enable)
{
    uint8_t cmd[2];

    // first update the reg_map on MCU
    reg_map_max77654.ssbs[ch].reg_b.enable_control = enable ? ON_IRRESPECTIVE_OF_FPS : OFF_IRRESPECTIVE_OF_FPS;

    cmd[0] = REG_ADDR_CNFG_SSBx_B + ch * 2;
    cmd[1] = reg_map_max77654.ssbs[ch].reg_b.enable_control | (reg_map_max77654.ssbs[ch].reg_b.active_discharge<<3) | (reg_map_max77654.ssbs[ch].reg_b.peak_current_limit<<4) | (reg_map_max77654.ssbs[ch].reg_b.operation_mode<<5);
    i2c_write_blocking(I2C_CH_MAX77654, MAX77654_SLAVE_ADDR, cmd, 2, false);

    PRINT("SSB%d %s", ch, enable ? "enabled" : "disabled\n");
    PRINT("%02x %02x\n", cmd[0], cmd[1]);
    return 0;
}

int SSBx_set_voltage(int ch, int16_t voltage_in_mV)
{
    uint8_t cmd[2];

    // first update the reg_map on MCU
    reg_map_max77654.ssbs[ch].reg_a.target_voltage = calculate_ssb_voltage_reg(voltage_in_mV);

    cmd[0] = REG_ADDR_CNFG_SSBx_A + ch * 2;
    cmd[1] = reg_map_max77654.ssbs[ch].reg_a.target_voltage;
    i2c_write_blocking(I2C_CH_MAX77654, MAX77654_SLAVE_ADDR, cmd, 2, false);
    PRINT("SSB%d voltage set to %d mV", ch, voltage_in_mV);
    PRINT("%02x %02x\n", cmd[0], cmd[1]);
    return 0;
}

int LDOx_set_mode(int ch, int mode)
{
    uint8_t cmd[2];
    
    reg_map_max77654.ldos[ch].reg_b.operation_mode = mode;
    
    cmd[0] = REG_ADDR_CNFG_LDOx_B + ch * 2;
    cmd[1] = reg_map_max77654.ldos[ch].reg_b.operation_mode|(reg_map_max77654.ldos[ch].reg_b.active_discharge<<3)|(reg_map_max77654.ldos[ch].reg_b.operation_mode<<4);
    i2c_write_blocking(I2C_CH_MAX77654, MAX77654_SLAVE_ADDR, cmd, 2, false);
    // PRINT("SSB%d %s", ch, enable ? "enabled" : "disabled\n");
    // PRINT("%02x %02x\n", cmd[0], cmd[1]);
    return 0;
}

int LDOx_enable_active_discharge(int ch, bool enable)
{
    uint8_t cmd[2];

    reg_map_max77654.ldos[ch].reg_b.active_discharge = enable ? ON_IRRESPECTIVE_OF_FPS : OFF_IRRESPECTIVE_OF_FPS;

    cmd[0] = REG_ADDR_CNFG_LDOx_B + ch * 2;
    cmd[1] = reg_map_max77654.ldos[ch].reg_b.operation_mode|(reg_map_max77654.ldos[ch].reg_b.active_discharge<<3)|(reg_map_max77654.ldos[ch].reg_b.operation_mode<<4);
    
    i2c_write_blocking(I2C_CH_MAX77654, MAX77654_SLAVE_ADDR, cmd, 2, false);
    // PRINT("SSB%d %s", ch, enable ? "enabled" : "disabled\n");
    // PRINT("%02x %02x\n", cmd[0], cmd[1]);
    return 0;
}

int LDOx_enable(int ch, bool enable)
{
    uint8_t cmd[2];

    reg_map_max77654.ldos[ch].reg_b.enable_control = enable ? ON_IRRESPECTIVE_OF_FPS : OFF_IRRESPECTIVE_OF_FPS;

    cmd[0] = REG_ADDR_CNFG_LDOx_B + ch * 2;
    cmd[1] = reg_map_max77654.ldos[ch].reg_b.operation_mode|(reg_map_max77654.ldos[ch].reg_b.active_discharge<<3)|(reg_map_max77654.ldos[ch].reg_b.operation_mode<<4);
    
    i2c_write_blocking(I2C_CH_MAX77654, MAX77654_SLAVE_ADDR, cmd, 2, false);
    // PRINT("SSB%d %s", ch, enable ? "enabled" : "disabled\n");
    // PRINT("%02x %02x\n", cmd[0], cmd[1]);
    return 0;
}

int LDOx_set_voltage(int ch, int16_t voltage_in_mV)
{
    uint8_t cmd[2];

    // first update the reg_map on MCU
    reg_map_max77654.ldos[ch].reg_a.target_voltage = calculate_ldo_voltage_reg(voltage_in_mV);

    cmd[0] = REG_ADDR_CNFG_LDOx_A + ch * 2;
    cmd[1] = reg_map_max77654.ldos[ch].reg_a.target_voltage;
    i2c_write_blocking(I2C_CH_MAX77654, MAX77654_SLAVE_ADDR, cmd, 2, false);
    PRINT("LDO%d voltage set to %d mV", ch, voltage_in_mV);
    PRINT("%02x %02x\n", cmd[0], cmd[1]);
    return 0;
}