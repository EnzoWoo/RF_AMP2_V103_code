/*---------------------------------------------------------------------
by Kang Woo Jung: 2019.03.25

-----------------------------------------------------------------------*/
#ifndef __KEY_H__
#define __KEY_H__ 

#ifdef __cplusplus
 extern "C" {
#endif

#include "ktype.h"

/* Unit [1msec]  */
#define CHATTERING_TIME		10			// ms
#define TWO_FUNC_TIME	    1000		// ms for 2nd key
#define THREE_FUNC_TIME	  3000		// ms for 3rd key
#define SPEED_WAIT_TIME		450			// ms
#define ACC_INTERVAL_TIME	2000		// ms

	 
typedef	struct {
	uint32_t det;	 	// Detection
	uint32_t pre;	 	// Level Previous
	uint32_t lev;	 	// Level
	uint32_t mak;	 	// Rsing  Edge
	uint32_t bra;		// Falling Edge
	uint32_t bot;		// Rising & Falling Edge
	uint32_t hol;		// Risng Edge Hold :: user가 clear
	//
	uint32_t chattering[32];
}in_t;

typedef	struct {
	uint32_t det;		// Detection
	uint32_t pre;		// Level Previous
	uint32_t lev;		// Level
	uint32_t mak;		// Rsing  Edge
	uint32_t bra;		// Falling Edge
	uint32_t bot;		// Rising & Falling Edge
	uint32_t hol;		// Risng Edge Hold :: user가 clear
	uint32_t one;   // One function
	uint32_t two;   // Two function
	uint32_t thr;   // Three function
	uint32_t spe;		// Key Speed Pulse
	//
	uint32_t chattering[32];
	uint32_t delay[32];
	uint32_t speed[32];
	uint32_t acc_interval[32];
	uint32_t acc_step[32];
}key_t;


void in_process(in_t * in, uint16_t in_su); // in_su : 1~32        // in.det 에 소스연결
void key_process(key_t * key, uint16_t key_su); // key_su : 1~32   // key.det 에 소스연결


#ifdef __cplusplus
}
#endif

#endif
