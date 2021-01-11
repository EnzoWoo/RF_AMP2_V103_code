/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/
#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "ktype.h"


void eeprom_write_buf(I2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t *buf, uint16_t size);
void eeprom_read_buf(I2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t *buf, uint16_t size);


#endif
