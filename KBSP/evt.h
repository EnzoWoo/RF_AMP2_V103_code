/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/
#ifndef __EVT_H__
#define __EVT_H__

#include "ktype.h"

#if defined(STM32F746xx) || defined(STM32F407xx)
	#define EVT_TIM_CNT	TIM6->CNT		// clock source = 1MHz
	 
#elif defined(STM32L431xx)
	#define EVT_TIM_CNT	TIM7->CNT		// clock source = 1MHz

#elif defined(STM32F303xC)
	#define EVT_TIM_CNT	TIM6->CNT		// clock source = 1MHz
	 
#elif defined(STM32F103xB) || defined(STM32L053xx)
	#define EVT_TIM_CNT	 TIM2->CNT	// clock source = 1MHz
	 
#else
	#error  "Error! ../klib/event.h"
	 
#endif
#define EVT_TIM_SIZ	0x10000
typedef uint32_t	evms_t;
typedef uint16_t	evus_t;

uint16_t evms_evt(evms_t *evt, evms_t time);
evms_t evms_cnt(evms_t *evt);
void evms_rst(evms_t *evt);
void evms_set(evms_t *evt);
evms_t evms_cap(evms_t *evt);

uint16_t evus_evt(evus_t *evt, evus_t time);
evus_t evus_cnt(evus_t *evt);
void evus_rst(evus_t *evt);
void evus_set(evus_t *evt);
evus_t evus_cap(evus_t *evt);


typedef struct {
	uint16_t pre :1;
	uint16_t in :1;
	uint16_t out  :1;
	uint16_t outn :1;
	uint16_t mak :1;
	uint16_t bra  :1;
	uint16_t bot :1;
	uint16_t b7 :1;
	//
	evus_t evt;
}kevt_t;
#define pwm_t	kevt_t		// Ä¡¿Ï
#define tmr_t	kevt_t
#define pha_t	kevt_t
#define one_t	kevt_t

typedef struct {
	uint16_t out :1;
	evus_t evt;
	evus_t count;
}evdel_t;
uint16_t evdel(evdel_t *evdel, evms_t del_ms, uint16_t source); // for GPIO Input

typedef struct {
	uint16_t pre :1;
	uint16_t pre2 :1;  // for capture_width();
	uint16_t in :1;
	//
	evus_t evt;
	evus_t width_L;
	evus_t width_H;
	evus_t period;
	evus_t half_period;
}cap_t;
void pwm_process(pwm_t *pwm,  evus_t period,  evus_t pulse);
void pwm_reset(pwm_t *pwm);
void capture_process(cap_t *cap);		// capture pulse
void oneshot_process(one_t *one, evus_t time);
void phase_del_process(pha_t *pha, evus_t time);
void timer_process(tmr_t *tmr, evus_t time);
 

#endif



