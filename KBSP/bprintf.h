/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/
#ifndef __BPRINTF_H__
#define __BPRINTF_H__

#include "ktype.h"

void bprintf(uint8_t *buf, uint16_t size, const uint8_t *fmt,...);
//void bprintf2(uint8_t *buf, const uint8_t *fmt,...) // buf 크기주의!, NUL문자포함


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

