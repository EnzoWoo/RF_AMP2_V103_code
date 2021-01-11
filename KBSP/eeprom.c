/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/
#include "bsp.h"
#include "eeprom.h"


void eeprom_write_buf(I2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t *buf, uint16_t size)
{
	if(HAL_I2C_Mem_Write_IT(hi2c,addr,0,I2C_MEMADD_SIZE_8BIT,buf,size)!= HAL_OK){	}
  while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY){ } 
  while (HAL_I2C_IsDeviceReady(hi2c,addr,10,300) == HAL_TIMEOUT){	}
  while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY){ } 
}

void eeprom_read_buf(I2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t *buf, uint16_t size)
{
	if(HAL_I2C_Mem_Read_IT(hi2c,addr,0,I2C_MEMADD_SIZE_8BIT,buf,size)!= HAL_OK){ }
  while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY){ } 
}

