/*-----------------------------------------
by Kang Woo Jung

2017.03.10

-------------------------------------------*/

#include "user.h"
#include "rf.h"


rf_t 		rf;


void rf_control_init(uint16_t set)
{
	if(!set) return; 
	dsp_pid2_init(&rf.pid_vo, RF_PID_VO_KP, RF_PID_VO_KI, RF_PID_VO_KD, RF_PWM_LIMIT, 0);
	dsp_pid2_init(&rf.pid_io, RF_PID_IO_KP, RF_PID_IO_KI, RF_PID_IO_KD, RF_PWM_LIMIT, 0);
	dsp_pid2_init(&rf.pid_vb, RF_PID_VB_KP, RF_PID_VB_KI, RF_PID_VB_KD, RF_PWM_LIMIT, 0);
}

/*
uint16_t rf_control(float ref)
{
	static float amp;
	float amp_io;
	float amp_vb;
	
	rf.tmr.in = (ref > 0);
	timer_process(&rf.tmr, RF_RUN_DELAY);
	rf_control_init(rf.tmr.bot);
	
	if(event(&rf.evt, RF_PID_PERIOD) ){
		if(ref > RF_REF_LIMIT) ref = RF_REF_LIMIT;
		ref *= rf.tmr.out;
		amp = dsp_pid2(&rf.pid_vo, (ref - RF_VOLT_FB));
		amp_io = dsp_pid2(&rf.pid_io, (RF_CURR_LIMIT - RF_CURR_FB) * 100);
		amp_vb = dsp_pid2(&rf.pid_vb, (RF_VBUS_LIMIT- RF_VBUS_FB));

		if(amp > amp_io) amp = amp_io;
		if(amp > amp_vb) amp = amp_vb;
		amp *= rf.tmr.out;
		rf_dirve_pwm((uint16_t) (amp * rf.tmr.in));
	}
	
	return (uint16_t) amp;
}
*/


void rf_control_process(void)
{
	rf_control_init(status_bot.run | rf.tmr.bot);
	
	if(status_lev.run){
		float ref_volt;
		status_lev.rf_en = (usr.rf_ref > 0);
		rf.tmr.in = (usr.rf_ref > 0);
		timer_process(&rf.tmr, RF_RUN_DELAY);
		rf_control_init(status_bot.run | rf.tmr.bot);
		
		if(event(&rf.evt, RF_PID_PERIOD) ){
			float amp_io;
			float amp_vb;
			ref_volt = (float) (usr.rf_ref * (RF_VOLT_LIMIT * RF_REF_PERCENTAGE));
			ref_volt *= rf.tmr.out;
			rf.error = (ref_volt - RF_VOLT_FB);
			
			rf.amp = dsp_pid2(&rf.pid_vo, rf.error);
			amp_io = dsp_pid2(&rf.pid_io, (RF_CURR_LIMIT - RF_CURR_FB) * RF_CURR_ERROR_GAIN);
			amp_vb = dsp_pid2(&rf.pid_vb, (RF_VBUS_LIMIT- RF_VBUS_FB));

			if(rf.amp > amp_io) rf.amp = amp_io;
			if(rf.amp > amp_vb) rf.amp = amp_vb;
			rf_drive_pwm((uint16_t) (rf.amp * rf.tmr.out));
		}
	}
	else {
		float pwm;
		status_lev.rf_en &= !status_bra.run; 
		pwm = usr.rf_ref * (RF_PWM_LIMIT * RF_REF_PERCENTAGE);
		rf_drive_pwm((uint16_t) (pwm * status_lev.rf_en));
	}
	
	rf_relay_drive_process();	
}


void rf_relay_drive_process(void)
{
	rf.one.in = status_bra.rf_en;
	oneshot_process(&rf.one, RF_STOP_DELAY);
	
	RF_RELAY = status_lev.rf_en | rf.one.out;
	rf_push_pull_enable(RF_RELAY);
}


void rf_drive_pwm(uint16_t pwm)
{
	if(pwm > RF_PWM_LIMIT)	pwm = RF_PWM_LIMIT;
	
	if(pwm <= BUCK_MAX){
		BUCK_PWM = pwm;
		BOOST_PWM = 0;
	}
	else {
		BUCK_PWM = (uint16_t) BUCK_MAX;
		BOOST_PWM = (uint16_t)(pwm - BUCK_MAX);
	}
}

void rf_push_pull_enable(uint16_t enable)
{
	if(enable){
		RF_PWM1 = RF_SET_PWM;
		RF_PWM2 = RF_TOP_VAL - RF_SET_PWM;
	}
	else {
		RF_PWM1 = 0;
		RF_PWM2 = RF_TOP_VAL;
	}
}
