
/*-----------------------------------------
by Kang Woo Jung

2017.03.13

-------------------------------------------*/

#include "user.h"
#include "vacuum.h"


const float Vacuum_Level_Tbl[11]={
	0.0F, 1.5F, 3.0F, 4.5F, 6.0F, 7.5F, 9.0F, 10.5F, 12.0F, 13.5F, 15.0F
}; // kPa


vac_t 		vac;

void vac_control_init(uint16_t set)
{
	if(!set) return; 
	//dsp_pid2_init(&vac.pid_pr, VAC_PID_PR_KP, VAC_PID_PR_KI, VAC_PID_PR_KD, VAC_PWM_TOP_VAL, 0);
	dsp_pid2_init(&vac.pid_pr, VAC_PID_PR_KP, VAC_PID_PR_KI, VAC_PID_PR_KD, VAC_PWM_LIMIT, 0);
}


void vac_control_process(void)
{
	if(VAC_PRESS_FB < 0 ) VAC_PRESS_FB = 0;
	vac_control_init(status_bot.run);
	
	if(status_lev.run){
		status_lev.vac_en = (usr.vac_ref > 0);
		vac_control_init(status_bot.vac_en);
		
		//if(event(&vac.evt, VAC_PID_PERIOD) ){
			float ref_press;
			ref_press = (float) (usr.vac_ref * (VAC_PRESS_LIMIT * VAC_REF_PERCENTAGE));
			//ref_press *= status_lev.vac_en;
			vac.error = (ref_press - VAC_PRESS_FB) * VAC_PRESS_ERROR_GAIN;
			//vac.amp = dsp_pid2(&vac.pid_pr, vac.error);
			vac_drive_pwm((uint16_t) ((vac.amp+VAC_PWM_OFFSET) * status_lev.vac_en));
			//vac_drive_pwm((uint16_t) (vac.amp * status_lev.vac_en));
		//}
	}
	else {
		vac.error = 0;
		
		float pwm;
		status_lev.vac_en &= !status_bra.run; 
		pwm = usr.vac_ref * (VAC_PWM_LIMIT * VAC_REF_PERCENTAGE);
		vac_drive_pwm((uint16_t) (pwm * status_lev.vac_en));
	}

	if(event(&vac.evt, VAC_PID_PERIOD) ){
		vac.amp = dsp_pid2(&vac.pid_pr, vac.error);
	}
}


void vac_drive_pwm(uint16_t pwm)
{
	if(pwm > VAC_PWM_LIMIT)	pwm = VAC_PWM_LIMIT;
	VAC_PWM = pwm;
}


