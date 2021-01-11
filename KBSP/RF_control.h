/*-----------------------------------------
by Kang Woo Jung

2017.03.10

-------------------------------------------*/

#ifndef __RF_H__
#define __RF_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define RF_PWM1			TIM3->CCR1		// PWM MODE1
#define RF_PWM2			TIM3->CCR2		// PWM MODE2
#define RF_TOP				TIM3->ARR
#define RF_TOP_VAL		(69-1)		// FREQ = 782.609KHz		:	108MHz/(69*2)  up_down count
#define RF_DT_VAL			17			// DT = 315ns
#define RF_SET_PWM		((RF_TOP_VAL / 2) - RF_DT_VAL)
	 
#define BUCK_PWM			TIM8->CCR3
#define BOOST_PWM		TIM8->CCR4
#define BUCK_TOP      	TIM8->ARR
#define BUCK_TOP_VAL  	(2160-1)	// FREQ = 100kHz
#define BUCK_DT_VAL	 	(108-1)		// 216MHz * 500nsDT

#define BUCK_MAX 					(BUCK_TOP_VAL - (BUCK_DT_VAL * 2))
#define BUCK_BOOST_MAX   	(BUCK_MAX * 1.5F)	
#define RF_PWM_LIMIT			BUCK_BOOST_MAX

#define RF_RUN_DELAY	(MSEC_VAL * 1000)	//ms
#define RF_STOP_DELAY	(MSEC_VAL * 1500)	//ms
#define RF_PID_PERIOD	100		// us

#define RF_PID_VO_KP	3.5F
#define RF_PID_VO_KI		0.2F
#define RF_PID_VO_KD	5.0F

#define RF_PID_IO_KP		3.5F
#define RF_PID_IO_KI		0.2F
#define RF_PID_IO_KD		5.0F

#define RF_PID_VB_KP	3.5F
#define RF_PID_VB_KI		0.2F
#define RF_PID_VB_KD	5.0F

#define RF_VOLT_FB		an.val[RF_VOLT]
#define RF_CURR_FB		an.val[RF_CURR]
#define RF_VBUS_FB		an.val[RF_VBUS]

#define RF_VOLT_LIMIT	70.0F	// [V]
#define RF_CURR_LIMIT	0.35F	// [A]
#define RF_VBUS_LIMIT	30.0F	// [V]

#define RF_REF_PERCENTAGE	0.01F

#define RF_VOLT_ERROR_GAIN	0.4F
#define RF_CURR_ERROR_GAIN	100.0F
#define RF_VBUS_ERROR_GAIN	1.0F


typedef struct {
	evt_t	evt;
	tmr_t tmr;
	one_t one;	
	pid_t pid_vo;		// rf volt
	pid_t pid_io;		// rf current
	pid_t pid_vb;		// vbus volt
	float	error;
	float	amp;	
	
}rf_t;
extern rf_t 	rf;

void rf_control_init(uint16_t set);
//uint16_t rf_control(float ref);
void rf_control_process(void);
void rf_relay_drive_process(void);
void rf_drive_pwm(uint16_t pwm);
void rf_push_pull_enable(uint16_t enable);


#ifdef __cplusplus
}
#endif


#endif
