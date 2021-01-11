/*---------------------------------------------------------------------
by Kang Woo Jung: 2019.12.26

-----------------------------------------------------------------------*/
#include "bsp.h"
#include "edge.h"


void edge_process(edge_t *edge, uint32_t source)
{
	uint32_t signal;
	signal = edge->pre ^ source;
	edge->mak = signal & source; 
	edge->bra = signal & edge->pre; 
	edge->bot = edge->mak | edge->bra;
	edge->hol |= source;
	edge->pre = source; 
}	

