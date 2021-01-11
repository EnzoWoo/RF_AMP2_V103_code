/*-----------------------------------------
by Kang Woo Jung

2018.11.01

-------------------------------------------*/

#include "user.h"
#include "rtc_.h"

rtc_t 	rtc;

const char *RTC_WEEK[] = {"  ","MON","TUE","WED","THU","FRI","SAT","SUN"};

void HAL_RTCEx_TimeStampEventCallback(RTC_HandleTypeDef *hrtc)
{
	kprintf(&kpro1,(cu8 *)"RTC Event! ^,.^\n");
}

void rtc_init(void)
{
  rtc.sDate.Year = 18;
  rtc.sDate.Month = 11;
  rtc.sDate.Date = 1;
  rtc.sDate.WeekDay = RTC_WEEKDAY_THURSDAY;  
  HAL_RTC_SetDate(&hrtc, &rtc.sDate, RTC_FORMAT_BIN);
  
  rtc.sTime.Hours = 18;
  rtc.sTime.Minutes = 19;
  rtc.sTime.Seconds = 0;
  rtc.sTime.TimeFormat = RTC_HOURFORMAT_24;
  rtc.sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  rtc.sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  HAL_RTC_SetTime(&hrtc, &rtc.sTime, RTC_FORMAT_BIN);
}

void rtc_process(void)
{
	static evt_t evt;
	if(!event(&evt,1000000)) return;
    
  HAL_RTC_GetTime(&hrtc, &rtc.sTime, RTC_FORMAT_BIN); 
  HAL_RTC_GetDate(&hrtc, &rtc.sDate, RTC_FORMAT_BIN);

  kprintf(&kpro1,(cu8 *)" 20%d-%d-%d(%s), ", rtc.sDate.Year, rtc.sDate.Month, rtc.sDate.Date, RTC_WEEK[rtc.sDate.WeekDay]);
  kprintf(&kpro1,(cu8 *)" %d:%d:%d\n\r", rtc.sTime.Hours, rtc.sTime.Minutes, rtc.sTime.Seconds);
}

