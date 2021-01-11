/*---------------------------------------------------------------------
by Kang Woo Jung: 2019.03.27

-----------------------------------------------------------------------*/
#include "event.h"

extern __IO uint32_t uwTick; //sysTick counter

//------------------------------------------------------------- looptick
tm_t  looptick_us; //us
tm_t  looptick_ms; //ms
void evt_process(void)
{
	static evt_t evt_us, evt_ms;
	tm_t cap;
	//
	cap = EVT_TIM_CNT;
	looptick_us = cap - evt_us;	
  evt_us = cap;
	//
	//cap = HAL_GetTick();
	cap = uwTick;
	looptick_ms = cap - evt_ms;	
  evt_ms = cap;
}

//------------------------------------------------------------- cap
tm_t evt_capus(evt_t *evt)
{
	tm_t cap, num;
	cap = EVT_TIM_CNT;
	num = cap - *evt;
	*evt = cap;
	return (num);
}

tm_t evt_capms(evt_t *evt)
{
	tm_t cap, num;
	//cap = HAL_GetTick();
	cap = uwTick;
	num = cap - *evt;
	*evt = cap;
	return (num);
}

//------------------------------------------------------------- delay
uint16_t evt_delayus(evt_t *delay, tm_t time)
{
	*delay += looptick_us;
	if(*delay >= time){ *delay = 0;	return 1;	}
	return 0;
}

uint16_t evt_delayms(evt_t *delay, tm_t time)
{
	*delay += looptick_ms;
	if(*delay >= time){ *delay = 0;	return 1;	}
	return 0;
}

uint16_t evt_timerus(evt_t *timer, tm_t time)
{
	*timer += looptick_us;
	if(*timer >= time){ *timer = time;	return 1;	}
	return 0;
}

uint16_t evt_timerms(evt_t *timer, tm_t time)
{
	*timer += looptick_ms;
	if(*timer >= time){ *timer = time;	return 1;	}
	return 0;
}

//------------------------------------------------------------- event
uint16_t evt_us(evt_t *evt, tm_t time)		
{
	tm_t cap, num;
	cap = EVT_TIM_CNT;
	num = cap - *evt;
	if(num >= time){
		*evt = cap - num + time;	//지연 보정
		return (1);
	}
	return (0);
}

uint16_t evt_ms(evt_t *evt, tm_t time)		
{
	tm_t cap, num;
	cap = HAL_GetTick();
	num = cap - *evt;
	if(num >= time){
		*evt = cap - num + time;	//지연 보정
		return (1);
	}
	return (0);
}


//------------------------------------------------------------- indelay
uint16_t indelay(indel_t *indel, tm_t del_ms, uint16_t source)
{
	static tm_t usec;
	usec += looptick_us;
	if(usec < 1000)	return 0;
	usec = 0;
	
	if(source&1){
		if(++indel->count > del_ms){
			indel->count = del_ms;
			indel->out = 1;
		}
	}
	else {
		if(--indel->count<1){
			indel->count = 1;
			indel->out = 0;
		}
	}
	
	return (indel->out);
}


//------------------------------------------------------------- pwm
void pwm_process(pwm_t *pwm, tm_t period, tm_t pulse)
{
	tm_t cap, num;
	cap = EVT_TIM_CNT;
	num = cap - pwm->evt;
	pwm->mak = pwm->bra = pwm->bot = 0;
	if(num >= period){
		pwm->evt = cap - num + period;	// 지연 보정	
	}
	if(num < pulse){
		pwm->out = pwm->mak = pwm->bot = 1;
		pwm->outn = 0;
	}
	else{
		pwm->outn = pwm->bra = pwm->bot = 1;
		pwm->out = 0;
	}
}

void pwm_reset(pwm_t *pwm)
{
	pwm->evt = EVT_TIM_CNT;
}

//------------------------------------------------------------- pulse capture
void capture_process(cap_t *cap)
{
	if(cap->pre == cap->in) return;
	cap->pre = cap->in;
	
	tm_t capt, num;
	capt = EVT_TIM_CNT;
	num = capt - cap->evt;
	cap->evt = capt;
	//
	if(cap->in)	cap->width_H = num;
	else cap->width_L = num;
	cap->period = cap->width_L + cap->width_H;
	cap->half_period = num;
}

//------------------------------------------------------------- phase delay
void phase_del_process(pha_t *pha, tm_t time)
{
	tm_t cap, num;
	cap = EVT_TIM_CNT;
	num = cap - pha->evt;

	pha->mak = pha->bra = pha->bot = 0;
	if(pha->out){
		if(pha->in){
			pha->evt = cap;
		}
		else {
			if(num >= time){
				pha->outn = pha->bra = pha->bot = pha->out;
				pha->out = 0;
			}
		}
	}
	else {
		if(pha->in){
			if(num >= time){
				pha->out = 1;
				pha->mak = pha->bot = pha->out;
				pha->outn = !pha->out;
			}
		}
		else {
			pha->evt = cap;
		}
	}
}



