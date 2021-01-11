/*---------------------------------------------------------------------
by Kang Woo Jung: 2020.04.03

-----------------------------------------------------------------------*/
#ifndef __KCRC_H__
#define __KCRC_H__ 

#include "ktype.h"


#define PolynomCode_16        0xA001
#define PolynomCode_32        0xEDB88320L

// size 인수값 필히 u16 사용할 것... ㅠㅠ	 
uint16_t crc16(uint16_t crc, uint8_t dat);
uint16_t crc16_buf(uint16_t crc, uint8_t *Buf, uint16_t size);
uint32_t crc32(uint32_t crc, uint8_t dat);
uint32_t crc32_buf(uint32_t crc, uint8_t *Buf, uint16_t size);
	 
uint8_t sum8(uint8_t sum, uint8_t dat);
uint8_t sum8_buf(uint8_t sum, uint8_t *Buf, uint16_t size) ;	 
uint16_t sum16(uint16_t sum, uint8_t dat);
uint16_t sum16_buf(uint16_t sum, uint8_t *Buf, uint16_t size) ;	 

uint8_t xor8(uint8_t Xor, uint8_t dat);
uint8_t xor8_buf(uint8_t Xor, uint8_t *Buf, uint16_t size) ;	 

#endif


