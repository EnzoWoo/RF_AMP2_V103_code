
/*
FILE_NAME : StepMotor.h
Kang Woojung
Update : 2014.01.06
*/


#ifndef __STEP_MOTOR_H__
#define __STEP_MOTOR_H__

#include "stm32f.h"  // ../KLIB/


//---------------------------------------------------- Define
#define ANGLE_UNIT_FULL 0.072f
#define ANGLE_UNIT_HALF 0.036f

#define SM5P_FULL_STEP_MODE  0
#define SM5P_HALF_STEP_MODE  1

#define SMOT_DIR_CW   0
#define SMOT_DIR_CCW  1

#define SMOT_SPEED_MAX  100

//---------------------------------------------------- GPIO
/*
#define MHA1_OUT   PORTA.bit.b0
#define MHA2_OUT   PORTA.bit.b1
#define MHA3_OUT   PORTA.bit.b2
#define MHA4_OUT   PORTA.bit.b3
#define MHA5_OUT   PORTA.bit.b4
#define MLA1_OUT   PORTA.bit.b5
#define MLA2_OUT   PORTA.bit.b6
#define MLA3_OUT   PORTA.bit.b7
#define MLA4_OUT   PORTA.bit.b8
#define MLA5_OUT   PORTA.bit.b9
*/
typedef struct {
  __IO uint16_t  	output : 10;
  __IO uint16_t  	b10 : 1;
  __IO uint16_t  	b11 : 1;
  __IO uint16_t  	b12 : 1;
  __IO uint16_t  	b13 : 1;
  __IO uint16_t  	b14 : 1;
  __IO uint16_t  	b15 : 1;
} MOT_TypeDef;
#define MOTOR		(*(MOT_TypeDef*)(&GPIOA->ODR))

#define SM5P_ALL_HIGH() { GPIOA->ODR &= 0xfc00; GPIOA->ODR |= 0x001f; }
#define SM5P_ALL_LOW() { GPIOA->ODR &= 0xfc00; GPIOA->ODR |= 0x03e0; }
#define SM5P_ALL_OFF() { GPIOA->ODR &= 0xfc00; }


typedef struct {
	struct {
		u8 mode :1; // 0=Full(0.72deg), 1=Half(0.36deg)
		u8 direction :1;  // 0=CW, 1=CCW
		u8 run :1;
	}b;
	u16 speed; // max 100
	u8 pattern_step;
	s32 step;
	s32 set_step;
}type_sm5p;
extern type_sm5p  sm5p;


void StepMot5p_Init(void);
void StepMot5p_Drive(void); // call by interrupt
void StepMot5p_PatternOut(u8 pattern_step);

void StepMot5p_Run(u8 dir, s32 set_step);
void StepMot5p_Stop(void);
void StepMot5p_Mode(u8 mode);
void StepMot5p_SpeedSet(u8 speed);
void StepMot5p_SpeedUp(void);
void StepMot5p_SpeedDown(void);
void StepMot5p_ResetStep(void);
s32 StepMot5p_GetStep(void);


#endif
