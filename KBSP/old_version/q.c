/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/

#include "q.h"


void q_put(q_t *q, uint8_t data)	// write
{
  if(q->head+1 == q->tail || ((q->head == Q_SIZE-1) && q->tail == 0)) return; 
  q->buf[q->head] = data;
  if(++q->head >= Q_SIZE) q->head = 0;	
  //++q->head; q->head %= Q_SIZE;
}

uint8_t q_get(q_t *q)	// read
{
  uint8_t ret;
  if(q->head == q->tail)	return (0);
  ret = q->buf[q->tail];
  if(++q->tail >= Q_SIZE)  q->tail = 0;
  //++q->tail; q->tail %= Q_SIZE;
  return (ret);
}


void q_clear(q_t *q)
{
	q->head = q->tail = 0;	// 초기화
}

uint32_t q_is_data(q_t *q)		// data 있니?
{
	if(q->head != q->tail) return (1);		// 어 있어 1을 반환
	return (0);	// 없네 
}


uint32_t q_get_size(q_t *q)
{
  if(q->head < q->tail){		//	 
			return (Q_SIZE - (q->tail + q->head));	// 512 - (11 + 10) =  491?
 }
  return (q->head - q->tail);	// 512 - 511 = 1
}


