/*-----------------------------------------
by Kang Woo Jung

2016.12. 27

-------------------------------------------*/

#ifndef __PFC_H__
#define __PFC_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f.h"  // ../KLIB/

#define PFC_PWM_TOP_VAL	(10800.0F - 1.0F)		// ARR 20kHz = 216MHz / 10800
#define PFC_PWM				TIM8->CCR1
#define PFC_PWM_TOP		TIM8->ARR

#define PFC_REF_MAX			400.0F
#define PFC_REF_MIN			311.0F
	 
#define PR_VOLT_MIN			24.0F

#define PFC_PWM_DUTY_MAX 	0.68F
#define PFC_SINE_MIX_FACTOR 	0.72F // 중요!! 범위 0.0F ~ 1.0F

#define PFC_PID_KP	1.0F
#define PFC_PID_KI	 	0.05F
#define PFC_PID_KD	1.0F



void __TIM8_IRQ(void);
float pfc_control(float pr_volt_rms, float pr_volt, float dc_bus, float ref);
	 
	 
	 
	 
#ifdef __cplusplus
}
#endif

#endif


