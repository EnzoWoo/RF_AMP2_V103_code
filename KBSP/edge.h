/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/
#ifndef __EDGE_H__
#define __EDGE_H__ 

#include "ktype.h"

typedef	struct {
	uint32_t pre;		// Source Previous
	uint32_t mak;	  // Rsing  Edge
	uint32_t bra;		// Falling Edge
	uint32_t bot;		// Rising & Falling Edge
	uint32_t hol;		// Risng Edge Hold :: user°¡ clear
}edge_t;

void edge_process(edge_t *edge, uint32_t source);


#endif

