/*-----------------------------------------
by Kang Woo Jung

2017.01. 17

-------------------------------------------*/

#ifndef __WATER_TEMP_H__
#define __WATER_TEMP_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f.h"  // ../KLIB/

	 
#define WATER_REF_MAX	30.0F	// ℃
#define WATER_REF_MIN		0.0F		// ℃

#define WATER_AMP_MAX	1800.0F	// rpm
#define WATER_AMP_MIN 	550.0F		// rpm
#define WATER_AMP_TOP	(WATER_AMP_MAX - WATER_AMP_MIN)

#define WATER_PID_PERIOD	1000		// us  	 
#define WATER_PID_KP	7.0F
#define WATER_PID_KI	 	0.05F
#define WATER_PID_KD	7.0F

#define WATER_PI_CTR_TEMP  1.0F		// ℃ 이하에서 2nd PI 추가!!!
#define WATER_PI_KP			 	(1.0F / WATER_PI_CTR_TEMP)
#define WATER_PI_ERR_KI	 	0.05F
#define WATER_PI_AMP_KI	 	0.1F

void water_pid_init(void);
float water_pid(float temp, float ref);
	 
	 
#ifdef __cplusplus
}
#endif

#endif


