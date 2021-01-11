
/*-----------------------------------------
by Kang Woo Jung

2016.04.08

-------------------------------------------*/

#ifndef __OLED_WEG010016_H__
#define __OLED_WEG010016_H__

#include "stm32f.h"  // ../KLIB/


#define OLED_DATA   PortD.lo
#define OLED_RS  			PortD.b.b8
#define OLED_RW 			PortD.b.b9
#define OLED_E     		PortD.b.b10

//#define OLED_BUSY	PinD.b.b7
//#define OLED_CS    	PortB.b.b12


#define 	MAXCOL	100
#define 	MAXROW	16


#define SETDDRAMADD1st(x) (0x80|(x&0x7F))	//x
#define SELPAGEADD(x) (0x40|(x&0x01)) // y  =>0x40
											   
#define RECTANGLEADD(x) (11+((5+1) * x))
#define RECTANGLEADD1(x) ((5+1) * x)


void	oled_Test(void);

void oled_Delay(u32 del);
void oled_DWriteCmd(u8 cmd);
void oled_WriteData(u8 dat) ;
u8 oled_ReadData(void);

void oled_Init(void);

void oled_CheckBusy(void);
void oled_cleanScreen(void);
void oled_DrawImg(const u8 *p);



extern const u8 IMG_0[200];

extern const u8 IMG_3[200];

extern const u8 IMG_41[200];
extern const u8 IMG_42[200];
extern const u8 IMG_43[200];
extern const u8 IMG_44[200];
extern const u8 IMG_45[200];
extern const u8 IMG_46[200];

extern const u8 IMG_51[200];
extern const u8 IMG_52[200];
extern const u8 IMG_53[200];
extern const u8 IMG_54[200];
extern const u8 IMG_55[200];
extern const u8 IMG_56[200];
extern const u8 IMG_57[200];
extern const u8 IMG_58[200];

extern const u8 IMG_6[200];

extern const u8 IMG_7[200];

extern const u8 IMG_8[200];
extern const u8 IMG_81[200];
extern const u8 IMG_9[200];
extern const u8 IMG_10[200];
extern const u8 IMG_11[200];
extern const u8 IMG_12[200];
extern const u8 IMG_13[200];

extern const u8 IMG_20[200];
extern const u8 IMG_21[200];
extern const u8 IMG_22[200];
extern const u8 IMG_23[200];
extern const u8 IMG_24[200];

extern const u8 IMG_101[200];
extern const u8 IMG_102[200];
extern const u8 IMG_103[200];
extern const u8 IMG_104[200];
extern const u8 IMG_105[200];
extern const u8 IMG_106[200];
extern const u8 IMG_107[200];
extern const u8 IMG_108[200];
extern const u8 IMG_109[200];
extern const u8 IMG_110[200];


#endif


