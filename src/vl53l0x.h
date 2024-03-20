#ifndef VL53L0X_H
#define VL53L0X_H

#include <Wire.h>
#define VL53L0X_DelaymS 200
#define VL53L0X_REG_IDENTIFICATION_MODEL_ID 0xc0
#define VL53L0X_REG_IDENTIFICATION_REVISION_ID 0xc2
#define VL53L0X_REG_PRE_RANGE_CONFIG_VCSEL_PERIOD 0x50
#define VL53L0X_REG_FINAL_RANGE_CONFIG_VCSEL_PERIOD 0x70
#define VL53L0X_REG_SYSRANGE_START 0x00
#define VL53L0X_REG_RESULT_INTERRUPT_STATUS 0x13
#define VL53L0X_REG_RESULT_RANGE_STATUS 0x14
#define ADDRESS 0x29

extern byte gbuf[16]; // Global buffer used for read operations

void VL53L0X_test(void);
uint16_t VL53L0X_bswap(byte b[]);
uint16_t VL53L0X_makeuint16(int lsb, int msb);
void VL53L0X_write_byte_data(byte data);
void VL53L0X_write_byte_data_at(byte reg, byte data);
void VL53L0X_write_word_data_at(byte reg, uint16_t data);
byte VL53L0X_read_byte_data(void);
byte VL53L0X_read_byte_data_at(byte reg);
uint16_t VL53L0X_read_word_data_at(byte reg);
void VL53L0X_read_block_data_at(byte reg, int sz);
uint16_t VL53L0X_decode_vcsel_period(short vcsel_period_reg);
uint16_t VL53L0X_getDistance(void);
#endif // VL53L0X_H
