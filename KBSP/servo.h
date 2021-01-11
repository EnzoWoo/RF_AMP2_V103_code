/*---------------------------------------------------------------------
by Kang Woo Jung: 2019.03.21

-----------------------------------------------------------------------*/

#ifndef __SERVO_H__
#define __SERVO_H__

#include "ktype.h"
#include "event.h"
#include "bldc.h"

extern bldc_t  bldc;

typedef enum {
	SV_CTR_TORQ = 1,
	SV_CTR_RPM,
	SV_CTR_DISP
}	SV_CM_t;


typedef struct {
	//Control Mode for Indicator
	uint16_t ctr_mode; // CTR-TORQ, CTR-RPM, CTR-DISP
	
	//Control Event
	evt_t evt_torq;
	evt_t evt_rpm;
	evt_t evt_disp;
	evt_t evt_pwm;

	evt_t evt_acc;      // V2.015 
	uint32_t acc_time;  // V2.015 
	
	//Motor Control
	uint16_t *ccw;
	uint16_t *pwm;
	uint16_t pwm_top;
	
	//Destination
	float torq_set;
	float rpm_set;
	int32_t disp_set;
	int32_t disp_set_tail; // 
	
	//Feedback
	float *fb_torq;
	float *fb_rpm;
	int32_t *fb_disp;
	
	//Error
	float torq_err;
	float rpm_err;
	int32_t disp_err;
	
	//Amplitude
	float torq_amp;
	float rpm_amp;
	float disp_amp;
	uint16_t out_pwm;

} servo_t;

void servo_setup(servo_t *servo, uint16_t *ccw, uint16_t *pwm, float *fb_torq, float *fb_rpm, int32_t *fb_disp, uint16_t pwm_top);
void servo_init(servo_t *servo);
void servo_disp_set_init(servo_t *servo, int32_t disp);
void servo_drive(servo_t *servo);

#define PER100_TO_PWM(per) (BLDC_PWM_TOP*per/100)
void servo_motor_pwm(uint16_t pwm_set);

#endif

