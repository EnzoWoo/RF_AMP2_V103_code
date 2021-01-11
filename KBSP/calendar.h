/*-----------------------------------------
by Kang Woo Jung

2018.11.01

-------------------------------------------*/

#ifndef __RTC_H__
#define __RTC_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "ktype.h"

typedef struct {
	RTC_DateTypeDef sDate;
	RTC_TimeTypeDef sTime;
}rtc_t;
extern rtc_t 	rtc;

void rtc_init(void);
void rtc_process(void);



#ifdef __cplusplus
}
#endif


#endif

