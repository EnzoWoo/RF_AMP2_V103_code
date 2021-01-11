/*---------------------------------------------------------------------
by Kang Woo Jung: 2019.01.09

-----------------------------------------------------------------------*/

#ifndef __KPR_H__
#define __KPR_H__ 

#include "ktype.h"
#include "kq.h"
#include "kevt.h"

/* Format
	1. STX,DATA0,DATA1, ..... ,DATAn, CRC_L, CRC_H,ETX
	2. CRC Cal = DATA only
	3. Little endian
*/

typedef struct {
	q_t qrx;
	q_t qtx;
	uint8_t bufrx[Q_SIZ]; //tag
	uint8_t buftx[Q_SIZ]; //tag
	uint8_t bufDMA[Q_SIZ];
	//
	uint16_t busy;
	evus_t evus;
	uint16_t timeout;
} kpro_t;

uint16_t kpro_receive_size(kpro_t *kpro, UART_HandleTypeDef *huart, uint16_t size);
uint16_t kpro_receive_loop(kpro_t *kpro, UART_HandleTypeDef *huart, uint16_t timeout); //looptime timeout 설정 잫 할 것!!!! 대단히 중요! mcu폭주도 가능!
uint16_t kpro_receive_time(kpro_t *kpro, UART_HandleTypeDef *huart, uint16_t timeout); //[msec] timeout 설정 잫 할 것!!!! 대단히 중요! mcu폭주도 가능!
void kpro_transmit(kpro_t *kpr, UART_HandleTypeDef *huart, uint16_t size); // size = tag data byte su
void kprintf(kpro_t *kpro, const uint8_t *fmt,...);

#endif

