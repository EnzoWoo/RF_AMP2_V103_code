/*---------------------------------------------------------------------
by Kang Woo Jung: 2019.02.18

-----------------------------------------------------------------------*/

#include "inverter.h"
#include "usr.h"
#include "sin.h"



Type_Inv  inv;


float  inverter_control(float dc_bus, float ref)
{
	// ref limit
	if(ref > MOT_RPM_MAX) ref = MOT_RPM_MAX;
	
	// soft start/stop
	static evt_t evt;
	if(ref > MOT_RPM_MIN){
		if(event(&evt, INV_RPM_KI_PERIOD)){
			inv.rpm = Filter(INV_RPM_KI, ref, inv.rpm);
		}
	}
	else {
		inv.rpm = 0;
	}

	// set tim1 pwm amplitude
	inv.amp = (INV_PWM_MAX * MOT_VOLT_MAX / MOT_RPM_MAX) * (inv.rpm / dc_bus);
	if(inv.amp > INV_PWM_MAX)	inv.amp = INV_PWM_MAX;
	
	// motor speed setting
	static uint32_t evt1;
	static uint32_t evt1_time;
	if(inv.rpm > MOT_RPM_MIN)	evt1_time = EVENT_TIME_REF / inv.rpm;  // event 6kHz, inv 60Hz, motor 1800rpm
	else evt1_time = EVENT_TIME_REF / MOT_RPM_MIN;		// event 200Hz, inv 2Hz, motor 60rpm
	
	// motor speed event
	if(event(&evt1, evt1_time)){    // max 6kHz(1800rpm), min 60Hz(60rpm)
		TP12 = 1; ////////////////////////////////////////////////////////////
		static uint16_t step;
		uint16_t pwm;
		if(inv.rpm >= MOT_RPM_MIN){
			pwm = (uint16_t) (inv.amp * ((float) SIN_TBL[step] / SIN_MAX_VAL));
			if(step < (SIN_TBL_SIZ/2)){
				INV_PWM34 = 0;
				INV_PWM12 = pwm;
			}
			else {
				INV_PWM12 = 0;
				INV_PWM34 = pwm;
			}
			step++;
			step %= SIN_TBL_SIZ;
		}
		else {
			step = 0;
			INV_PWM12 = 0;
			INV_PWM34 = 0;
		}
		TP12 ^= 1; ////////////////////////////////////////////////////////////
	}

	return  inv.amp;
}


