/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/

#ifndef __PID_H__
#define __PID_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "ktype.h"

#define ERROR_PER_AMPLITUDE	0.5F
	 
typedef struct{
	float limit_min; // kwj
	float limit_max; // kwj
	//
	float kp; 
	float ki; 
	float kd; 
	//
	uint16_t started;
	float prev_err;
	//
	float term;
	float error_thresh; // error가 error_thresh보다 작을 때 적분연산 적용

}pid_t;
void pid_init(pid_t *pid, float limit_min, float limit_max, float kp, float ki, float kd, float error_thresh);
void pid_reset(pid_t *pid);
float pid(pid_t *pid, float  err);

typedef struct{
	float limit_min; // kwj
	float limit_max; // kwj
	//
	float kp;
	float ki;
	float kd;
	//
	float inv_h; // 미분에 영향	
	uint16_t started;
	float prev_error;
	//
	float h; // 적분에 영향	
	float integral;
	float error_thresh; // error가 error_thresh보다 작을 때 적분연산 적용
} PID_t;
void PID_init(PID_t *pid, float limit_min, float limit_max, float kp, float ki, float kd, float error_thresh, float step_time);
void PID_reset(PID_t *pid);
float PID(PID_t *pid, float error);


/*
class PID_Controller {
public:
	void Initialize(float kp, float ki, float kd, float error_thresh, float step_time);
	float Update(float error);

private:
	bool started;
	float kp;
	float ki;
	float kd;
	float h;
	float inv_h;
	float prev_error;
	float error_thresh;
	float integral;
};
*/


/*
void arm_PID_Test(void);
void arm_DSP_test(void);
*/


#ifdef __cplusplus
}
#endif


#endif


