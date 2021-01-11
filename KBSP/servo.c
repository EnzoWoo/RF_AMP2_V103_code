/*---------------------------------------------------------------------
by Kang Woo Jung: 2019.03.21

-----------------------------------------------------------------------*/
#include "bsp.h"
#include "usr.h"
#include "servo.h"

//Error Gain CAL
#define SV_AMP_LIMIT    servo->pwm_top  // pwm limit

/* 25W */
#define TORQ_MAX  5.0F
#define RPM_MAX   4000.0F
#define DISP_MAX  4000.0F
#define TORQ_ERR_Gain (SV_AMP_LIMIT/TORQ_MAX)
#define RPM_ERR_Gain  (SV_AMP_LIMIT/RPM_MAX)
#define DISP_ERR_Gain (SV_AMP_LIMIT/DISP_MAX)

//pid TORQ
#define TORQ_pid_KP		4.0F
#define TORQ_pid_KI		0.01F
#define TORQ_pid_KD		4.0F
#define TORQ_pid_ERROR_THRESH (TORQ_ERR_Gain * 0.5F)

//pid RPM
#define RPM_pid_KP		5.0F
#define RPM_pid_KI		0.001F
#define RPM_pid_KD		2.0F
#define RPM_pid_ERROR_THRESH  (RPM_ERR_Gain * 1500.0F)
#define RPM_PID_LIMIT_MAX_GAIN   0.34F  ////////////////////////////////// very very important!!!!!!!!! 

//PID DISP
#define DISP_pid_KP		3.5F
#define DISP_pid_KI		0.022F
#define DISP_pid_KD		3.5F
#define DISP_pid_ERROR_THRESH (DISP_ERR_Gain * 1500.0F)
#define DISP_pid_STEP_TIME 1.0F
//

/* 60W */
#define TORQ_MAX_60W  5.0F
#define RPM_MAX_60W   4000.0F
#define DISP_MAX_60W  4000.0F
#define TORQ_ERR_Gain_60W (SV_AMP_LIMIT/TORQ_MAX_60W)
#define RPM_ERR_Gain_60W  (SV_AMP_LIMIT/RPM_MAX_60W)
#define DISP_ERR_Gain_60W (SV_AMP_LIMIT/DISP_MAX_60W)

//pid TORQ
#define TORQ_pid_KP_60W		4.0F
#define TORQ_pid_KI_60W		0.01F
#define TORQ_pid_KD_60W		4.0F
#define TORQ_pid_ERROR_THRESH_60W (TORQ_ERR_Gain_60W * 0.5F)

//pid RPM
#define RPM_pid_KP_60W		5.0F
#define RPM_pid_KI_60W		0.001F
#define RPM_pid_KD_60W		2.0F
#define RPM_pid_ERROR_THRESH_60W  (RPM_ERR_Gain_60W * 1500.0F)
#define RPM_PID_LIMIT_MAX_GAIN_60W   0.34F  ////////////////////////////////// very very important!!!!!!!!! 

//PID DISP
#define DISP_pid_KP_60W		3.5F
#define DISP_pid_KI_60W		0.022F
#define DISP_pid_KD_60W		3.5F
#define DISP_pid_ERROR_THRESH_60W (DISP_ERR_Gain_60W * 1500.0F)
#define DISP_pid_STEP_TIME_60W 1.0F
//


pid_t	  torq_pid;
pid_t	  rpm_pid;
PID_t   disp_PID;
bldc_t 	bldc;


void servo_setup(servo_t *servo, uint16_t *ccw, uint16_t *pwm, float *fb_torq, float *fb_rpm, int32_t *fb_disp, uint16_t pwm_top)
{
	servo->ccw = ccw; //pointer
	servo->pwm = pwm; //pointer
	servo->pwm_top = pwm_top; //value
	servo->fb_torq = fb_torq; //pointer
	servo->fb_rpm = fb_rpm; //pointer
	servo->fb_disp = fb_disp; //pointer
}

void servo_init(servo_t *servo)
{
	servo->torq_set =0;
	servo->rpm_set =0;
	servo_disp_set_init(servo,0);
	
	//ERR
	servo->torq_err = 0;
	servo->rpm_err = 0;
	servo->disp_err = 0;
	//AMP
	servo->torq_amp = 0;
	servo->rpm_amp = 0;
	servo->disp_amp = 0;

	if(para.motor_kind == 2){
		// PID 25W
		pid_init(&torq_pid, 0.0F, SV_AMP_LIMIT, TORQ_pid_KP, TORQ_pid_KI, TORQ_pid_KD, TORQ_pid_ERROR_THRESH); //Troque
		pid_init(&rpm_pid, 0.0F, SV_AMP_LIMIT, RPM_pid_KP, RPM_pid_KI, RPM_pid_KD, RPM_pid_ERROR_THRESH); //Velocity
		PID_init(&disp_PID, -SV_AMP_LIMIT, SV_AMP_LIMIT, DISP_pid_KP, DISP_pid_KI, DISP_pid_KD, DISP_pid_ERROR_THRESH,DISP_pid_STEP_TIME); //Displacement
	}
	else if(para.motor_kind == 1){
		// PID 60W
		pid_init(&torq_pid, 0.0F, SV_AMP_LIMIT, TORQ_pid_KP_60W, TORQ_pid_KI_60W, TORQ_pid_KD_60W, TORQ_pid_ERROR_THRESH_60W); //Troque
		pid_init(&rpm_pid, 0.0F, SV_AMP_LIMIT, RPM_pid_KP_60W, RPM_pid_KI_60W, RPM_pid_KD_60W, RPM_pid_ERROR_THRESH_60W); //Velocity
		PID_init(&disp_PID, -SV_AMP_LIMIT, SV_AMP_LIMIT, DISP_pid_KP_60W, DISP_pid_KI_60W, DISP_pid_KD_60W, DISP_pid_ERROR_THRESH_60W,DISP_pid_STEP_TIME_60W); //Displacement
	}
	else {
		// PID 25W
		pid_init(&torq_pid, 0.0F, SV_AMP_LIMIT, TORQ_pid_KP, TORQ_pid_KI, TORQ_pid_KD, TORQ_pid_ERROR_THRESH); //Troque
		pid_init(&rpm_pid, 0.0F, SV_AMP_LIMIT, RPM_pid_KP, RPM_pid_KI, RPM_pid_KD, RPM_pid_ERROR_THRESH); //Velocity
		PID_init(&disp_PID, -SV_AMP_LIMIT, SV_AMP_LIMIT, DISP_pid_KP, DISP_pid_KI, DISP_pid_KD, DISP_pid_ERROR_THRESH,DISP_pid_STEP_TIME); //Displacement
	}
}


