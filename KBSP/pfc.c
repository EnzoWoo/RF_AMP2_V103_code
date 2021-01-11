/*-----------------------------------------
by Kang Woo Jung

2016.12. 27

-------------------------------------------*/

#include "kbsp.h"
#include "pfc.h"

#include "user.h"


Type_PID   pid_pfc;


void __TIM8_IRQ(void)
{
}



float pfc_control(float pr_volt_rms, float pr_volt, float dc_bus, float ref)
{
	static float pr_volt_pp;
	static float pr_sin;
	static float amp;	

	
	//ref limit
	if(ref > PFC_REF_MAX) ref = PFC_REF_MAX;
	pr_volt_pp = pr_volt_rms * 1.4142F;

	static uint32_t evt;
	if(evt_out(&evt, 100)){

		TP13 = 1; ////////////////////////////////////////////////////////////
	
		// PID Initialization
		static float prev;
		if(prev != ref){
			if(prev == 0.0F){
				Dsp_PID_Init(&pid_pfc, PFC_PID_KP, PFC_PID_KI, PFC_PID_KD, (PFC_PWM_TOP_VAL * PFC_PWM_DUTY_MAX));
			}
			prev = ref;
		}
			
		//ref limit
		if(ref > PFC_REF_MAX) ref = PFC_REF_MAX;
		
		// PID
		amp = Dsp_PID(&pid_pfc, (ref - dc_bus));
		if(amp > (PFC_PWM_TOP_VAL * PFC_PWM_DUTY_MAX)) amp = (PFC_PWM_TOP_VAL * PFC_PWM_DUTY_MAX);
		
		// PR volt area for PFC control
		if(ref > PFC_REF_MIN && (pr_volt >= PR_VOLT_MIN)){
			// pr_volt limit
			if(pr_volt > pr_volt_pp) pr_volt = pr_volt_pp;
			if(pr_volt < PR_VOLT_MIN) pr_volt = PR_VOLT_MIN;
			
			// cal sine gain 
			pr_sin = pr_volt_pp - pr_volt; // wave invert
			pr_sin /= (pr_volt_pp - PR_VOLT_MIN); // 0 ~ 1 값으로 치환
			uint16_t pwm;
			pwm = (uint16_t)((amp * (1 - PFC_SINE_MIX_FACTOR)) + (amp * pr_sin * PFC_SINE_MIX_FACTOR)); // Amp & Sin --> Mix, Combin
			
			// pwm limit
			if(pwm > (PFC_PWM_TOP_VAL * PFC_PWM_DUTY_MAX)) pwm = (PFC_PWM_TOP_VAL * PFC_PWM_DUTY_MAX);
			PFC_PWM = pwm;
		}
		// Stop
		else {
			PFC_PWM = 0;
		}
		
		TP13 ^= 1; ////////////////////////////////////////////////////////////		
	
	}

	return  amp;
}




