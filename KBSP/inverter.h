/*---------------------------------------------------------------------
by Kang Woo Jung: 2019.02.18

-----------------------------------------------------------------------*/

#ifndef __INVERTER_H__
#define __INVERTER_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "ktype.h"


#define MOT_RPM_MAX		1800.0F	// inv 60Hz --> motor
#define MOT_RPM_MIN		60.0F		// inv 2Hz --> motor
#define MOT_FREQ_TOP		(1800.0F*2/60.0F)	 

#define INV_RPM_KI_PERIOD	10000		// us  	 
#define INV_RPM_KI					0.025F
	 
// motor speed 연산 시 사용
#define EVENT_TIME_REF	(300000.0F-1.0F)	// 300000 = (1MHz/6kHz)*1800 // 1800rpm : 6kHz(166.7us)
	 
#define MOT_VOLT_MAX		311.1F
#define SINE_STEP_MAX 	SIN_TBL_SIZ

	 
#define INV_PWM_TOP_VAL	(36000.0F-1.0f)	 // ARR = 36000 = 216MHz / 6kHz 
#define INV_PWM_TOP		TIM1->ARR
#define INV_PWM12				TIM1->CCR1
#define INV_PWM34				TIM1->CCR2

#define INV_PWM_GAIN	 	(INV_PWM_TOP_VAL/MOT_RPM_MAX)
#define INV_PWM_MAX		(MOT_RPM_MAX*INV_PWM_GAIN-1.0F)   // 1800rmp * 20 - 1
#define INV_PWM_MIN			(MOT_RPM_MIN*INV_PWM_GAIN-1.0F) 	// 60rpm * 20 - 1

 
 typedef struct {
	float rpm; 		// speed
	float amp;		// amplitude
	 
}Type_Inv;
 
 
float  inverter_control(float dc_bus, float ref);


#ifdef __cplusplus
}
#endif

#endif

