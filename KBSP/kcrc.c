/*---------------------------------------------------------------------
by Kang Woo Jung: 2020.04.03

-----------------------------------------------------------------------*/

#include "kcrc.h"

uint16_t crc16(uint16_t crc, uint8_t dat)
{
    crc ^= ((uint16_t) dat & 0xff);
    for(int i=0; i<8 ; i++){
        if(crc & 0x0001){ crc = (crc >> 1) ^ PolynomCode_16; }
        else{ crc >>= 1; }
    }
    return (crc);
}
uint16_t crc16_buf(uint16_t crc, uint8_t *Buf, uint16_t size) 
{
	while(size--){
    crc ^= ((uint16_t) *Buf & 0xff);
    for(int i=0; i<8 ; i++){
        if(crc & 0x0001){ crc = (crc >> 1) ^ PolynomCode_16; }
        else{ crc >>= 1; }
    }
		Buf++;
	}
	return crc;
}

uint32_t crc32(uint32_t crc, uint8_t dat)
{
    crc ^= ((uint32_t) dat & 0xff);
    for(int i=0; i<8 ; i++){
        if(crc & 0x00000001U){ crc = (crc >> 1) ^ PolynomCode_32; }
        else{ crc >>= 1; }
    }
    return (crc);
}
uint32_t crc32_buf(uint32_t crc, uint8_t *Buf, uint16_t size) 
{
	while(size--){
    crc ^= ((uint16_t) *Buf & 0xff);
    for(int i=0; i<8 ; i++){
        if(crc & 0x00000001U){ crc = (crc >> 1) ^ PolynomCode_32; }
        else{ crc >>= 1; }
    }
		Buf++;
	}
	return crc;
}

uint8_t sum8(uint8_t sum, uint8_t dat)
{
    sum += dat;
    return (sum);
}
uint8_t sum8_buf(uint8_t sum, uint8_t *Buf, uint16_t size) 
{
    while(size){
        sum += *Buf;
        Buf++;
        size--;
    }
    return (sum);
}
uint16_t sum16(uint16_t sum, uint8_t dat)
{
    sum += dat;
    return (sum);
}
uint16_t sum16_buf(uint16_t sum, uint8_t *Buf, uint16_t size) 
{
    while(size){
        sum += *Buf;
        Buf++;
        size--;
    }
    return (sum);
}


uint8_t xor8(uint8_t Xor, uint8_t dat)
{
    Xor ^= dat;
    return (Xor);
}
uint8_t xor8_buf(uint8_t Xor, uint8_t *Buf, uint16_t size) 
{
    while(size){
        Xor ^= *Buf;
        Buf++;
        size--;
    }
    return (Xor);
}

