/*-----------------------------------------
by Kang Woo Jung

2017. 01. 17

-------------------------------------------*/

#include "klib.h"
#include "water_temp.h"

#include "user.h"


Type_PID 	pid_water;

void water_pid_init(void)
{
	Dsp_PID_Init(&pid_water, WATER_PID_KP, WATER_PID_KI, WATER_PID_KD, WATER_AMP_TOP); // PID init
}

float water_pid(float temp, float ref)
{	
/*
	static float prev;
	if(prev != ref){
		if(prev == 0.0F){
			//PID  init
			Dsp_PID_Init(&pid_water, WATER_PID_KP, WATER_PID_KI, WATER_PID_KD, WATER_AMP_TOP); // PID init
		}
		prev = ref;
	}
*/
	//ref limit
	if(ref < WATER_REF_MIN) ref = WATER_REF_MIN;
	if(ref > WATER_REF_MAX) ref = WATER_REF_MAX;
	
	// PID
	static float amp, amp2, err, err2;
	static uint32_t evt;
	if(evt_out(&evt, WATER_PID_PERIOD)){
		
		err = temp - ref; // 위치변경불가!!!
		err2 = Filter(WATER_PI_ERR_KI, err, err2); // 위치변경불가!!!
		
		if(err2 > WATER_PI_CTR_TEMP){  // 수식 절대 수정하지 말 것!!!!!, 상수 값은 변경가능.
			// base PID
			amp = Dsp_PID(&pid_water, err);	// PID
		}
		else {
			// base PID
			amp2 = Dsp_PID(&pid_water, err2);	// PID
			//PI
			amp2 *= (err2 * WATER_PI_KP);		// P
			amp = Filter(WATER_PI_AMP_KI, amp2, amp); // I
		}
	}
	
	if(amp > WATER_AMP_TOP) amp = WATER_AMP_TOP;
	if(amp < 0.0F) amp = 0.0F;
	
	return (amp + WATER_AMP_MIN);  // return water pid amp : inverter rpm value
}
	




