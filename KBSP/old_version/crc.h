/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/
#ifndef __CRC_H__
#define __CRC_H__ 

#include "ktype.h"


#define PolynomCode_16        0xA001
#define PolynomCode_32        0xEDB88320L

	 
uint16_t crc16(uint16_t crc, uint8_t dat);
uint16_t crc16_buf(uint16_t crc, uint8_t *Buf, uint16_t size); // size 인수값 필히 u16 사용할 것... ㅠㅠ

	 
uint8_t xor8(uint8_t xor_code, uint8_t dat);
uint8_t xor8_buf(uint8_t xor_code, uint8_t *Buf, uint16_t size) ;	 
 
#endif


