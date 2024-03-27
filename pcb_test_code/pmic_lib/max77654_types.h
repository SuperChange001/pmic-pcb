#ifndef __MAX__77654__TYPES__H__

#define __MAX__77654__TYPES__H__


typedef struct {
    unsigned int target_voltage : 6;  // Bits 6:0, 0x00 = 0.8V, 0x3F = 5.5V
    unsigned int reserved : 1;  // Bits 7 reserved for future use
}reg_cnfg_ssbx_a_t;

typedef struct {
    unsigned int enable_control : 3;  // Bits 2:0; 0x04 = OFF, 0x07 = ON
    unsigned int active_discharge : 1;  // Bit 3; 0x00 = Disable, 0x01 = Enable
    unsigned int peak_current_limit : 2; // Bits 5:4; 0x00 = 1.0A, 0x01 = 0.75A, 0x02 = 0.5A, 0x03 = 0.33A
    unsigned int operation_mode : 1;  // Bit 6; 0x00 = Buck-boost, 0x01 = Buck
    unsigned int reserved : 1;  // Bits 7; reserved for future use
} reg_cnfg_ssbx_b_t;

typedef struct {
    reg_cnfg_ssbx_a_t reg_a;
    reg_cnfg_ssbx_b_t reg_b;
} reg_cnfg_ssbx_t;



typedef struct {
    unsigned int target_voltage : 6;  // Bits 6:0, 0x00 = 0.8V, 0x3F = 3.975V
    unsigned int reserved : 1;  // Bits 7 reserved for future use
} reg_cnfg_ldox_a_t;

typedef struct {
    unsigned int enable_control : 3;  // Bits 2:0; 0x04 = OFF, 0x07 = ON
    unsigned int active_discharge : 1;  // Bit 3; 0x00 = Disable, 0x01 = Enable
    unsigned int operation_mode : 1;  // Bit 4; 0x00 = LDO, 0x01 = LSW
    unsigned int reserved : 3;  // Bits 7:5, reserved for future use
} reg_cnfg_ldox_b_t;



typedef struct{
    reg_cnfg_ldox_a_t reg_a;
    reg_cnfg_ldox_b_t reg_b;
} reg_cnfg_ldox_t;

typedef struct {
    reg_cnfg_ssbx_t ssbs[3];  // SSBS0, SSBS1, SSBS2
    reg_cnfg_ldox_t ldos[2];  // LDO0, LDO1
} reg_map_max77654_t;



#endif