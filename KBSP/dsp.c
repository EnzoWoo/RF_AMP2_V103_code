/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/
#include "dsp.h"

float Filter(float factor , float new_val, float avg_val)
{
 	if(factor > 1.0f || factor < 0.0f) return new_val;
	return ((new_val * factor) + (avg_val * (1.0f - factor)));
}

float Filter2(float factor , float new_val, float avg_val)
{
	float dev;
	dev = fabs(avg_val - new_val);
	if(dev < 5.0f){
		dev *= 0.05f;
		factor *= dev;
	}	
 	if(factor > 1.0f || factor < 0.0f) return new_val;
	return ((new_val * factor)+(avg_val * (1.0f - factor)));
}

