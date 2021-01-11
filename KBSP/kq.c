/*---------------------------------------------------------------------
by Kang Woo Jung: 2019.01.09

-----------------------------------------------------------------------*/
#include "kq.h"

void q_put(q_t *q, uint8_t data)
{
  if(((q->inP+1) & Q_SIZ_MASK) == q->outP)	return;
  q->buf[q->inP++] = data;
	q->inP &= Q_SIZ_MASK;
}

uint8_t q_get(q_t *q)
{
  uint8_t ret;
  if(q->inP == q->outP)	return (0);
  ret = q->buf[q->outP++];
	q->outP &= Q_SIZ_MASK;
  return (ret);
}

uint16_t q_end(q_t *q)
{
	return (q->buf[((q->inP - 1) & Q_SIZ_MASK)]); // 포인터 변화 없음.
}

uint16_t q_size(q_t *q)
{
  return ((q->inP - q->outP) & Q_SIZ_MASK);
}

void q_clear(q_t *q)
{	
	q->outP = q->inP = 0;
}
