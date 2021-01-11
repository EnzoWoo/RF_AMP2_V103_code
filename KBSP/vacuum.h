
/*-----------------------------------------
by Kang Woo Jung

2017.03.13

-------------------------------------------*/

#ifndef __VACUUM_H__
#define __VACUUM_H__

#ifdef __cplusplus
 extern "C" {
#endif

	 
#define VAC_PWM					TIM10->CCR1
#define VAC_PWM_TOP			TIM10->ARR

#define VAC_PWM_TOP_VAL	(2160-1)
#define VAC_PWM_LIMIT			(VAC_PWM_TOP_VAL * 0.56F)
#define VAC_PWM_OFFSET		(VAC_PWM_TOP_VAL * 0.25F)
	 
#define VAC_RUN_DELAY		(MSEC_VAL * 1000)	//ms
#define VAC_PID_PERIOD	(MSEC_VAL * 2)		// ms
	 
#define VAC_PID_PR_KP	4.5F
#define VAC_PID_PR_KI	0.35F
#define VAC_PID_PR_KD	6.5F

//#define VAC_PRESS_FB		an.val[VACUUM]
#define VAC_PRESS_FB		an.ave1[VACUUM]
//#define VAC_PRESS_FB		an.rms_vacuum.mean


//#define VAC_PRESS_LIMIT	23.0F	// [kPa]
#define VAC_PRESS_LIMIT	33.0F	// [kPa]

#define VAC_REF_PERCENTAGE		0.01F
#define VAC_PRESS_ERROR_GAIN	1.0F

typedef struct {
	evt_t	evt;
	tmr_t tmr;
	pid_t pid_pr;		// vacuum pressure
	
	float	error;
	float	amp;
}vac_t;
extern vac_t 	vac;


void vac_control_init(uint16_t set);
void vac_control_process(void);
void vac_drive_pwm(uint16_t pwm);



#ifdef __cplusplus
}
#endif


#endif


