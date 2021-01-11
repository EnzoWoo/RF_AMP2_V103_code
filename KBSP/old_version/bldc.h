/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/

#ifndef __BLDC_H__
#define __BLDC_H__

#include "ktype.h"
#include "event.h"
 
#define BLDC_CW	  0
#define BLDC_CCW	1


#define POS_VAL_OFFSET  50.0F  // [mm] 중요!!!

#define BLDC_STEP_NUM 	6.0F	 	 //스텝

#define BLDC_MG_NUM 		4.0F	 	 //자석극수 
//#define BLDC_MG_NUM 		8.0F	 	 //자석극수 
//#define BLDC_GEER_RATIO 12.0F		 //기어비
#define BLDC_GEER_RATIO 14.5F		 //기어비

#define BLDC_PULLY_PIE  49.9315F //unit: mm, BLDC_DPT / pie
#define BLDC_CIRCUMFERENCE	156.8645F // (BLDC_PULLY_PIE * 3.1459265) 	// unit: mm/turn, 폴리 원주 
#define BLDC_mmPP 			0.544668F // (BLDC_CIRCUMFERENCE/(BLDC_STEP_NUM*BLDC_MG_NUM*BLDC_GEER_RATIO)) // unit: mm/pulse

#define HALL_MODE_60		6		// 60도
#define HALL_MODE_120		12	//120도
#define HALL_MODE			HALL_MODE_60

#define HALL_IN_U			PinA.b0
#define HALL_IN_V			PinA.b1
#define HALL_IN_W			PinA.b2
#define HALL_IN_SHIFT		12		//GPIOD bit12,13,14
#define HALL_IN_PATTERN	(GPIOA->IDR & 0x7)
//#define HALL_IN_PATTERN	((GPIOD->IDR >> HALL_IN_SHIFT) & 0x7)

/* LB11620T motor controller */
#define BLDC_nRD PinE.b8
#define BLDC_nHP PinE.b11
#define BLDC_nF_R PortE.b10
#define BLDC_PWM TIM1->CCR1


#define BLDC_PWM_TOP 	TIM1->ARR  //	
/*
#define BLDC_PWM1			TIM1->CCR1
#define BLDC_PWM2			TIM1->CCR2
#define BLDC_PWM3			TIM1->CCR3

#define BLDC_TOP1		PortE.b9
#define BLDC_TOP2		PortE.b11
#define BLDC_TOP3		PortE.b13
*/

typedef struct {
	// Drive
	uint16_t ccw;	  //0:cw, 1:ccw  cw: 위치값 증가, ccw: 위치값 감소
	uint16_t dir;   //cw또는 cww의 회전방향 결정
	uint16_t pwm;   //구동량
	
	// Measurement
	// Position
	uint16_t hall_prev;
	int32_t pos_cnt;
	float pos_val;
	float	pos_ave1;
	float	pos_ave2;
	// Velocity
  evt_t ev_cap; // rpm,rpm,velocity sensing
	evt_t ev_rst; // rpm,velocity reset
	uint16_t vel_rst;
	uint32_t period;
	float	rpm_val; // turn/min
	float	rpm_ave1; // turn/min
	float	rpm_ave2; // turn/min
	float	vel_val; // mm/s
	float	vel_ave1; // mm/s
	float	vel_ave2; // mm/s
	// Torque
	evt_t ev_tor; 	//
	float	pos_pre;
	float	tor_val;  // kgf.cm
	float	tor_ave1; // kg.cm
	float	tor_ave2; // kg.cm
	
}bldc_t;

void bldc_position_init(bldc_t *bldc);
void bldc_velocity_init(bldc_t *bldc);
void bldc_torque_init(bldc_t *bldc);
void bldc_init(bldc_t *bldc);
void bldc_drive(bldc_t *bldc);
void bldc_measurement(bldc_t *bldc, float watt); // cw: 위치값 증가, ccw: 위치값 감소
//
uint16_t bldc_simulation(uint16_t ccw, uint16_t  rpm);


#endif


