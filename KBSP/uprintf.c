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
/* printf() ���� ������
%c    charactor
%d    ��ȣ�ִ� int
%e, %E    ������ǥ���
%f      float
%g, %G    %e,%f �� �ܼ��� ������ �����ؼ� ���
%s   string
%u   ��ȣ���� int
%x   hex
%X   HEX
%p   pointer
*/

