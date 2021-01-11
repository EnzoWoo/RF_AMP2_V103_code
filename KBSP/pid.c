/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/
#include "pid.h"
#include "usr.h"

void pid_init(pid_t *pid, float limit_min, float limit_max, float kp, float ki, float kd, float error_thresh)
{
	pid->limit_min = limit_min;
	pid->limit_max = limit_max;
	//
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
	pid->error_thresh = error_thresh;
	//
	//pid->term = 0.0f;
	//pid->started = 0;
	//pid->prev_err = 0.0f;
}

void pid_reset(pid_t *pid)
{
	pid->term = 0.0f;
	pid->started = 0;
	pid->prev_err = 0.0f;
}

float pid(pid_t *pid, float  error)
{
	float out,q;
	  
	if(fabs(error) < pid->error_thresh) q = 1;
	else q = 0;
	
	// Proportinal(비례)
	out = pid->kp * error;
	
	// Integral(적분)
	pid->term += (pid->ki * q * error);
	out += pid->term;
	
	if(pid->term > pid->limit_max) pid->term = pid->limit_max;  // kwj
	if(pid->term < pid->limit_min) pid->term = pid->limit_min;  // kwj
	
	// Differential(미분)
	if(pid->started) out += (pid->kd * (error - pid->prev_err));
	else pid->started = 1;
	pid->prev_err = error;
	
	if(out > pid->limit_max) out = pid->limit_max;  // kwj
	if(out < pid->limit_min) out = pid->limit_min;  // kwj

	return (out); //부츨력 제한!
}

void PID_init(PID_t *pid, float limit_min, float limit_max, float kp, float ki, float kd, float error_thresh, float step_time)
{
	pid->limit_min = limit_min;
	pid->limit_max = limit_max;
	//
	pid->kp = kp;
	pid->ki = ki;	
	pid->kd = kd;
	pid->error_thresh = error_thresh;
	pid->h = step_time; // 적분에 영향	
	pid->inv_h = 1/step_time; // 미분에 영향	
	//
	pid->started = 0;
	pid->integral = 0;
	pid->started = 0;
}

void PID_reset(PID_t *pid)
{
	pid->integral = 0;
	pid->started = 0;
}


float PID(PID_t *pid, float error)
{
	float q,deriv,out;
	//
	if(fabs(error) < pid->error_thresh) q = 1;
	else q = 0;
	
	//적분
	pid->integral += (pid->h * q * error);
	if(pid->integral > pid->limit_max) pid->integral = pid->limit_max;  // kwj
	if(pid->integral < pid->limit_min) pid->integral = pid->limit_min;  // kwj
	
	//미분
	if(pid->started) deriv = (error - pid->prev_error) * pid->kd * pid->inv_h;
	else {	pid->started = 1;	deriv = 0; }
	pid->prev_error = error;

	//
	out = pid->kp * (error + pid->ki * pid->integral + pid->kd * deriv);
	//out = pid->kp * error + pid->ki * pid->integral + pid->kd * deriv;

	if(out > pid->limit_max) out = pid->limit_max;  // kwj
	if(out < pid->limit_min) out = pid->limit_min;  // kwj

	return (out);
}


/*
void PID_Controller::Initialize(PID_t *pid, float kp, float ki, float kd, float error_thresh, float step_time)
{
	pid->kp = kp;
	pid->ki = ki;	
	pid->kd = kd;
	pid->error_thresh = error_thresh;
	
	pid->h =  step_time;
	pid->inv_h = 1/step_time;
	
	pid->integral = 0;
	pid->started = 0;
}


float PID_Controller::Update(PID_t *pid, float error)
{
	float q,deriv;
	
	//
	if(fabs(error) < pid->error_thresh) q = 1;
	else q = 0;
	
	//
	pid->integral += pid->h * q * error;
	
	//
	if(pid->started){
		deriv = (error - pid->prev_error) * pid->inv_h;
	}
	else {
		pid->started = 1;
		deriv = 0;
	}
	pid->prev_error = error;

	//
	return (pid->kp * (error + pid->ki * pid->integral + pid->kd * deriv));
}
*/


