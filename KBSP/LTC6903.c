
/*---------------------------------------------------------------------
by Kang Woo Jung: 2019.12.26

-----------------------------------------------------------------------*/

//#include <stdint.h> 
//#include <math.h>
//#include "math.h" 

#include "bsp.h" 
#include "LTC6903.h" 

uint16_t LTC6903_Get_Freq(uint16_t code)
{
	uint8_t oct;
	uint16_t dac;
	float dacf;
	float f; 
	oct = code >> 12;
	oct &= 0x0f;
	dac = (code >> 2);
	dac &= 0x3ff;
	dacf = dac;
	dacf /= 1024;
	dacf = 2 - dacf;
	f = 1 << oct;
	f *= 2078;
	f /= dacf;
	f /= 1000;
	return ((uint16_t)f);
}

uint16_t LTC6903_Get_Code(uint16_t freq_khz)
{
	uint8_t oct;
	uint16_t dac;
	float freq;
	float tmpf;
  double tmpd; 
	
	//kwj 편차 보정
	if(freq_khz >= 1000) freq_khz += 7;
	else if(freq_khz >= 833) freq_khz += 6;
	else if(freq_khz >= 666) freq_khz += 5;
	else if(freq_khz >= 500) freq_khz += 4;
	else if(freq_khz >= 366) freq_khz += 3;
	else if(freq_khz >= 233) freq_khz += 2;
	else if(freq_khz >= 100) freq_khz += 1;
	
	// CAl Start
	freq = (float)(freq_khz * 1000);
  tmpd = log10((double)(freq / 1039)); 
  tmpd *= 3.322F; 
  if (tmpd>15) tmpd = 15; 
  if (tmpd<0) tmpd = 0; 
  oct = (uint8_t)tmpd;
	
  tmpf = 2048 - (2078 * pow(2, (10+oct))) / freq;
	tmpf = (tmpf > (floor(tmpf) + 0.5)) ? ceil(tmpf) : floor(tmpf);
  if (tmpf > 1023) tmpf = 1023; 
  if (tmpf < 0) tmpf = 0;
	dac = (uint16_t)tmpf;	
	
	return ((uint16_t)((oct<<12) | (dac<<2) | LTC6903_CLK0)); 
}

