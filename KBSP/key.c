/*---------------------------------------------------------------------
by Kang Woo Jung: 2019.03.25

-----------------------------------------------------------------------*/

#include "bsp.h"
#include "key.h"

#define  VELOCITY_BUF_SIZE   8
uint32_t key_velocity[VELOCITY_BUF_SIZE]={
	200,100,50,25,13,6,3,1
};


void in_process(in_t * in, uint16_t in_su)  // in_su : 1~32 // in.det 에 소스연결
{
	uint32_t i;
	
	in->bot = in->bra = in->mak = 0;
	
	//static evt_t evt;
	//if(evt_us(&evt, MSEC_VAL) == 0) return;
	static tm_t usec;
	usec += looptick_us;
	if(usec < 1000)	return;
	usec = 0;

	if(in_su > 32) in_su = 32;
	for(i=0;i<in_su;i++)	{
		/* Kill Chattering */
		if(in->det&(1<<i)){
			if(++in->chattering[i] > CHATTERING_TIME){
				in->chattering[i] = CHATTERING_TIME;
				in->lev|=(1<<i);
			}
		}
		else {
			if(--in->chattering[i]<1){
				in->chattering[i]=1;
				in->lev&=(~(1<<i));
			}
		}
		
		/* make in, brake in, short in */
		if((in->lev^in->pre)&(1<<i)){
			in->pre&=(~(1<<i));
			in->pre|=(in->lev&(1<<i));
			if(in->lev&(1<<i)){
				in->mak|=(1<<i); /* make in */
			}
			else {
				in->bra|=(1<<i); /* brake in */
			}
			in->bot = in->bra | in->mak; // both in
			in->hol = in->mak; // hold in
		}
	}
}





void key_process(key_t * key, uint16_t key_su)  // key_su : 1~32 // key.det 에 소스연결
{
	uint32_t i;
	
	key->spe = key->one = key->two = key->thr = key->bot = key->bra = key->mak = 0;
	
	//static evt_t evt;
	//if(evt_us(&evt, MSEC_VAL) == 0) return;
	static tm_t usec;
	usec += looptick_us;
	if(usec < 1000)	return;
	usec = 0;
	
	if(key_su > 32) key_su = 32;
	for(i=0;i<key_su;i++)	{
		/* Kill Chattering */
		if(key->det&(1<<i)){
			if(++key->chattering[i] > CHATTERING_TIME){
				key->chattering[i] = CHATTERING_TIME;
				key->lev|=(1<<i);
			}
		}
		else {
			if(--key->chattering[i]<1){
				key->chattering[i]=1;
				key->lev&=(~(1<<i));
			}
		}
		
		/* make key, brake key, short key */
		if((key->lev^key->pre)&(1<<i)){
			key->pre&=(~(1<<i));
			key->pre|=(key->lev&(1<<i));
			if(key->lev&(1<<i)){
				key->mak|=(1<<i); /* make key */
				key->spe|=(1<<i); /* speed key 1st */
				
				//Initialization
				key->delay[i]=0; 
				key->speed[i]=0;
				key->acc_interval[i]=0;
				key->acc_step[i] = 0;
			}
			else {
				key->bra|=(1<<i); /* brake key */
				if(key->delay[i] < TWO_FUNC_TIME){
					key->one|=(1<<i); // one function
				}
				
			}
			key->bot = key->bra | key->mak; // both key
			key->hol = key->mak; // hold key
		}
		
  	/* speed key, long key */
		if(key->lev&(1<<i)){
			if(key->delay[i] < THREE_FUNC_TIME+1)	key->delay[i]++;
			//
			if(key->delay[i] > SPEED_WAIT_TIME){
				if(++key->speed[i] > key_velocity[key->acc_step[i]]){
					key->speed[i]=0;
					key->spe|=(1<<i); /* speed key 2nd,3dr,4th...*/
				}
			}
			if(++key->delay[i] == THREE_FUNC_TIME){
				key->delay[i] = THREE_FUNC_TIME+1;
				key->thr|=(1<<i); // three function
			}
			else if(key->delay[i] == TWO_FUNC_TIME){
				key->two|=(1<<i); // two function
			}
		}
        
		/* accelerate */
		if(++key->acc_interval[i] > ACC_INTERVAL_TIME){
			key->acc_interval[i]=0;
			key->speed[i]=0;
			if(key->acc_step[i] < VELOCITY_BUF_SIZE-1) key->acc_step[i]++;
		}
	}
}


