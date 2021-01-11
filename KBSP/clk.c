/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/

#include "bsp.h"
#include "clk.h"

RTC_DateTypeDef clk_sDate;
RTC_TimeTypeDef clk_sTime;


const char *clkDAY[DAY_SIZE] = {"  ","MON","TUE","WED","THU","FRI","SAT","SUN"};

void HAL_RTCEx_TimeStampEventCallback(RTC_HandleTypeDef *hrtc)
{
	//kprintf(&kpro1,(cu8 *)"RTC Event! ^,.^\n");
}


void clk_set_date(uint16_t year, uint16_t month, uint16_t date, day_t day)
{
  clk_sDate.Year = year;
  clk_sDate.Month = month;
  clk_sDate.Date = date;
  clk_sDate.WeekDay = day;  
  HAL_RTC_SetDate(&hrtc, &clk_sDate, RTC_FORMAT_BIN);
}

void clk_set_time(uint16_t hour, uint16_t min, uint16_t sec)
{
  clk_sTime.Hours = hour;
  clk_sTime.Minutes = min;
  clk_sTime.Seconds = sec;
  clk_sTime.TimeFormat = RTC_HOURFORMAT_24;
  clk_sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  clk_sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  HAL_RTC_SetTime(&hrtc, &clk_sTime, RTC_FORMAT_BIN);
}

void clk_get(clk_t *clk)
{
  HAL_RTC_GetTime(&hrtc, &clk_sTime, RTC_FORMAT_BIN); 
  HAL_RTC_GetDate(&hrtc, &clk_sDate, RTC_FORMAT_BIN);
	clk->year = clk_sDate.Year;
	clk->month = clk_sDate.Month;
	clk->date = clk_sDate.Date;
	clk->day = clk_sDate.WeekDay;
	clk->hour = clk_sTime.Hours;
	clk->min = clk_sTime.Minutes;
	clk->sec = clk_sTime.Seconds;
}
