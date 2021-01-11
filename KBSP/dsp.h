/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/


#ifndef __DSP_H__
#define __DSP_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "ktype.h"

	 


// 'factor'는 적분량과 반비례! 범위는(0<Ki && Ki<1)
float Filter(float factor , float new_val, float avg_val);		// Factor : <1, >0
float Filter2(float factor , float new_val, float avg_val);		// Factor : <1, >0 미분 추가

	 
#ifdef __cplusplus
}
#endif

#endif


