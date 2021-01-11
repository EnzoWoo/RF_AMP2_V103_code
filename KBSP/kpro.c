/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/
#include "usr.h" ////////////////////////
#include "bsp.h"
#include "kpro.h"


void kpro_callback(kpro_t *kpro, uint8_t dat)
{
	kpro->rx_buf[kpro->rx_index++] = dat;
	if(kpro->rx_index == RX_BUFF_SIZE) kpro->rx_index--;
	kpro->timeout = 0;
	kpro->connect_timeout = 0;
}


void kpro_init(kpro_t *kpro, UART_HandleTypeDef *huart, kpro_endian_t endian, kpro_check_t check_method)
{
	kpro->huart = huart;
	kpro->endian = endian;
	kpro->check_method = check_method;
	kpro->rx_index = 0;
	kpro->rx_index_get = 0;
	kpro->tx_index = 0;
	kpro->tx_index_stx = 0;
	kpro->timeout = 0;
	kpro->connect_timeout = 0;
	kpro->connect = 0;
}


uint16_t kpro_receive_size(kpro_t *kpro, uint16_t size)
{
	if(kpro->rx_buf[0] != 0x02){
		kpro->rx_index = 0;	
		return 0;
	}
	if(kpro->rx_index < size)	return 0;

	kpro->rx_index = 0;
	kpro->rx_index_get = 1;
	
	kpro->tx_index_stx = kpro->tx_index; // Update new STX_index!!!
	kpro->tx_index++;	//Skip STX's record location (kprintf finish) !!!!!!! 2020-04-03
	
	uint16_t crc = (*(uint16_t*)&kpro->rx_buf[size-3]);
	if(crc16_buf(0xffff, kpro->rx_buf+1, size-4) == crc){
		return 1;
	}
	else return 0;
}

uint16_t kpro_receive_time(kpro_t *kpro, uint16_t timeout_ms, uint16_t connect_timeout_ms)
{
	kpro->connect_timeout += looptick_ms;
	if(kpro->connect_timeout >= connect_timeout_ms){
		kpro->connect_timeout = connect_timeout_ms;
		kpro->connect = 0;
	}

	if(kpro->rx_index == 0)	return 0;
	
	//kpro->timeout += looptick_ms;
	//if(kpro->timeout < timeout_ms)	return 0;
	kpro->timeout += looptick_us;
	if(kpro->timeout < (timeout_ms*1000)) return 0;

	kpro->connect = 1;
	kpro->timeout = 0;
	kpro->connect_timeout = 0;
	
	if(kpro->rx_buf[0] != 0x02){
		kpro->rx_index = 0;
		kpro->rx_buf[0] = 0xff; // clear STX !!!;
		return 0;
	}
	kpro->rx_buf[0] = 0xff; // clear STX !!!;

	
	uint16_t complite = 0;
	uint16_t size = kpro->rx_index;
	
	if(kpro->check_method == KP_CRC16){
		//uint16_t crc = (*(uint16_t*)&kpro->rx_buf[size-3]); // 3 = CRC(2) + ETX(1)
		uint16_t crc;
		kpro->rx_index_get = kpro->rx_index - 3; // 3 = CRC(2) + ETX(1)
		get_U16(crc);
		uint16_t cal = crc16_buf(KP_CRC16_INIT, kpro->rx_buf+1, size-4);  // 4 =  STX(1) + CRC(2) + ETX(1)
		//uprintf(&huart2, (cu8*)"* crc=%04X, cal=%04X\n\r", crc, cal);///////////////////////////////////
		if(crc == cal) complite = 1;
	}
	if(kpro->check_method == KP_CRC32){
		//uint32_t crc = (*(uint32_t*)&kpro->rx_buf[size-5]); // 5 = CRC(4) + ETX(1)
		uint32_t crc;
		kpro->rx_index_get = kpro->rx_index - 5; // 5 = CRC(4) + ETX(1)
		get_U32(crc);
		uint32_t cal = crc32_buf(KP_CRC32_INIT, kpro->rx_buf+1, size-6);  // 6 =  STX(1) + CRC(4) + ETX(1)
		//uprintf(&huart2, (cu8*)"* crc=%08X, cal=%08X\n\r", crc, cal);///////////////////////////////////
		if(crc == cal) complite = 1;
	}
	else if(kpro->check_method == KP_SUM8){
		//uint8_t sum = kpro->rx_buf[size-2];  // 2 =  SUM(1) + ETX(1)
		uint8_t sum;
		kpro->rx_index_get = kpro->rx_index - 2;  // 2 =  SUM(1) + ETX(1)
		get_U8(sum);
		uint8_t cal = sum8_buf(KP_SUM8_INIT, kpro->rx_buf+1, size-3);  // 3 =  STX(1) + SUM(1) + ETX(1)
		//uprintf(&huart2, (cu8*)"* sum=%02X, cal=%02X\n\r", sum, cal);///////////////////////////////////
		if(sum == cal) complite = 1;
	}
	else if(kpro->check_method == KP_SUM16){
		//uint16_t sum = (*(uint16_t*)&kpro->rx_buf[size-3]);  // 3 = SUM(2) + ETX(1)
		uint16_t sum;
		kpro->rx_index_get = kpro->rx_index - 3;  // 3 = SUM(2) + ETX(1)
		get_U16(sum);
		uint16_t cal = sum16_buf(KP_SUM16_INIT, kpro->rx_buf+1, size-4);  // 4 =  STX(1) + SUM(2) + ETX(1)
		//uprintf(&huart2, (cu8*)"* sum=%04X, cal=%04X\n\r", sum, cal);///////////////////////////////////
		if(sum == cal) complite = 1;
	}
	else if(kpro->check_method == KP_XOR8){
		//uint8_t XOR = kpro->rx_buf[size-2];  // 2 =  XOR(1) + ETX(1)
		uint8_t XOR;
		kpro->rx_index_get = kpro->rx_index - 2;  // 2 =  XOR(1) + ETX(1)
		get_U8(XOR);
		uint8_t cal = xor8_buf(KP_XOR8_INIT, kpro->rx_buf+1, size-3);  // 3 =  STX(1) + XOR(1) + ETX(1)
		//uprintf(&huart2, (cu8*)"* xor=%02X, cal=%02X\n\r", XOR, cal);///////////////////////////////////
		if(XOR == cal) complite = 1;
	}

	kpro->rx_index = 0;
	kpro->rx_index_get = 1;
	
	//for Response
	kpro->tx_index_stx = kpro->tx_index; // Update new STX_index!!!
	kpro->tx_index++;  // Skip STX's record location (kprintf finish) !!!!!!! 2020-04-03
	//
	return complite;
}

