/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/
#ifndef __NTC_H__
#define __NTC_H__ 


#include "ktype.h"
	 
#define NTC_TABLE_SIZE   	41	 
	 
extern const float NTC_D20K[NTC_TABLE_SIZE];
extern const float ERTJ1VG103F[NTC_TABLE_SIZE];	 
extern const float LY1_395F103F_25RT[NTC_TABLE_SIZE];
extern const float SPM_45_SERIES[NTC_TABLE_SIZE];       //// For FairChild IGBT module SPM45Series
	 
	 
#define KELVIN_K		273.0F

/* Steinhart */
/*	 
// NTC-D20K (10k) 
#define	C3a	(16.88631888f/10000000.0F)
#define	C2a	(2.081672056F/10000.0F)
#define	C1a	(1.30514346F/1000.0F)
*/
/* ERTJ1VG103F */
#define	C3a	(14.50478745F/10000000.0F)
#define	C2a	(-0.761659699F/10000.0F)
#define	C1a	(2.88965412F/1000.0F)

	 
float ntc_GetTempADC(const float *table, uint16_t adc);  // ADC direct 보간
	
float ntc_GetTempS(float ntc_r); 		 //Steinhart 방정식
float ntc_GetR(uint16_t adc);

//float ntc_GetTempR(float ntc_r); 		 // Resistor 보간

#endif

