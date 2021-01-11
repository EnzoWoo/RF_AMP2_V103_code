/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/
#include "evt.h"

//--------------------------------------------------------------------- ms
extern __IO uint32_t uwTick; //sysTick counter

uint16_t evms_evt(evms_t *evt, evms_t time)
{
	evms_t cap, num;
	//cap = HAL_GetTick();
	cap = uwTick;
	num = cap - *evt;
	if(num >= time){
		*evt = cap - num + time;	//지연 보정
		return (1);
	}
	return (0);
}
evms_t evms_cnt(evms_t *evt)
{
	evms_t cap, num;
	//cap = HAL_GetTick();
	cap = uwTick;
	num = cap - *evt;
	return (num);
}
void evms_rst(evms_t *evt)
{
	//*evt = HAL_GetTick();
	*evt = uwTick;
}
void evms_set(evms_t *evt)
{
	//*evt = HAL_GetTick() + (MSEC_VAL * 10);
	*evt = uwTick - 1;
}
evms_t evms_cap(evms_t *evt)
{
	evms_t cap, num;
	//cap = HAL_GetTick();
	cap = uwTick;
	num = cap - *evt;
	*evt = cap;
	return (num);
}

//--------------------------------------------------------------------- us
uint16_t evus_evt(evus_t *evt, evus_t time)
{
	evus_t cap, num;
	cap = EVT_TIM_CNT;
	num = cap - *evt;
	if(num >= time){
		*evt = cap - num + time;	//지연 보정
		return (1);
	}
	return (0);
}
uint16_t evus_cnt(evus_t *evt)
{
	evus_t cap, num;
	cap = EVT_TIM_CNT;
	num = cap - *evt;
	return (num);
}
void evus_rst(evus_t *evt)
{
	*evt = EVT_TIM_CNT;
}
void evus_set(evus_t *evt)
{
	*evt = EVT_TIM_CNT - 1;
}
uint16_t evus_cap(evus_t *evt)
{
	evus_t cap, num;
	cap = EVT_TIM_CNT;
	num = cap - *evt;
	*evt = cap;
	return (num);
}


//--------------------------------------------------------------------- ms
uint16_t evdel(evdel_t *evdel, evms_t ms, uint16_t source)
{
	if(evus_evt(&evdel->evt, 1000) ){
		if(source&1){
			if(++evdel->count > ms){
				evdel->count = ms;
				evdel->out = 1;
			}
		}
		else {
			if(--evdel->count<1){
				evdel->count = 1;
				evdel->out = 0;
			}
		}
	}	
	return (evdel->out);
}


//--------------------------------------------------------------------- us
void pwm_process(pwm_t *pwm, evus_t period, evus_t pulse)
{
	evus_t cap, num;
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


void capture_process(cap_t *cap)
{
	if(cap->pre == cap->in) return;
	cap->pre = cap->in;
	
	evus_t capt, num;
	capt = EVT_TIM_CNT;
	num = capt - cap->evt;
	cap->evt = capt;
	//
	if(cap->in)	cap->width_H = num;
	else cap->width_L = num;
	cap->period = cap->width_L + cap->width_H;
	cap->half_period = num;
}


void oneshot_process(one_t *one, evus_t time)
{
	evus_t cap, num;
	cap = EVT_TIM_CNT;
	num = cap - one->evt;

	one->mak = one->bra = one->bot = 0;
	if(one->pre ^ one->in){
		if(one->in){
			one->mak |= one->outn;
			one->bot |= one->outn;
			one->out = one->in;
			one->outn = !one->out;
			one->evt = cap;
		}
		one->pre = one->in;
	}
	if(one->out){
		if(num >= time){
			one->out = 0;
			one->bra = one->bot = one->outn = !one->out;
		}
	}
	else {
		one->evt = cap; // 중요!!!
	}
}


void phase_del_process(pha_t *pha, evus_t time)
{
	evus_t cap, num;
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


void timer_process(tmr_t *tmr, evus_t time)
{
	evus_t cap, num;
	cap = EVT_TIM_CNT;
	num = cap - tmr->evt;
	
	tmr->mak = tmr->bra = tmr->bot = 0;
	if(tmr->in){
		if(num >= time){
			tmr->evt = cap - time;	// 보정
			tmr->mak |= !tmr->out;
			// tmr->mak = !tmr->out;	// 되는지 해봐야함
			tmr->bot |= !tmr->out;
			tmr->out = 1;
			tmr->outn = !tmr->out;
		}
	}
	else {
		tmr->evt = cap;
		tmr->bra |= tmr->out;
		tmr->bot |= tmr->out;
		tmr->out = 0;
		tmr->outn = !tmr->out;
	}
}


