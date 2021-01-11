/*---------------------------------------------------------------------
by Kang Woo Jung: 2020.03.28

-----------------------------------------------------------------------*/
#include "usr.h"
#include "an.h"
//#include "cos.h"



/* ---------------------------------------------------------------*/

an_t	   an;

/*
Gain = (measure_max - measure_min) / (diplay_max - display_min)
Offset = measure_max - (diplay_max * Gain)
       = measure_min - (diplay_min * Gain)
*/

void an_process(void)
{
	//HAL_IWDG_Refresh(&hiwdg); // WatchDOG Clear!!!
	
	/* DMA Buffer to AN Buffer */
	/* Get ADC1_12B */
	uint16_t i, j;
	i=0;
	for(j=0 ; j<(ADC1_CH_NUM/2) ; j++){
		(*(uint32_t *) &an.adc[i]) = (*(uint32_t *) &an.adc1_buf[j]);  i+=2;
	}
	//if(ADC1_CH_NUM%2) an.adc[i++] = (uint16_t)(an.adc1_buf[j] & 0xFFFF);

	/* Get ADC2_12B */
	//for(j=0 ; j<(ADC2_CH_NUM/2) ; j++){
	//	(*(uint32_t *) &an.adc[i]) = (*(uint32_t *) &an.adc2_buf[j]);  i+=2;
	//}
	//if(ADC2_CH_NUM%2) an.adc[i++] = (uint16_t)(an.adc2_buf[j] & 0xFFFF);

	/* Get ADC3_12B */
	//for(i=0, j=0 ; j<(ADC3_CH_NUM/2) ; j++){
	//	(*(uint32_t *) &an.adc[i]) = (*(uint32_t *) &an.adc3_buf[j]);  i+=2;
	//}
	//if(ADC3_CH_NUM%2) an.adc[i++] = (uint16_t)(an.adc3_buf[j] & 0xFFFF);
	
	
	/* Amplitude */
	i = AN_VOLT;
	an.val[i] = (float) an.adc[i] * 326.7f * 0.967f / ADC_MAX_VAL;
	an.ave1[i] = Filter(0.05f, an.val[i], an.ave1[i]);   // filter_1st
	an.ave2[i] = Filter(0.01f, an.ave1[i], an.ave2[i]);  // filter_2nd

	/* ACS713ELCTR-20A-T
	> Sensitivity = 185 mV/A
	> Offset = 0.48V;	
	*/
	i++; //AN_CURR
	an.val[i] = (float) an.adc[i] * 3.3F / ADC_MAX_VAL;
	an.val[i] -= 0.48f;
	an.val[i] /= 0.185F;
	an.val[i] -= an.ofs[i];
	if(an.val[i] < 0) an.val[i] = 0;
	an.ave1[i] = Filter(0.05f, an.val[i], an.ave1[i]);   // filter_1st
	an.ave2[i] = Filter(0.01f, an.ave1[i], an.ave2[i]);  // filter_2nd

	i++; //AN_V24
	an.val[i] = (float) an.adc[i] * 33.0f / ADC_MAX_VAL;
	an.ave1[i] = Filter(0.05f, an.val[i], an.ave1[i]);   // filter_1st
	an.ave2[i] = Filter(0.01f, an.ave1[i], an.ave2[i]);  // filter_2nd

  i++; //AN_V12
	an.val[i] = (float) an.adc[i] * 33.0f / ADC_MAX_VAL;
	an.ave1[i] = Filter(0.05f, an.val[i], an.ave1[i]);   // filter_1st
	an.ave2[i] = Filter(0.01f, an.ave1[i], an.ave2[i]);  // filter_2nd

  i++; //AN_V5
	an.val[i] = (float) an.adc[i] * 33.0f / ADC_MAX_VAL;
	an.ave1[i] = Filter(0.05f, an.val[i], an.ave1[i]);   // filter_1st
	an.ave2[i] = Filter(0.01f, an.ave1[i], an.ave2[i]);  // filter_2nd
	
	/*
	i++; //AN_TEMP
	an.val[i] = ntc_GetTempADC(ERTJ1VG103F, an.adc[i]);
	an.ave1[i] = Filter(0.05f, an.val[i], an.ave1[i]);   // filter_1st
	an.ave2[i] = Filter(0.01f, an.ave1[i], an.ave2[i]);  // filter_2nd
	*/

	i++; // AN_TMP in mcu
	an.val[i] = cpu_temp(an.adc[i]);
	an.ave1[i] = Filter(0.01f, an.val[i], an.ave1[i]);   // filter_1st
	an.ave2[i] = Filter(0.01f, an.ave1[i], an.ave2[i]);  // filter_2nd
	
/*
	i++; // AN_BAT in mcu
	an.val[i] = cpu_bat(an.adc[i]);
	an.ave1[i] = Filter(0.01f, an.val[i], an.ave1[i]);   // filter_1st
	an.ave2[i] = Filter(0.01f, an.ave1[i], an.ave2[i]);  // filter_2nd
	
	i++; // AN_VREF in mcu
	an.val[i] = cpu_vref(an.adc[i]);
	an.ave1[i] = Filter(0.05f, an.val[i], an.ave1[i]);   // filter_1st
	an.ave2[i] = Filter(0.01f, an.ave1[i], an.ave2[i]);  // filter_2nd
*/
}



