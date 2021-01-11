/*---------------------------------------------------------------------
by Kang Woo Jung: 2019.03.27

-----------------------------------------------------------------------*/
#ifndef __EVT_H__
#define __EVT_H__

#include "ktype.h"

#if defined(STM32F746xx) || defined(STM32F407xx)
	#define EVT_TIM_CNT	TIM2->CNT		// clock source = 1MHz
	//#define EVT_TIM_SIZ	0x100000000
	typedef uint32_t	evt_t;					// 32bit, unit = 1us
	typedef uint32_t	tm_t;						// 32bit, unit = 1us
	 
#elif defined(STM32L431xx)
	#define EVT_TIM_CNT	TIM7->CNT		// clock source = 1MHz
	//#define EVT_TIM_SIZ	0x100000000
	typedef uint32_t	evt_t;					// 32bit, unit = 1us
	typedef uint32_t	tm_t;						// 32bit, unit = 1us

#elif defined(STM32F303xC)
	#define EVT_TIM_CNT	TIM2->CNT		// clock source = 1MHz
	//#define EVT_TIM_SIZ	0x100000000
	typedef uint32_t	evt_t;					// 32bit, unit = 1us
	typedef uint32_t	tm_t;						// 32bit, unit = 1us
	 
#elif defined(STM32F103xB)
	#define EVT_TIM_CNT	TIM3->CNT		// clock source = 1MHz
	//#define EVT_TIM_SIZ	0x10000
	typedef uint16_t	evt_t;					// 16bit, unit = 1us
	typedef uint16_t	tm_t;						// 16bit, unit = 1us
	 
#else
	#error  "Error! ../klib/event.h"
	 
#endif

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
	evt_t evt;
}event_t;
#define pwm_t	event_t		// Ä¡¿Ï
#define tmr_t	event_t
#define pha_t	event_t
#define one_t	event_t

typedef struct {
	uint16_t out :1;
	evt_t evt;
	tm_t  count;
}indel_t;

typedef struct {
	uint16_t pre :1;
	uint16_t pre2 :1;  // for capture_width();
	uint16_t in :1;
	//
	evt_t evt;
	tm_t width_L;
	tm_t width_H;
	tm_t period;
	tm_t half_period;
}cap_t;


//-------------------------------------------------------------
extern tm_t looptick_us; //us
extern tm_t looptick_ms; //ms

void evt_process(void);
//
tm_t evt_capus(evt_t *evt);
tm_t evt_capms(evt_t *evt);
uint16_t evt_delayus(evt_t *delay, tm_t time);
uint16_t evt_delayms(evt_t *delay, tm_t time);
uint16_t evt_timerus(evt_t *timer, tm_t time);
uint16_t evt_timerms(evt_t *timer, tm_t time);
uint16_t evt_us(evt_t *evt, tm_t time);
uint16_t evt_ms(evt_t *evt, tm_t time);

//-------------------------------------------------------------
uint16_t indelay(indel_t *indel, tm_t del_ms, uint16_t source); // for GPIO Input
void pwm_process(pwm_t *pwm,  tm_t period,  tm_t pulse);
void pwm_reset(pwm_t *pwm);
void capture_process(cap_t *cap);		// capture pulse
void phase_del_process(pha_t *pha, tm_t time);
 


#endif



