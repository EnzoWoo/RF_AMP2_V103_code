/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/

#include "crc.h"

uint16_t crc16(uint16_t crc, uint8_t dat)
{
    uint16_t i;

    crc ^= ((uint16_t) dat & 0xff); // size 인수값 필히 u16 사용할 것... ㅠㅠ
	
    for(i=0; i<8 ; i++){
        if(crc & 0x0001){ crc = (crc >> 1) ^ PolynomCode_16; }
        else{ crc >>= 1; }
    }
    
    return (crc);
}


uint16_t crc16_buf(uint16_t crc, uint8_t *Buf, uint16_t size) 
{
    uint16_t i;

    while(size){
        crc ^= ((uint16_t) *Buf & 0xff);  // size 인수값 필히 u16 사용할 것... ㅠㅠ
        Buf++;
        size--;
        for(i=0; i<8 ; i++){
            if(crc & 0x0001){ crc = (crc >> 1) ^ PolynomCode_16; }
            else{ crc >>= 1; }
        }
    }
    
    return (crc);
}


uint8_t xor8(uint8_t xor_code, uint8_t dat)
{
    xor_code ^= dat;
    return (xor_code);
}


uint8_t xor8_buf(uint8_t xor_code, uint8_t *Buf, uint16_t size) 
{
    while(size){
        xor_code ^= *Buf;
        Buf++;
        size--;
    }
    return (xor_code);
}


