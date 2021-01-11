/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/
#ifndef __BPRINTF_H__
#define __BPRINTF_H__

#include "ktype.h"

void bprintf(uint8_t *buf, uint16_t size, const uint8_t *fmt,...);
//void bprintf2(uint8_t *buf, const uint8_t *fmt,...) // buf ũ������!, NUL��������


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
	
#endif

