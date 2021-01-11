/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/
#include "bsp.h"//////////////
#include "clcd.h"

cu16 CLCD_ADDR[]={0,0x40,0x14,0x54};

clcd_t  clcd;

void clcd_init(void)
{
  for(uint16_t i=0 ; i<CLCD_BUF_SIZE ; i++) {
    clcd.buf[i] = ' ';
  }
  //
  cLcd_Init();
}

void clcd_process(void) 
{
	static uint16_t cnt, mc, index;
	
	if(++cnt<10) return;
	cnt = 0;
	
  switch(mc){
 	// addr LINE1
	case 0:
		index = CLCD_LINE1;
		RS_lo; RW_lo; E_hi; 
		LCD_DATA_OUT(0x80);
		mc++;
		break;      
	case 1: E_lo; mc++; break;	  
	// data
	case 2:
    	RS_hi; RW_lo; E_hi; 
	    LCD_DATA_OUT(clcd.buf[index]);
    	mc++;
	    break;      
  case 3:
		E_lo;
    if(++index >= CLCD_LINE_LEN) mc++;
		else mc = 2;
		break;	  
 	// addr LINE2
	case 4:
		index = CLCD_LINE2;
		RS_lo; RW_lo; E_hi; 
		LCD_DATA_OUT(0xC0);
		mc++;
		break;      
	case 5: E_lo; mc++; break;	  
	// data
	case 6:
    	RS_hi; RW_lo; E_hi; 
	    LCD_DATA_OUT(clcd.buf[index]);
    	mc++;
	    break;      
  case 7:
		E_lo;
    if(++index >= CLCD_LINE2+CLCD_LINE_LEN) mc = 0;
		else mc = 6;
		break;	  
  default: mc = 0; break;
  }
}

void clcd_clear(void)
{
  for(uint16_t i=0 ; i<CLCD_BUF_SIZE ; i++) clcd.buf[i] = ' ';
}



//------------------------------------------------------------- cLCD Driver

void cLcd_Delay(u32 t)
{
  u32 i;
  for(;t!=0;t--){
		i=11;
		for(;i!=0;i--);
  }
}


void cLcd_Init(void)
{
  cLcd_Command(0x3c); // Function : 8bit mode, display on
  cLcd_Command(0x0c); // display on, cursor off, blink off
  cLcd_Command(0x01); // display clear all
  cLcd_Command(0x06); // Entry mode set
  cLcd_FontAddr(0);  // 0~56
	cLcd_Clear();
}

u32 cLcd_Str(u32 row, u32 column, cu8 *str)
{
  cLcd_Cursor(row, column);
  while(*str){ cLcd_Data(*str); str++; column++;}
  return column;
}

void cLcd_Cha(u32 row, u32 column, cu8 cha)
{
  cLcd_Cursor(row, column);
  cLcd_Data(cha);
}

void cLcd_Cursor(u32 row, u32 column)
{
  cLcd_Command((u8) (CLCD_ADDR[row]+column) | 0x80);
}


void cLcd_CursorOnOff(u8 val)
{
  if(val) cLcd_Command(0x0d);
  else cLcd_Command(0x0c);
}


void cLcd_FontAddr(u8 addr)
{
  if(addr > 56) addr = 56;
  cLcd_Command(addr + 0x40); // CGRAM address
}

void cLcd_Font(u8 *p)
{
  u8 i;
  for(i=0;i < 8; i++){  cLcd_Data(*p); p++; }
}

void cLcd_Clear(void)
{
  cLcd_Command(0x01); // display clear
  cLcd_Delay(LCD_DELAY_INIT); // delay 1.5ms
}

void cLcd_ClearRow(u8 row)
{
  u8 i;
  u8 addr=0;
  switch(row){  // 20¹®ÀÚ 4Çà LCD addr
    case 0: addr = 0x00; break;
    case 1: addr = 0x40; break;
    case 2: addr = 0x14; break;
    case 3: addr = 0x54; break;
  }
  cLcd_Command(addr | 0x80);
  for(i=0;i<0x14;i++){ cLcd_Data(' '); }
}


void cLcd_Command(u8 command)
{
  RS_lo; RW_lo; E_hi;
  LCD_DATA_OUT(command);
  cLcd_Delay(LCD_DELAY_CK);
  E_lo;
	cLcd_Delay(LCD_DELAY_DATA);
}

void cLcd_Data(u8 data)
{
  RS_hi;  RW_lo; E_hi; 
	LCD_DATA_OUT(data);
  cLcd_Delay(LCD_DELAY_CK);
  E_lo;
	cLcd_Delay(LCD_DELAY_DATA);
}
