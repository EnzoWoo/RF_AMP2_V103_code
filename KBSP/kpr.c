/*---------------------------------------------------------------------
by Kang Woo Jung: 2019.01.09

-----------------------------------------------------------------------*/
#include "usr.h" ////////////////////////
#include "bsp.h"

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1) {
		static uint8_t dr;
    HAL_UART_Receive_IT(&huart1,&dr,1); // ** rd는 전역변수여야함 !!!
		evus_rst(&kpro1.evus);
		q_put(&kpro1.qrx, dr);
		kpro1.timeout = 0;
		//HAL_UART_Transmit_IT(&huart1,&kpro1.dr,1); ////////////////
  }
  else if (huart->Instance == USART2) {
		static uint8_t dr;
    HAL_UART_Receive_IT(&huart2,&dr,1); // ** rd는 전역변수여야함 !!!
		q_put(&kpro2.qrx, dr);
		evus_rst(&kpro2.evus);
		kpro2.timeout = 0;
		//HAL_UART_Transmit_IT(&huart1,&kpro2.dr,1); ///////////////////
	}
}

uint16_t kpro_receive_size(kpro_t *kpro, UART_HandleTypeDef *huart, uint16_t size)
{
	//static uint8_t dr;
	//if(HAL_UART_Receive_IT(huart,&dr,1) == HAL_OK)	q_put(&kpro->qrx, dr);
	//
	if(kpro->busy==0){
		if(q_get(&kpro->qrx) == 0x02) kpro->busy = 1;
	}
	else {
		if(q_size(&kpro->qrx) < size-1) return 0;
		kpro->busy = 0;
		//
		uint16_t size = 0;
		while(q_size(&kpro->qrx))	kpro->bufrx[size++] = q_get(&kpro->qrx);
		uint16_t crc = kpro->bufrx[size-3];
		crc |= (kpro->bufrx[size-2]<<8);
		if(crc16_buf(0xffff,kpro->bufrx,size-3) == crc)	return 1;
	}
	return 0;
}

/* timeout 설정 잫 할 것!!!! 대단히 중요! mcu폭주도 가능! */
uint16_t kpro_receive_loop(kpro_t *kpro, UART_HandleTypeDef *huart, uint16_t timeout)
{
/*
	static uint8_t dr;
	if(HAL_OK == HAL_UART_Receive_IT(huart,&dr,1)){	q_put(&kpro->qrx, dr); kpro->timeout = 0; }
*/	
	if(q_size(&kpro->qrx) == 0){	kpro->timeout = 0; return 0; }
	//
  kpro->timeout++;
	if(kpro->timeout < timeout) return 0;
	kpro->timeout = 0;
	//
	if(q_get(&kpro->qrx) != 0x02){	q_clear(&kpro->qrx);	return 0;	}
	uint16_t size = 0;
	while(q_size(&kpro->qrx))	kpro->bufrx[size++] = q_get(&kpro->qrx);
	uint16_t crc = kpro->bufrx[size-3];
	crc |= (kpro->bufrx[size-2]<<8);
	if(crc16_buf(0xffff,kpro->bufrx,size-3) == crc)	return 1;
	return 0;
}

/* timeout 설정 잫 할 것!!!! 대단히 중요! mcu폭주도 가능! */
uint16_t kpro_receive_time(kpro_t *kpro, UART_HandleTypeDef *huart, uint16_t timeout)
{
/*
	static uint8_t dr;
	if(HAL_OK == HAL_UART_Receive_IT(huart,&dr,1)){ q_put(&kpro->qrx, dr); evus_rst(&kpro->evus);	kpro->timeout = 0; }
*/	
	if(q_size(&kpro->qrx) == 0){ evus_rst(&kpro->evus); kpro->timeout = 0; return 0;	}
	//
	if(evus_evt(&kpro->evus, 1000) == 0) return 0;
  kpro->timeout++;
	if(kpro->timeout < timeout) return 0;
	kpro->timeout = 0;
	//
	if(q_get(&kpro->qrx) != 0x02){	q_clear(&kpro->qrx);	return 0;	}
	uint16_t size = 0;
	while(q_size(&kpro->qrx))	kpro->bufrx[size++] = q_get(&kpro->qrx);
	uint16_t crc = kpro->bufrx[size-3];
	crc |= (kpro->bufrx[size-2]<<8);
	if(crc16_buf(0xffff,kpro->bufrx,size-3) == crc)	return 1;
	return 0;
}

void kpro_transmit(kpro_t *kpro, UART_HandleTypeDef *huart, uint16_t size)
{
	uint16_t siz = 0;
	while(q_size(&kpro->qtx))	kpro->bufDMA[siz++] = q_get(&kpro->qtx);
	//
	kpro->bufDMA[siz++] = 0x02; //STX
	uint16_t index = 0;
	while(index < size) kpro->bufDMA[siz++] = kpro->buftx[index++];
	uint16_t crc = crc16_buf(0xffff,kpro->buftx,size);
	kpro->bufDMA[siz++] = crc&0xff;
	kpro->bufDMA[siz++] = crc>>8;
	kpro->bufDMA[siz++] = 0x03; //ETX
	//
	HAL_UART_Transmit_DMA(huart, kpro->bufDMA, siz);
}

void kputs(kpro_t *kpro, uint8_t *str)
{
	while(*str){
		q_put(&kpro->qtx, *str);
		str++;
	}
}

void kprintf(kpro_t *kpro, const uint8_t *fmt,...)
{
	uint8_t str[128];
	va_list ap;
	
	va_start(ap, fmt);
	vsprintf((char *) str,(const char *)fmt, ap);
	va_end(ap);
	
	kputs(kpro, &str[0]);
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


