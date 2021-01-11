/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/

#ifndef __CLK_H__
#define __CLK_H__

#include "ktype.h"

typedef enum {
	DAY_NUL=0,
	DAY_MON,
	DAY_TUE,
	DAY_WED,
	DAY_THU,
	DAY_FRI,
	DAY_SAT,
	DAY_SUN,
	DAY_SIZE
}day_t;

extern const char *clkDAY[DAY_SIZE];

typedef struct {
	uint16_t year;
	uint16_t month;
	uint16_t date;
	uint16_t day;
	uint16_t hour;
	uint16_t min;
	uint16_t sec;
	
}clk_t;

void clk_set_date(uint16_t year, uint16_t month, uint16_t date, day_t day);
void clk_set_time(uint16_t hour, uint16_t min, uint16_t sec);
void clk_get(clk_t *clk);


#endif