/*
    y[n] = y[n-1] + A0 * x[n] + A1 * x[n-1] + A2 * x[n-2]
    A0 = Kp + Ki + Kd
    A1 = (-Kp ) - (2 * Kd )
    A2 = Kd  

typedef struct
  {
    float32_t A0;          // The derived gain, A0 = Kp + Ki + Kd 
    float32_t A1;          // The derived gain, A1 = -Kp - 2Kd. 
    float32_t A2;          // The derived gain, A2 = Kd . 
    float32_t state[3];    // The state array of length 3
    float32_t Kp;               // The proportional gain. 
    float32_t Ki;               // The integral gain.
    float32_t Kd;               // The derivative gain. 
  } arm_pid_instance_f32;
*/

/*
void arm_PID_Test(void)
{
	
  q31_t i;

  arm_pid_instance_f32   pid_instance_f32;
  int32_t  resetStateFlag;
  float32_t  err; // max :1   (100%)
  float32_t  ctr;  // max :1   (100%)
  
  float32_t  ref_max;  // 제어범위 
  float32_t  ref;  // 제어목표
  float32_t  fb;   // Feedback (추종값)
  
  
   resetStateFlag=0;
   pid_instance_f32.A0=0.0f;
   pid_instance_f32.A1=0.0f;
   pid_instance_f32.A2=0.0f;
   pid_instance_f32.Kp=0.05;   // 비례: 값이 클을 수록 추종이 빠름.
   pid_instance_f32.Ki=0.6f;     // 적분: 값이 클을 수록 추종이 빠름.
   pid_instance_f32.Kd=0.05f;   // 미분: 값이 클을 수록 추종이 빠름.
   arm_pid_init_f32(&pid_instance_f32, resetStateFlag);
//   arm_pid_reset_f32(&pid_instance_f32);

   ref_max = 100;
   ref=100;

   for(i=0 ; i<30 ; i++){
      err =  (ref-fb)/ref_max; //100분율
      if(err>1) err=1;       //max 100% 
     
      PRINTF2("No=%0.3d, Ref=%0.4f,   FB=%0.4f,   ERR=%0.4f,",  i,  ref,  fb,  err);
     
      ctr = arm_pid_f32(&pid_instance_f32, err);
      if(ctr>1) ctr=1;         //max 100% 
      fb=(ctr*ref_max);
      PRINTF2("   CTR=%0.4f \n\r", ctr);
//      DelayMs(10);

   }
   
    ref=0;
    for(i=0 ; i<30 ; i++){
      err =  (ref-fb)/ref_max; //100분율
      if(err>1) err=1;       //max 100% 
     
      PRINTF2("No=%0.3d, Ref=%0.4f,   FB=%0.4f,   ERR=%0.4f,",  i,  ref,  fb,  err);
     
      ctr = arm_pid_f32(&pid_instance_f32, err);
      fb=(ctr*ref_max);
      PRINTF2("   CTR=%0.4f \n\r", ctr);
//      DelayMs(10);
   }
 }
 
*/   

 /*  

 void arm_DSP_test(void)
 {	
   float  flo[1];
   s32	qm31[1];
   u32	blockSize=1;
 
   float32_t  flo[1];
   q31_t  qm31[1];
   uint32_t blockSize=1;
 
	flo[0] = -0.12345678;
	qm31[0] = 0;
	blockSize=1;
	arm_float_to_q31(flo, qm31, blockSize);
	PRINTF2("f:%f, q31:0x%x\n\r", flo[0],qm31[0]);
	arm_q31_to_float(qm31, flo, blockSize);
	PRINTF2("f:%f, q31:0x%x\n\r", flo[0],qm31[0]);
 }
 
 */

