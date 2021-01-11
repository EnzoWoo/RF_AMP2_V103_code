/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/
#ifndef __FND_H__
#define __FND_H__

#include "ktype.h"

#define SEG(val) { GPIOB->ODR &= 0xff; GPIOB->ODR |= (val<<8); }
#define SEG0	PortB.b8
#define SEG1	PortB.b9
#define SEG2	PortB.b10
#define SEG3	PortB.b11
#define SEG4	PortB.b12
#define SEG5	PortB.b13
#define SEG6	PortB.b14
#define SEG7	PortB.b15

#define DIGIT0	PortD.b0
#define DIGIT1	PortD.b1
#define DIGIT2	PortD.b2

extern uint8_t fnd[3+1];////////////

void fnd_drive(void);
void dprintf(uint8_t *fnd, uint8_t digit_num, const uint8_t *fmt,...);

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

