/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/

#ifndef __KPRO_H__
#define __KPRO_H__ 

#include "ktype.h"
#include "kq.h"
#include "event.h"

/* Format
	1. STX,DATA0,DATA1, ..... ,DATAn, CRC_L, CRC_H,ETX
	2. CRC Cal = DATA only
	3. Little endian
*/

#define KP_CRC16_INIT		0xffff
#define KP_CRC32_INIT		0xffffffff
#define KP_SUM8_INIT		0x00
#define KP_SUM16_INIT		0x0000
#define KP_XOR8_INIT		0x00
#define KP_XOR16_INIT		0x0000

typedef enum {
	KP_Little_Endian=0,
	KP_Big_Endian
} kpro_endian_t;

typedef enum {
	KP_DIRECT=0,
	KP_CRC16,
	KP_CRC32,
	KP_SUM8,
	KP_SUM16,
	KP_XOR8,
	KP_XOR16,
	KP_END
} kpro_check_t;

#define RX_BUFF_SIZE  128
#define TX_BUFF_SIZE  512

typedef struct {
	UART_HandleTypeDef *huart;
	uint16_t endian;
	uint16_t check_method;
	uint16_t timeout;

	uint16_t rx_index_get;
	uint16_t rx_index;
	uint8_t rx_buf[RX_BUFF_SIZE];
	
	uint16_t tx_index_stx;
	uint16_t tx_index;
	uint8_t tx_buf[TX_BUFF_SIZE];
	uint8_t tx_dmabuf[TX_BUFF_SIZE];
	
	uint16_t connect_timeout;
	uint16_t connect;
	
}kpro_t;

//Endian Little and Big
#define get_U8(data) data = kpro->rx_buf[kpro->rx_index_get++];
#define get_U16(data){\
	if(kpro->endian == KP_Little_Endian){\
		((uint8_t*) &data)[0] = kpro->rx_buf[kpro->rx_index_get++];\
		((uint8_t*) &data)[1] = kpro->rx_buf[kpro->rx_index_get++];\
	}\
	else{\
		((uint8_t*) &data)[1] = kpro->rx_buf[kpro->rx_index_get++];\
		((uint8_t*) &data)[0] = kpro->rx_buf[kpro->rx_index_get++];\
	}\
}
#define get_U32(data){\
	if(kpro->endian == KP_Little_Endian){\
		((uint8_t*) &data)[0] = kpro->rx_buf[kpro->rx_index_get++];\
		((uint8_t*) &data)[1] = kpro->rx_buf[kpro->rx_index_get++];\
		((uint8_t*) &data)[2] = kpro->rx_buf[kpro->rx_index_get++];\
		((uint8_t*) &data)[3] = kpro->rx_buf[kpro->rx_index_get++];\
	}\
	else{\
		((uint8_t*) &data)[3] = kpro->rx_buf[kpro->rx_index_get++];\
		((uint8_t*) &data)[2] = kpro->rx_buf[kpro->rx_index_get++];\
		((uint8_t*) &data)[1] = kpro->rx_buf[kpro->rx_index_get++];\
		((uint8_t*) &data)[0] = kpro->rx_buf[kpro->rx_index_get++];\
	}\
}

#define put_U8_STX(data) kpro->tx_buf[kpro->tx_index_stx] = data;
#define put_U8(data) kpro->tx_buf[kpro->tx_index++] = data;
#define put_U16(data){\
	if(kpro->endian == KP_Little_Endian){\
		kpro->tx_buf[kpro->tx_index++] = ((uint8_t*) &data)[0];\
		kpro->tx_buf[kpro->tx_index++] = ((uint8_t*) &data)[1];\
	}\
	else{\
		kpro->tx_buf[kpro->tx_index++] = ((uint8_t*) &data)[1];\
		kpro->tx_buf[kpro->tx_index++] = ((uint8_t*) &data)[0];\
	}\
}
#define put_U32(data){\
	if(kpro->endian == KP_Little_Endian){\
		kpro->tx_buf[kpro->tx_index++] = ((uint8_t*) &data)[0];\
		kpro->tx_buf[kpro->tx_index++] = ((uint8_t*) &data)[1];\
		kpro->tx_buf[kpro->tx_index++] = ((uint8_t*) &data)[2];\
		kpro->tx_buf[kpro->tx_index++] = ((uint8_t*) &data)[3];\
	}\
	else{\
		kpro->tx_buf[kpro->tx_index++] = ((uint8_t*) &data)[3];\
		kpro->tx_buf[kpro->tx_index++] = ((uint8_t*) &data)[2];\
		kpro->tx_buf[kpro->tx_index++] = ((uint8_t*) &data)[1];\
		kpro->tx_buf[kpro->tx_index++] = ((uint8_t*) &data)[0];\
	}\
}

void kpro_callback(kpro_t *kpro, uint8_t dat);
void kpro_init(kpro_t *kpro, UART_HandleTypeDef *huart, kpro_endian_t endian, kpro_check_t check_method);
uint16_t kpro_receive_size(kpro_t *kpro, uint16_t size); // size = paket size (include STX,DATA's,CRC,ETX)
uint16_t kpro_receive_time(kpro_t *kpro, uint16_t timeout_ms, uint16_t connect_timeout_ms); // timeover ms
void kpro_transmit(kpro_t *kpro);
void kprintf(kpro_t *kpro, const uint8_t *fmt,...);


#endif

