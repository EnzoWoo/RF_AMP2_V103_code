/*---------------------------------------------------------------------
by Kang Woo Jung: 2019.01.09

-----------------------------------------------------------------------*/

#ifndef __KQ_H__
#define __KQ_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "ktype.h"

#define Q_SIZ 256              // 중요!!! 2의 배수 ex) 16,32,64,128,256...
#define Q_SIZ_MASK  (Q_SIZ-1)  // 중요!!! 2의 배수 - 1,  ex) 15,31,63,127,255...
typedef struct {
  uint16_t inP;
  uint16_t outP;
	uint8_t buf[Q_SIZ+1];
} q_t;

void q_put(q_t *q, uint8_t data);
uint8_t q_get(q_t *q);
uint16_t q_end(q_t *q);
uint16_t q_size(q_t *q);
void q_clear(q_t *q);


#ifdef __cplusplus
}
#endif


#endif
