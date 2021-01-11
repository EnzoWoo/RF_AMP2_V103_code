/*-----------------------------------------
by Kang Woo Jung

2016.10.06

-------------------------------------------*/

#ifndef __UPRINTF_H__
#define __UPRINTF_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "ktype.h"

void uprintf(UART_HandleTypeDef *huart, const uint8_t *fmt,...);
	 
	 
#ifdef __cplusplus
}
#endif
#endif
