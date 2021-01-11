
/*-----------------------------------------
by Kang Woo Jung

2018.11.28

-------------------------------------------*/
// encoder rotary switch

#ifndef __ENCODER_H__
#define __ENCODER_H__ 

#include "ktype.h"

typedef enum {
  ENC_RISING = 1,
  ENC_FALLING,
  ENC_RISING_FALLING,
}enc_def;

typedef struct {
	uint16_t pre;
	int16_t val;
}enc_t;

//@ "a_b" --> bit0 = a, bit1 = b
void encoder_process(enc_t *enc, enc_def edge, uint16_t a_b);

/*
	using...
	
	int16_t val;
	static enc_t enc;
	
	val += encoder_get(*enc, ENC_RISING, (PinA.b1<<1)|(PinA.b0<<0));
	display(val);
	.
	.
	.
	
*/

#endif
