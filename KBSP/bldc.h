/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/

#ifndef __BLDC_H__
#define __BLDC_H__

#include "ktype.h"
#include "event.h"

// Sin Conttrol Speed
#define BLDC_SIN_CTR_START_RPM    240 // rpm
#define BLDC_SIN_CTR_END_RPM      120 // rpm

//#define BLDC_PWM_TOP  1000-1 // (80MHz/5) / 1000 = 16kHz
#define BLDC_PWM_FREQ   16000	 // Hz
#define BLDC_SETP_NUM  	6	     // 6-STEP
#define BLDC_POLE_NUM  	4	     // 4(��)
#define BLDC_PULSE_NUM	(BLDC_POLE_NUM * BLDC_SETP_NUM) // 24 = 4(��) * 6(STEP)
#define BLDC_SIN_STEP_NUM_LIMIT   (BLDC_PWM_FREQ * 60 / BLDC_PULSE_NUM / BLDC_SIN_CTR_END_RPM)

#define BLDC_PULLEY_CIRCUMFERENCE 159.6145F // mm, ���� ���� 
#define BLDC_GEAR_RATIO   14.5F     //���� 
#define BLDC_DISP_UNIT	  (BLDC_PULLEY_CIRCUMFERENCE /(BLDC_GEAR_RATIO * BLDC_PULSE_NUM) ) // 0.4587 mm 

#define HALL_IN_U			PinA.b0
#define HALL_IN_V			PinA.b1
#define HALL_IN_W			PinA.b2
#define HALL_IN_PATTERN	(GPIOA->IDR & 0x7)

#define BLDC_PWM_TOP 	TIM1->ARR
#define BLDC_PWM_MEAN	(TIM1->ARR/2)
#define BLDC_PWM1			TIM1->CCR1
#define BLDC_PWM2			TIM1->CCR2
#define BLDC_PWM3			TIM1->CCR3

typedef struct {
	uint16_t hall :1;   // 1 = motor hall pattern �̻�
	uint16_t direct :1; // 1 = motor ���� �̻� (motor ccw != hall_ccw)
	
}bldc_error_t;

typedef struct {
	uint16_t hall;
	uint16_t hall_period; // RPM PID contol interval time // motor.c
	uint16_t hall_inv;    // close = 0, left = 0 ���� ���Ͱ� �ݽð�������� ���� SET�� ��!
	uint16_t hall_ccw;	  // 0:cw, 1:ccw  cw: ��ġ�� ����, ccw: ��ġ�� ����

	//sin wave
	uint16_t pwm_step_cnt; // count by timer_1 interrupt
	uint16_t pwm_step_num; // update by timer_1 interrupt
	int16_t sin_value;
	uint16_t sin_index;

	// Drive
	uint16_t dir;  // ccw���� ������ ȸ������ ����
	uint16_t ccw;	 // 0:cw, 1:ccw  cw: ��ġ�� ����, ccw: ��ġ�� ����
	//
	uint16_t en_sin_ctr; // Enable Sine control
	uint16_t pwm1; //U pwm
	uint16_t pwm2; //V pwm
	uint16_t pwm3; //W pwm
	uint16_t pwm;  //������
	uint16_t pwm_rect;  // ������

	// TORQ
	float *torq_pt;
	float torq_val;
	float dTorq_val;  // �ʴ� TORQ�� ��ȭ dTorq
	
	// DISP
	int32_t disp_val;
	uint16_t dDisp_val;  // �ʴ� Disp�� ��ȭ dDisp

	// RPM
	//float	rpm_val;
	float	rpm_ave1;
	float	rpm_ave2;
	float	dRpm_val;  // �ʴ� RPM�� ��ȭ dRPM
	
	// Error
	bldc_error_t error;    
	
}bldc_t;


void bldc_setup(bldc_t * bldc, float * torq);
void bldc_drive_process(bldc_t *bldc);
void bldc_drive_rectangle(bldc_t * bldc);
void bldc_drive_triangle(bldc_t * bldc);
void bldc_drive_sin1(bldc_t * bldc);
void bldc_drive_sin2(bldc_t * bldc);
void bldc_get_torq(bldc_t * bldc);
void bldc_get_rpm(bldc_t * bldc);
void bldc_get_disp(bldc_t * bldc);
void bldc_get_dTorq(bldc_t * bldc);
void bldc_get_dRpm(bldc_t * bldc);
void bldc_get_dDisp(bldc_t * bldc);


uint16_t bldc_simulation(uint16_t  deg, uint16_t ccw, uint16_t  rpm);

#endif


