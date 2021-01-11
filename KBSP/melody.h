
/*-----------------------------------------
by Kang Woo Jung

2014.09.22

-------------------------------------------*/

#ifndef __MELODY_H__
#define __MELODY_H__

#include "stm32f.h"  // ../KLIB/


#define MELODY_TIM_SOURCE_CLK	2100000L

#define Melody_PwmOff() { TIM_SetCompare3(TIM2, 0x0); }
#define Melody_PwmOn(Period)  { TIM_SetAutoreload(TIM2,Period); TIM_SetCompare3(TIM2,Period/2); }


/*  MELODY Format :

u8 MELODY_BUF[]={
	VOICE,LENGTH,
	.....
	VOICE,LENGTH,
	......
	VOICE_END
};

u8 MELODY_BUF[]={
  C4,M8,
  D4,M8,
  E4,M8,
  F4,M8,
  G4,M8,
  A4,M8,
  B4,M8,
  C5,M8,
  MUT,M8,
  
  C5,M4,
  D5,M4,
  E5,M4,
  F5,M4,
  G5,M4,
  A5,M4,
  B5,M4,
  C6,M4,
  VOICE_END
};
*/


enum VOICE{
  MUT=0x00,
  C1,C1S,D1,D1S,E1,F1,F1S,G1,G1S,A1,A1S,B1,
  C2,C2S,D2,D2S,E2,F2,F2S,G2,G2S,A2,A2S,B2,
  C3,C3S,D3,D3S,E3,F3,F3S,G3,G3S,A3,A3S,B3,
  C4,C4S,D4,D4S,E4,F4,F4S,G4,G4S,A4,A4S,B4,
  C5,C5S,D5,D5S,E5,F5,F5S,G5,G5S,A5,A5S,B5,
  C6,C6S,D6,D6S,E6,F6,F6S,G6,G6S,A6,A6S,B6,
  C7,C7S,D7,D7S,E7,F7,F7S,G7,G7S,A7,A7S,B7,
  C8,C8S,
  VOICE_END=0xff
};

#define VOICE_TOP   C8S

enum LENGTH{
  M64=1,  //64분음표
  M32=2,  //32분음표
  M24=3,  //24분음표
  M16=4,  //16분음표
  M12=6,  //12분음표
  M8=8,   //8분음표
  M6=12,  //6분음표
  M4=16,  //4분음표
  M3=24,  //3분음표
  M2=32,  //2분음표
  M0=64   //온음표 
};
#define BPM80   47  // unit : ms
#define BPM120  31  // unit : ms
#define BPM160  23  // unit : ms
#define MELO_M64  BPM160

typedef struct {
  struct {
    u8 play  :1;
  }b;
  u32 Timer; // 음길이
  u8 *melody;
  u16 pointer;
  u16 voice_prev;
  u16 voice;
  u8 length;
  u16 freq_period;
} Type_Melo;
extern Type_Melo Melo;

#define IS_MELODY_PLAY (Melo.b.play)
#define IS_MELODY_STOP (!Melo.b.play)


void Melody_Driver(void);
void Melody_Start(u8 *melody);
void Melody_Stop(void);


#endif



