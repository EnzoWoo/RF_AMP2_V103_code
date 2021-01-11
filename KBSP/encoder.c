/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/
//#include "user.h"
#include "bsp.h"
#include "encoder.h"

//@ "a_b" --> bit0 = a, bit1 = b
void encoder_process(enc_t *enc, enc_def edge, uint16_t a_b)
{
	uint16_t sig, mak, bra;
	
	sig = enc->pre ^ a_b;
	mak = sig & a_b; 
	bra = sig & enc->pre; // both edge »ç¿ë½Ã...
	enc->pre = a_b;	
	enc->val = 0; // 
	
	if(edge == ENC_RISING){
		if(TypeBit16(mak).b0){
			if(TypeBit16(a_b).b1) enc->val = 1;
			else enc->val = -1;
		}
	}
	else if(edge == ENC_FALLING){
		if(TypeBit16(bra).b0){
			if(TypeBit16(a_b).b1) enc->val = 1;
			else enc->val = -1;
		}
	}
	else { // Both Edge, Rising & Falling
		if(TypeBit16(mak).b0 | TypeBit16(bra).b0){
			if(TypeBit16(mak).b0 ^ TypeBit16(a_b).b1) enc->val = 1;
			else  enc->val = -1;
		}
	}
}

