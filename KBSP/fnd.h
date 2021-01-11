/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/
#ifndef __FND_H__
#define __FND_H__

#include "ktype.h"

#define FND_DATA  PORTD.high
#define FND_DRV0	PortC.b6
#define FND_DRV1	PortC.b7
#define FND_DRV2	PortC.b8

//#define FND_DATA_OUT(val) { FND_DATA |= (val<<8); }
#define FND_DRIVE0() { FND_DRV2=OFF; FND_DATA=fnd_buf[0]; FND_DRV0=ON; }
#define FND_DRIVE1() { FND_DRV0=OFF; FND_DATA=fnd_buf[1]; FND_DRV1=ON; }
#define FND_DRIVE2() { FND_DRV1=OFF; FND_DATA=fnd_buf[2]; FND_DRV2=ON; }

#define DIGIT_SIZE 3

#define SEG7  7
#define SEG8  8
#define DIGIT0 0
#define DIGIT1 1
#define DIGIT2 2

extern uint8_t fnd_buf[DIGIT_SIZE+1];////////////

void fnd_process(void);
void fnd_img(uint8_t *img);
void fnd_printf(uint16_t seg_mode, uint16_t digit, const uint8_t *fmt,...);

//----------------------------------------- for printf()
/* printf() 포맷 지정자
%c    charactor
%d    부호있는 int
%e, %E    과학적표기법
%f      float
%g, %G    %e,%f 중 단순한 것으로 선택해서 출력
%s   string
%u   부호없는 int
%x   hex
%X   HEX
%p   pointer
*/
	
#endif

