/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/
#include "uprintf.h"

void uprintf(UART_HandleTypeDef *huart, const uint8_t *fmt,...)
{
	static uint8_t str[128], size;
	va_list ap;
	
	va_start(ap, fmt);
	vsprintf((char *) str,(const char *)fmt, ap);
	va_end(ap);
	
	size = 0;
	while(str[size]) size++;
	while(HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY_TX);
	HAL_UART_Transmit(huart, str, size, 10);
	//while(HAL_UART_Transmit_DMA(huart, str, size) != HAL_OK);
}

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

