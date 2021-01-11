/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/
#include "bsp.h"
#include "bprintf.h"


void bprintf(uint8_t *buf, uint16_t size, const uint8_t *fmt,...)
{
	uint8_t str[64];
	va_list ap;
	
	va_start(ap, fmt);
	vsprintf((char*)str,(const char*)fmt, ap);
	va_end(ap);

	int16_t i=0;
	while(str[i]){
		buf[i]=str[i];
		if(++i >= size) break;
	}
}

/*
void bprintf2(uint8_t *buf, const uint8_t *fmt,...) 
{
	va_list ap;
	
	va_start(ap, fmt);
	vsprintf((char*)buf,(const char*)fmt, ap);
	va_end(ap);
}
*/

//----------------------------------------- for printf()
/* printf()
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

