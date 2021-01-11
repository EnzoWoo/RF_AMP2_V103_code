/*-----------------------------------------
by Kang Woo Jung

2016.12.13

-------------------------------------------*/

#ifndef __KPRO_H__
#define __KPRO_H__ 

#ifdef __cplusplus
 extern "C" {
#endif
	 
//#include "stm32f4xx_hal.h"
//#include "stm32f7xx_hal.h"

#include "q.h"

#define START_CODE	0xAA
#define END_CODE		0xED
	 
#define CRC16_INIT_DAT	0xFFFF
#define XOR8_INIT_DAT	0x00

//bps 38400
#define KPRO_TIME_OUT  300    // us 통신속도보다 길게... ex) 1byte 전송속도 = 1/115200*10bit = 86.8us
#define KPRO_LINK_OUT   2500000 // us 통신간격보다 길게...


#define RX_BUF_SIZE		64
#define TX_BUF_SIZE		256

	 
/* ---------------------------------------------------------------*/
// (little endian)	 
/* ---------------------------------------------------------------*/
#define getLittle_U16(buffer, index, data)	{(*(uint16_t*) &data) = (*(uint16_t *)(buffer + index)) ; index += 2;}
#define getLittle_U32(buffer, index, data)	{(*(uint32_t*) &data) = (*(uint32_t *)(buffer + index)) ; index += 4;}

#define putLittle_U16(buffer, index, data)	{(*(uint16_t *)(buffer + index)) = (*(uint16_t*) &data); index += 2;}
#define putLittle_U32(buffer, index, data)	{(*(uint32_t *)(buffer + index)) = (*(uint32_t*) &data); index += 4;}


/* ---------------------------------------------------------------*/
// (big endian)	 
/* ---------------------------------------------------------------*/
#define getBig_U16(buffer, index, data){\
	((uint8_t*) &data)[1] = buffer[index++];\
	((uint8_t*) &data)[0] = buffer[index++];\
}\

#define getBig_U32(buffer, index, data){\
	((uint8_t*) &data)[3] = buffer[index++];\
	((uint8_t*) &data)[2] = buffer[index++];\
	((uint8_t*) &data)[1] = buffer[index++];\
	((uint8_t*) &data)[0] = buffer[index++];\
}\

#define putBig_U16(buffer, index, data){\
	buffer[index++] = ((uint8_t*) &data)[1];\
	buffer[index++] = ((uint8_t*) &data)[0];\
}\

#define putBig_U32(buffer, index, data){\
	buffer[index++] = ((uint8_t*) &data)[3];\
	buffer[index++] = ((uint8_t*) &data)[2];\
	buffer[index++] = ((uint8_t*) &data)[1];\
	buffer[index++] = ((uint8_t*) &data)[0];\
}\

	 	 
typedef enum {
	DIRECT_Little = 0,
	CRC16_Little,
	XOR8_Little,
	XOR8_Big,
	KPRO_FORMAT_SIZE
}Type_KproFormat;


typedef struct {
	//Rx
	uint16_t rxLink 		:1;
	uint16_t rxLinkLed :1;
	uint16_t rxOn	:1;
	//
	uint32_t rxEvLinkEnd;
	uint32_t rxEvPacketEnd;
	//
	uint32_t rxPacketLeng;
	//
	//uint32_t rxDataSu;	
	Type_Q rxQ;
	uint8_t rxBuf[RX_BUF_SIZE];
	
	//Tx
	uint32_t txDataSu;	
	Type_Q txQ;
	uint8_t txBuf[TX_BUF_SIZE];
	
}Type_Kpro;


uint16_t kpro_process(Type_Kpro *Kpro, UART_HandleTypeDef  *huart, Type_KproFormat format);
void kpro_response(Type_Kpro * Kpro, Type_KproFormat format);

uint16_t kpro_GetRxLink(Type_Kpro * Kpro);
uint16_t kpro_GetRxLinkLed(Type_Kpro * Kpro);

void kprintf(uint16_t enable, Type_Kpro *Kpro, const uint8_t *fmt,...);



#ifdef __cplusplus
}
#endif


#endif

