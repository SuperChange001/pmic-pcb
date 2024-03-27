#ifndef __MAX__77654__H__

#define __MAX__77654__H__

// Types needs to be defined in the header file
#define LDO_MODE_LDO 0x00
#define LDO_MODE_LSW 0x01


int max77654_init(i2c_inst_t *i2c);
int SSBx_enable(int ch, bool enable);
int SSBx_set_voltage(int ch, int16_t voltage_in_mV);

// ========LDO========
int LDOx_set_mode(int ch, int mode); //mode: LDO_MODE_LDO or LDO_MODE_LSW
int LDOx_enable_active_discharge(int ch, bool enable);
int LDOx_enable(int ch, bool enable);
int LDOx_set_voltage(int ch, int16_t voltage_in_mV);
#endif