// stm32f103
#define ADC_TEMP_V25   1.43F
#define ADC_AvgSlope	 0.0043F
float cpu_temp(uint16_t adc_value)
{
  return ((ADC_TEMP_V25 - ((float) adc_value * ADC_VREF / ADC_MAX_VAL)) / ADC_AvgSlope + 25); // stm32f103
}

float cpu_bat(uint16_t adc_value)
{
	float vref;
	vref = (float) adc_value * 10.0f / ADC_MAX_VAL;
	return vref;
}

#define ADC_VrefINT  1.2F
float cpu_vref(uint16_t adc_value)
{
	return ((float)(ADC_VrefINT * ADC_MAX_VAL) / adc_value);
}

#define AN_RMS_MAX		 100000.0F	
#define AN_RMS_MIN		-100000.0F
void an_sampling(sample_t *sample, uint16_t sampling_su, float value) // sampling_su (unit: loop time)
{
	if(value > sample->max_scan) sample->max_scan = value;
	if(value < sample->min_scan)  sample->min_scan = value;
	//
	if(++sample->cnt >= sampling_su){
		//TP2^=1; /////////////////////////////////
		sample->cnt = 0;
		sample->max = sample->max_scan;
		sample->min = sample->min_scan;
		sample->mean = ((sample->max_scan - sample->min_scan) / 2.0F) + sample->min_scan;
		sample->max_scan = AN_RMS_MIN;
		sample->min_scan = AN_RMS_MAX;
	}
}

#define VACUUM_Sensitivity 			0.03826F  // V/kPa
#define VACUUM_R_GAIN					(1.0F+(15.0F/30.0F))
#define VACUUM_V_AMP					(ADC_VREF*VACUUM_R_GAIN)
#define VACUUM_V_OFFSET			4.55F  // VS=5V, 4.5V~4.6V
#define VACUUM_V_CAL(adc_val)	(((float) adc_val * VACUUM_V_AMP) / ADC_MAX_VAL)
float an_vacuum_cal(uint16_t adc_value)
{
	float vacuum;
	vacuum = VACUUM_V_CAL(adc_value);			// ADC 전압 값 취득
	vacuum = VACUUM_V_OFFSET - vacuum;	// 압력에 대한 전압 값 취득
	vacuum *= (1.0F/VACUUM_Sensitivity);  			// 압력환산
	return vacuum;
}

#define ZX_Rsh	0.01F			// R shunt
#define ZX_Rs		1200.0F	// R sense
#define ZX_Rc		100.0F		// R chip on
#define ZX_Ic			0.000004F	// Curr chip load
#define ZX_CURR_MAX   (((ADC_VREF / ZX_Rs) - ZX_Ic) * ZX_Rc / ZX_Rsh)		// 27.46 [A]

float cal_zxct1009(uint16_t adc_value)
{
	return  ((float) adc_value * (ZX_CURR_MAX / ADC_MAX_VAL));
}


/* ---------------------------------------------------------------*/