void kpro_transmit(kpro_t *kpro)
{
/*
	"Update new STX_index" & "Skip STX's record location" 은
	kpro_receive_xxxx() 에서 처리함!
*/
	
	uint16_t check_start_index = kpro->tx_index_stx + 1;
	uint16_t check_size = kpro->tx_index - check_start_index;
	
	//put_U8(kpro->tx_buf, kpro->tx_index_stx, 0x02); //STX
	put_U8_STX(0x02); //STX

	if(kpro->check_method == KP_CRC16){
		uint16_t crc = crc16_buf(KP_CRC16_INIT, &kpro->tx_buf[check_start_index], check_size);
		put_U16(crc);
		//uprintf(&huart2, (cu8*)"* crc=%08X\n\r", crc);///////////////////////////////////
	}
	if(kpro->check_method == KP_CRC32){
		uint32_t crc = crc32_buf(KP_CRC32_INIT, &kpro->tx_buf[check_start_index], check_size);
		put_U32(crc);
		//uprintf(&huart2, (cu8*)"* crc=%08X\n\r", crc);///////////////////////////////////
	}
	else if(kpro->check_method == KP_SUM8){
		uint8_t sum = sum8_buf(KP_SUM8_INIT, &kpro->tx_buf[check_start_index], check_size);
		put_U8(sum);
		//uprintf(&huart2, (cu8*)"* sum=%02X\n\r", sum);///////////////////////////////////
	}	
	else if(kpro->check_method == KP_SUM16){
		uint16_t sum = sum16_buf(KP_SUM16_INIT, &kpro->tx_buf[check_start_index], check_size);
		put_U16(sum);
		//uprintf(&huart2, (cu8*)"* sum=%04X\n\r", sum);///////////////////////////////////
	}	
	else if(kpro->check_method == KP_XOR8){
		uint8_t XOR = xor8_buf(KP_XOR8_INIT, &kpro->tx_buf[check_start_index], check_size);
		put_U8(XOR);
	  //uprintf(&huart2, (cu8*)"* xor=%02X\n\r", XOR);///////////////////////////////////
	}
	
	put_U8(0x03); //ETX
	
	for(uint16_t i=0 ; i<kpro->tx_index ; i++) kpro->tx_dmabuf[i] = kpro->tx_buf[i];
	HAL_UART_Transmit_DMA(kpro->huart, kpro->tx_dmabuf, kpro->tx_index);
	kpro->tx_index = 0; //!!!
	kpro->tx_index_stx = 0; //!!!
}


void kprintf(kpro_t *kpro, const uint8_t *fmt,...)
{
	uint8_t str[128];
	va_list ap;
	
	va_start(ap, fmt);
	vsprintf((char *) str,(const char *)fmt, ap);
	va_end(ap);

	uint8_t *s = str;
	while(*s){
		kpro->tx_buf[kpro->tx_index++] = *s++;
	}
}


//----------------------------------------- for printf()
/* printf() 포맷 지정자
%c    charactor
%d    부호있는 int
%e, %E    과학적표기법
%f      float
%g, %G    %e,%f 중 단순한 것으로 선택해서 출력
%s   string
%u   부호없는 int
%x   hex
%X   HEX
%p   pointer
*/