void servo_disp_set_init(servo_t *servo, int32_t disp)
{
	servo->disp_set = disp;
	servo->disp_set_tail = disp;
}

void servo_drive(servo_t *servo)
{
	//TORQ
 	if(evt_us(&servo->evt_torq, 200)){
		if(servo->torq_set == 0){
			pid_reset(&torq_pid);
			servo->torq_amp = 0;
		}
		else {
			servo->torq_err = (servo->torq_set - *servo->fb_torq) * TORQ_ERR_Gain;
			servo->torq_amp = pid(&torq_pid, servo->torq_err);
		}
	}
	
	//RPM
 	//if(evt_us(&servo->evt_rpm, bldc.hall_period)){
 	if(evt_us(&servo->evt_rpm, 1000)){
		if(servo->rpm_set == 0){
			pid_reset(&rpm_pid);
			servo->rpm_amp = 0;
		}
		else {
			//#define RPM_LIMIT_GAIN  ((999.9F-150.0F)/(2600.0F-400.0F))
			//#define RPM_LIMIT_OFS   (999.0F-(2600 * RPM_LIMIT_GAIN))
			//rpm_pid.limit_max = servo->rpm_set * RPM_LIMIT_GAIN + RPM_LIMIT_OFS;
			rpm_pid.limit_max = servo->rpm_set * RPM_PID_LIMIT_MAX_GAIN;
			//rpm_pid.limit_max = 999.0F; /////////////////////////////////
			servo->rpm_err = (servo->rpm_set - *servo->fb_rpm) * RPM_ERR_Gain;
			servo->rpm_amp = pid(&rpm_pid, servo->rpm_err);
		}
	}
	
	
	// *Soft start (disp_set --> disp_set_tail) //V2.015
	if(*servo->fb_rpm < 100) servo->acc_time = 600;
 	if(evt_us(&servo->evt_acc, servo->acc_time)){
		if(servo->disp_set > servo->disp_set_tail) servo->disp_set_tail++;
		if(servo->disp_set < servo->disp_set_tail) servo->disp_set_tail--;
		// acc
		if(--servo->acc_time < 200) servo->acc_time = 200; // acc limit
	}
	
	//DISP
 	if(evt_us(&servo->evt_disp, 1000)){
		if(*servo->fb_disp <= servo->disp_set_tail+1 && *servo->fb_disp >= servo->disp_set_tail-1){
			PID_reset(&disp_PID);
			servo->disp_amp = 0;
		}
		else {
			servo->disp_err = (int32_t) ((servo->disp_set_tail - *servo->fb_disp) * DISP_ERR_Gain);
			servo->disp_amp = PID(&disp_PID, servo->disp_err);
			*servo->ccw = (servo->disp_amp < 0.0F); //motor direction
			servo->disp_amp = fabs(servo->disp_amp);  //절대값
		}
	}
	
	//Combine
	float amp_val;
	amp_val = servo->torq_amp;
	if(servo->rpm_amp < amp_val) amp_val = servo->rpm_amp;
	if(servo->disp_amp < amp_val) amp_val = servo->disp_amp;
	servo->out_pwm = (uint16_t) amp_val;
	
	// MOTOR control output
	if(servo->out_pwm > servo->pwm_top) servo->out_pwm = servo->pwm_top;
	*servo->pwm = servo->out_pwm;
	
	//Control Mode for Indicator
	if(servo->rpm_amp > servo->torq_amp){
	  if(servo->disp_amp > servo->torq_amp) servo->ctr_mode = SV_CTR_TORQ;
		else  servo->ctr_mode = SV_CTR_DISP;
	}
	else if(servo->disp_amp > servo->rpm_amp)  servo->ctr_mode = SV_CTR_RPM;
	else  servo->ctr_mode = SV_CTR_DISP;	
}


extern bldc_t 	bldc;
void servo_motor_pwm(uint16_t pwm_set)
{
	static uint16_t cnt, pwm;
	//정적분 
	if(++cnt > 5){
		cnt=0;
		if(pwm < pwm_set)	pwm++;
		else if(pwm > pwm_set) pwm--;
	}
	bldc.pwm = pwm;
}



