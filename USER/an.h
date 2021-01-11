/*---------------------------------------------------------------------
by Kang Woo Jung: 2020.03.28

-----------------------------------------------------------------------*/
#ifndef __AN_H__
#define __AN_H__

#include "ktype.h"

/* ---------------------------------------------------------------*/
/*
	 Clock source(PCLK2) : 108MHz
	 Prescaler : PCLK2 Divided by 4
	 Resolution : 12bit 15 ADC clock cycle
	 Sampling Time : 144 cycles
	 Number of conversion: 17 	// DMA channel num(32bit) = Number of conversion(16bit) / 2 , 나머지가 있을 경우 + 1
	 DMA: Yes
	 Total conversion time = 1/(108MHz /4div /(15cycle+144cycle) /(9 DMA_ch))
*/	 
#define ADC1_CH_NUM	6
#define ADC2_CH_NUM	0
#define ADC3_CH_NUM	0
#define ADC_NUM 	(ADC1_CH_NUM + ADC2_CH_NUM + ADC3_CH_NUM)
	 
#define ADC_MAX_VAL	(4096.0F-1.0F)
#define ADC_VREF   	3.3F		// V


/* ---------------------------------------------------------------*/
typedef enum {
	AN_VOLT=0,
	AN_CURR,
	AN_V24,
	AN_V12,
	AN_V5,
	AN_CPU_TEMP,
	//
	AN_SIZE
} Type_AnEnum;


/* ---------------------------------------------------------------*/
typedef struct
{
#if ADC1_CH_NUM	
	uint32_t adc1_buf[ADC1_CH_NUM];
#endif
#if ADC2_CH_NUM	
	uint32_t adc2_buf[ADC2_CH_NUM];
#endif
#if ADC3_CH_NUM	
	uint32_t adc3_buf[ADC3_CH_NUM];
#endif
	int16_t adc[ADC_NUM];
	float ofs[AN_SIZE];
	float val[AN_SIZE];
	float ave1[AN_SIZE];
	float ave2[AN_SIZE];
}an_t;
extern an_t	 an;

typedef struct {
	uint16_t cnt;	// sampling counter
	float max_scan;
	float min_scan;
	//result
	float max;
	float min;
	float mean;
}sample_t;
extern sample_t beam;


/* ---------------------------------------------------------------*/
void an_process(void);

float cpu_temp(uint16_t adc_value);
float cpu_bat(uint16_t adc_value);
float cpu_vref(uint16_t adc_value);
void an_sampling(sample_t *sample, uint16_t sampling_su, float value);
float an_vacuum_cal(uint16_t adc_value);
float cal_zxct1009(uint16_t adc_value);

#endif


