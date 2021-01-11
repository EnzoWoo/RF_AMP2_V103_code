
/*
FILE_NAME : StepMotor.c
Kang Woojung
Update : 2014.01.06
*/

#include "SYS.h"
#include "UserLIB.h"
//#include "User.h"



type_sm5p  sm5p;



#define SM5P_FULL_STEP_MAX  10
const u16 SM5P_FULL_STEP[SM5P_FULL_STEP_MAX]={ // 0.72도, 500 Step / 360도
	0x0181,0x0103,0x0302,0x0206,0x0224,
	0x002c,0x0068,0x0058,0x00d0,0x0091
};

#define SM5P_HALF_STEP_MAX  20
const u16 SM5P_HALF_STEP[SM5P_HALF_STEP_MAX]={ // 0.36도, 1000 Step / 360도
	0x0181,0x0101,0x0103,0x0102,0x0302,
	0x0202,0x0206,0x0204,0x0224,0x0024,
	0x002c,0x0028,0x0068,0x0048,0x0058,
	0x0050,0x00d0,0x0090,0x0091,0x0081
};


void StepMot5p_Init(void)
{
	sm5p.b.mode = SM5P_FULL_STEP_MODE;
	sm5p.b.direction = SMOT_DIR_CW;
	sm5p.b.run = OFF;	
	//
	sm5p.speed = SMOT_SPEED_MAX;
	sm5p.pattern_step = 0;
	sm5p.step = 0;
	sm5p.set_step = 0;
	//
	SM5P_ALL_LOW();
	DelayMs(50);
	StepMot5p_PatternOut(sm5p.pattern_step);
	DelayMs(50);
	SM5P_ALL_OFF();
}



void StepMot5p_Drive(void) // call by interrupt
{
	static s32 count;
	u16 pattern;

	if(++count < (SMOT_SPEED_MAX-sm5p.speed)) return;
	count=0;
		
	if(!sm5p.b.run){
//		SM5P_ALL_OFF();
		return;
	}
	
	if(sm5p.step == sm5p.set_step){
		sm5p.b.run = OFF;
//		SM5P_ALL_LOW();
		return;
	}
	
	/* FULL_STEP MODE */
	if(sm5p.b.mode == SM5P_FULL_STEP_MODE){
		
		if(sm5p.b.direction == SMOT_DIR_CW){
			if(++sm5p.pattern_step>=SM5P_FULL_STEP_MAX){
				sm5p.pattern_step=0;
			}
			sm5p.step++;
		}
		else {
			if(sm5p.pattern_step){ sm5p.pattern_step--; }
			else { sm5p.pattern_step=SM5P_FULL_STEP_MAX-1; }
			sm5p.step--;
		}
		pattern = SM5P_FULL_STEP[sm5p.pattern_step];
	}
	/* HALF_STEP MODE */
	else {
		if(sm5p.b.direction == SMOT_DIR_CW){
			if(++sm5p.pattern_step>=SM5P_HALF_STEP_MAX){
				sm5p.pattern_step=0;
			}
			sm5p.step++;
		}
		else {
			if(sm5p.pattern_step){ sm5p.pattern_step--; }
			else { sm5p.pattern_step=SM5P_HALF_STEP_MAX-1; }
			sm5p.step--;
		}
		pattern = SM5P_HALF_STEP[sm5p.pattern_step];
	}
	/* Pattern Output */
	MOTOR.output=pattern;
}


void StepMot5p_PatternOut(u8 pattern_step)
{
	u16 pattern;
	
	/* FULL_STEP MODE */
	if(sm5p.b.mode == SM5P_FULL_STEP_MODE){
		if(pattern_step>=SM5P_FULL_STEP_MAX){
			pattern_step=0;
		}
		pattern = SM5P_FULL_STEP[pattern_step];
	}
	/* HALF_STEP MODE */
	else {
		if(pattern_step>=SM5P_HALF_STEP_MAX){
			pattern_step=0;
		}
		pattern = SM5P_HALF_STEP[pattern_step];
	}
	/* Pattern Output */	
	MOTOR.output=pattern;
}


void StepMot5p_Run(u8 dir, s32 set_step)
{
	sm5p.set_step = sm5p.step+set_step;
	sm5p.b.run = ON;
}

void StepMot5p_Stop(void)
{
	sm5p.b.run = OFF;
//	sm5p.set_step = sm5p.step;
}

void StepMot5p_Mode(u8 mode)
{
	sm5p.b.mode = mode;
}

void StepMot5p_SpeedSet(u8 speed)
{
	sm5p.speed = speed;
}

void StepMot5p_SpeedUp(void)
{
	if(++sm5p.speed > SMOT_SPEED_MAX){
		sm5p.speed = SMOT_SPEED_MAX;
	}
}

void StepMot5p_SpeedDown(void)
{
	if(sm5p.speed){
		sm5p.speed--;
	}
}

void StepMot5p_ResetStep(void)
{
	sm5p.step = 0;
	sm5p.set_step = 0;
}

s32 StepMot5p_GetStep(void)
{
	return sm5p.step;
}






