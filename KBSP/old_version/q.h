/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/

#ifndef __Q_H__
#define __Q_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "ktype.h"

	 
#define Q_SIZE 	512

typedef struct {
 uint32_t head;
 uint32_t tail;
	uint8_t buf[Q_SIZE+1]; // dummy Ãß°¡
}q_t;


void q_put(q_t *q, uint8_t data);
uint8_t q_get(q_t *q);
void q_clear(q_t *q);
uint32_t q_is_data(q_t *q);
uint32_t q_get_size(q_t *q);



#ifdef __cplusplus
}
#endif


#endif


