/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/

#ifndef __CLCD_H__
#define __CLCD_H__

#include "kType.h"

#define LCD_DATA    PORTD.low
#define LCD_RS      PortC.b10
#define LCD_RW      PortC.b11
#define LCD_E       PortC.b12
#define LCD_BACKLIGHT PortA.b13

#define RS_hi       LCD_RS=1
#define RS_lo       LCD_RS=0
#define RW_hi       LCD_RW=1
#define RW_lo       LCD_RW=0
#define E_hi        LCD_E=1
#define E_lo        LCD_E=0

#define LCD_DATA_OUT(data)  { LCD_DATA = data;}


#define LCD_DELAY_CK    7        //1.11us   
#define LCD_DELAY_DATA  (LCD_DELAY_CK * 30)	   //30us 
#define LCD_DELAY_INIT  (LCD_DELAY_CK * 3000)  //3ms
#define cLcd_BackLightON()  { LCD_BACKLIGHT=1; }
#define cLcd_BackLightOFF() { LCD_BACKLIGHT=0; }


/* Buffer Drive */
#define  CLCD_LINE_LEN   16
#define  CLCD_LINE_NUM   2
#define  CLCD_LINE1      0
#define  CLCD_LINE2     (CLCD_LINE_LEN+16)
#define  CLCD_BUF_SIZE  (CLCD_LINE2*CLCD_LINE_NUM)
typedef struct {
	uint8_t buf[CLCD_BUF_SIZE];
}clcd_t;
extern clcd_t  clcd;
void clcd_init(void);
void clcd_process(void);
void clcd_clear(void);  

  
/* Direct Drive */	
void cLcd_Delay(u32 t);
void cLcd_Init(void);

u32 cLcd_Str(u32 row, u32 column, cu8 *str);
void cLcd_Cha(u32 row, u32 column, cu8 cha);
void cLcd_Cursor(u32 row, u32 column);

void cLcd_CursorOnOff(u8 val);
void cLcd_FontAddr(u8 addr);
void cLcd_Font(u8 *p);
void cLcd_Clear(void);
void cLcd_ClearRow(u8 row);

void cLcd_Command(u8 command);
void cLcd_Data(u8 data);


#endif



