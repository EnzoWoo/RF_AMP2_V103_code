/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/
#ifndef __BSP_H__
#define __BSP_H__


// System GPIO
#include "main.h"
#include "adc.h"
//#include "comp.h"
#include "crc.h"
//#include "dac.h"
#include "dma.h"
#include "i2c.h"
//#include "irtim.h"
#include "iwdg.h"
//#include "rng.h"
//#include "rtc.h"
#include "spi.h"
#include "tim.h"
//#include "tsc.h"
#include "usart.h"
#include "gpio.h"

// User
#include "ktype.h"
//#include "bldc.h"
//#include "bprintf.h"
//#include "clcd.h"
//#include "clcd_spi.h"
//#include "clk.h"   //rtc
#include "dsp.h"
#include "edge.h"
#include "eeprom.h"
//#include "encoder.h"
#include "event.h"
//#include "evt.h"
//#include "fnd.h"
#include "key.h"
#include "kcrc.h"
#include "kq.h"
//#include "kpr.h"
#include "kpro.h"
#include "ntc.h"
#include "pid.h"
//#include "servo.h"
#include "uprintf.h"
#include "ltc6903.h"


void bsp_init(void);

#endif
