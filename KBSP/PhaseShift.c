/*-----------------------------------------
by Kang Woo Jung

2016.02.26

-------------------------------------------*/

#include "stm32f4xx.h"
#include "Lib_KWJ.h"
#include "PhaseShift.h"

#include "main.h"

Type_Ps	 Ps;

/*
Counter_Mode : TIM_CounterMode_Up
OC1_Mode : TIM_OCMode_Toggle;
OC2_Mode : TIM_OCMode_Toggle;
TIM_IT_CC2 : ENABLE
*/

/*------------------------------------------------------------------------------*/
void Ps_Control(void)
{
    static u32 EvPs;
    static s32 pwm;

	/* Init */
	if(PsStaMak.Run){
	}

	if(PsStaBra.Run){
 	}

	
	if(!PsStaLev.Run){
		//Stop
		return;
	}

	if(PsStaLev.Auto){
		// Auto
	}
	else {
		// Manual
	}

    //Test
    if(Ev_Out(&EvPs, 10000)){
        if(++pwm > 400){
            pwm = 0;
        }
        if(pwm < 101){ // 0~100
            Ps_PwmSet(pwm);
        }
        else if(pwm < 200){ // 101~199
        }
        else if(pwm < 301){ // 200~300
            Ps_PwmSet(100-(pwm - 200));
        }
        else {
        }
    }
}


void Ps_PwmSet(float per)
{
    Ps.Pwm = (vu16) ((PS_PWM_TOP * per) / 100);
}

void Ps_Compare_IRQ(void)
{
    if(Ps.Pwm > PS_PWM_TOP) Ps.Pwm = PS_PWM_TOP;
    PS_PWM = Ps.Pwm;
    //TIM_SetCompare2(TIM9, Ps.Pwm);
}




