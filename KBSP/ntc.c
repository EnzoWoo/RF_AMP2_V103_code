/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/
#include <math.h>
//#include "arm_math.h"
#include "ntc.h"
#include "ntc_table.h"

float ntc_GetTempADC(const float *table, uint16_t adc)
{
	uint16_t index;
	int32_t dx_adc, width_adc;
	float ret;
	
	if(table[0] > table[NTC_TABLE_SIZE-1]){
		// NTC is Bottom Side
		if(adc >= table[0]){
			ret = NTC_TABLE_BOT; // 저온
		}
		else if(adc <= table[ NTC_TABLE_SIZE-1]){
			ret =  NTC_TABLE_TOP; // 고온
		}
		else {
			// Serch Level
			for(index=1 ; index <  NTC_TABLE_SIZE ; index++){
				if(adc >= table[index]) break;
			}
			// 보간법
			dx_adc = adc - table[index - 1];
			width_adc = table[index] - table[index - 1];
			ret = ((float) dx_adc / width_adc + index - 1) * NTC_TABLE_UNIT + NTC_TABLE_BOT;
		}
	}
	else {
		// NTC is Top Side
		if(adc <= table[0]){
			ret = NTC_TABLE_BOT; // 저온
		}
		else if(adc >= table[ NTC_TABLE_SIZE-1]){
			ret =  NTC_TABLE_TOP; // 고온
		}
		else {
			// Serch Level
			for(index=1 ; index <  NTC_TABLE_SIZE ; index++){
				if(adc <= table[index]) break;
			}
			// 보간법
			dx_adc = adc - table[index - 1];
			width_adc = table[index] - table[index - 1];
			ret = ((float) dx_adc / width_adc + index - 1) * NTC_TABLE_UNIT + NTC_TABLE_BOT;
			//ret = ((float) dx_adc *  NTC_TABLE_UNIT) / width_adc + ((float) index - 1) *  NTC_TABLE_UNIT +  NTC_TABLE_BOT;
		}
	}
	return ret;
}

/*
//Steinhart 방정식
float ntc_GetTempS(float ntc_r)
{
	double log_r, log_3r, ret;
	
	if(ntc_r >= 569426.0F)  ret = -50.0F;
	else if(ntc_r <= 191.0F)	ret = 150.0F;
	else {
		log_r = log(ntc_r);
		log_3r = log_r*log_r*log_r ;
		ret = 1.0f / (C1a + C2a*log_r + C3a*log_3r);
		//ret = 1.0f / (C1a + C2a*log_r + C3a*pow(log_r, 3));
		
		ret -= KELVIN_K; // K --> ℃
	}
	return ((float) ret);
}
*/


float ntc_GetR(uint16_t adc)
{
	return (Rtop / ((ADC_TOP / adc) - 1.0F));
}


// Resistor 보간 (알고리즘)
/*
float ntc_GetTempR(float ntc_r)
{
	uint16_t index;
	float dx_r,width_r;
	float x1_temp,x2_temp;
	float ret;
	
	if(ntc_r >= ntc_table[0])
	{
		ret = TABLE_BOT;
	}
	else if(ntc_r <= ntc_table[TABLE_SIZE - 1])
	{
		ret = TABLE_TOP;
	}
	else {
		for(index = 2 ; index < TABLE_SIZE ; index++){
			if(ntc_r >= ntc_table[index]) break;
		}
		// 보간법
		dx_r = ntc_r - ntc_table[index - 1];
		width_r = ntc_table[index] - ntc_table[index-1];
		x1_temp = (float)((index - 1) * TABLE_UNIT) + TABLE_BOT;
		x2_temp = (float)(index * TABLE_UNIT) + TABLE_BOT;
		ret = (((x2_temp - x1_temp) * dx_r) / width_r) + x1_temp;
	}
	return ret;
}
*/









