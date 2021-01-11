/*-----------------------------------------
by Kang Woo Jung

2016.02.26

-------------------------------------------*/

#ifndef __PHASE_SHIFT_H__
#define __PHASE_SHIFT_H__

#include "stm32f.h"  // ../KLIB/

/*------------------------------------------------------------------ Define's */
//#define PS_PWM_TOP  (1680-1)    // 100kHz
#define PS_PWM_TOP  (2800-1)    // 60kHz


/*-------------------------------------------------------------------- GPIO's */
#define PS_PWM    TIM9->CCR2


/*--------------------------------------------------------------------- VAR's */
typedef struct {
	Type_Edge Sta;
    vu16 Pwm;   ///////

}Type_Ps;
extern Type_Ps	 Ps;


/*------------------------------------------------------------------ Status's */
typedef struct {
    u32 Auto : 1;
    u32 Run : 1;
    u32 b2 : 1;
    u32 b3 : 1;
    u32 b4 : 1;
    u32 b5 : 1;
    u32 b6 : 1;
    u32 b7 : 1;

}Type_PsSta;
#define PsStaLev   (*(Type_PsSta *)(&Ps.Sta.Lev))
#define PsStaMak   (*(Type_PsSta *)(&Ps.Sta.Mak))
#define PsStaBra   (*(Type_PsSta *)(&Ps.Sta.Bra))


void Ps_Control(void);
void Ps_PwmSet(float per); //Unit [%]
void Ps_Compare_IRQ(void);


#endif


