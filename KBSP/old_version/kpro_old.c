/*-----------------------------------------
by Kang Woo Jung

2016.12.13

-------------------------------------------*/
#include <stdio.h>
#include <stdarg.h>

#include "klib.h"
#include "usart.h"/////////////////////


uint16_t  kpro_process(Type_Kpro *Kpro, UART_HandleTypeDef  *huart, Type_KproFormat format)
{
	uint8_t Start, End;
	uint16_t i, code, code_op, data_size;
	
	/* Receive to Q */
	static uint8_t rx_dat;
	if(HAL_OK == HAL_UART_Receive_DMA(&huart1, &rx_dat, 1)){
		Q_Put(&Kpro->rxQ, rx_dat);
	}	

    /* Rx Link Check */
	if(Q_GetSize(&Kpro->rxQ) != Kpro->rxPacketLeng)
	{
		Kpro->rxPacketLeng = Q_GetSize(&Kpro->rxQ);
		Kpro->rxOn = 1;
		Kpro->rxLink = 1;
		evt_reset(&Kpro->rxEvLinkEnd);
		evt_reset(&Kpro->rxEvPacketEnd);
	}
	
    /* Rx LinkEnd Check (check timeout)*/
	if(evt_out(&Kpro->rxEvLinkEnd, KPRO_LINK_OUT))
	{
		Kpro->rxLink = 0;
		Kpro->rxLinkLed = 0;
	}


	data_size = 0; // 중요

    /* Get Receive Data */
	if(Kpro->rxOn && evt_out(&Kpro->rxEvPacketEnd, KPRO_TIME_OUT)){ // 통신Link와 Time out 검출?
		Kpro->rxLinkLed ^= 1;

		// Get Start code
		Start = Q_Get(&Kpro->rxQ);
		Kpro->rxPacketLeng--;
		// Get Data+Check+End
		for(i=0 ; i < Kpro->rxPacketLeng ; i++){
			Kpro->rxBuf[i] = Q_Get(&Kpro->rxQ);
		}
		// format check
		switch(format){
		case DIRECT_Little:
			data_size = Kpro->rxPacketLeng;
			break;
		case CRC16_Little:
			// Get End code
			Kpro->rxPacketLeng--;
			End = Kpro->rxBuf[Kpro->rxPacketLeng];
			// Get Check code
			Kpro->rxPacketLeng-=2;
			code = Kpro->rxBuf[Kpro->rxPacketLeng];
			/* Receive Data Error check */ 
			code = (*( uint16_t *)(&Kpro->rxBuf[Kpro->rxPacketLeng]));
			code_op = CRC16_Buf(CRC16_INIT_DAT, Kpro->rxBuf, Kpro->rxPacketLeng);
			if(code == code_op)	 data_size = Kpro->rxPacketLeng;
			break;
		case XOR8_Little:
		case XOR8_Big:
			// Get End code
			Kpro->rxPacketLeng--;
			End = Kpro->rxBuf[Kpro->rxPacketLeng];
			// Get Check code
			Kpro->rxPacketLeng--;
			code = Kpro->rxBuf[Kpro->rxPacketLeng];
			/* Receive Data Error check */
			code_op = XOR8(XOR8_INIT_DAT, Start);
			code_op = XOR8_Buf(code_op, Kpro->rxBuf, Kpro->rxPacketLeng);
			if(code == code_op)	data_size = Kpro->rxPacketLeng;
			break;
		default:
			break;
		}
			
		/* Inital 중요! */
		Kpro->rxOn = 0;
		Kpro->rxPacketLeng = 0;
		Q_Clear(&Kpro->rxQ);
    }
	
	return data_size;
}


void kpro_response(Type_Kpro * Kpro, Type_KproFormat format)
{
	static uint8_t buf[TX_BUF_SIZE+10];
	uint16_t code, size;
	
	// 메세지 처리 kprintf()의 TxQ Data --> DMA Buffer
	size = Q_GetSize(&Kpro->txQ);
	for(int i=0 ; i<size ; i++){
		buf[i] = Q_Get(&Kpro->txQ);
	}
	
	// 데이타처리  Kpro->txBuf + Format --> DMA buffer
	switch(format){
	case DIRECT_Little:
		for(uint16_t i=0; i < Kpro->txDataSu ; i++){
			buf[size++] = Kpro->txBuf[i];
		}
		break;
	case CRC16_Little:
		buf[size++] = START_CODE;
		for(uint16_t i=0; i < Kpro->txDataSu ; i++){
			buf[size++] = Kpro->txBuf[i];
		}
		code = CRC16_Buf(CRC16_INIT_DAT, Kpro->txBuf,  Kpro->txDataSu);//CRC16 Cal
		buf[size++] = code & 0xff;	//CRC Low
		buf[size++] = code >> 8;	//CRC High
		buf[size++] = END_CODE;
		break;
	case XOR8_Little:
	case XOR8_Big:
		buf[size++] = START_CODE;
		for(uint16_t i=0; i < Kpro->txDataSu ; i++){
			buf[size++] = Kpro->txBuf[i];
		}
		code = XOR8(XOR8_INIT_DAT,  START_CODE);
		code = XOR8_Buf(code, Kpro->txBuf,  Kpro->txDataSu); //XOR8 Cal
		buf[size++] = code;
		buf[size++] = END_CODE;
		break;
	default:
		break;
	}
	
	// Tx DMA 실행!
	HAL_UART_Transmit_DMA(&huart1, buf, size); ////////////////////////////////////
}



uint16_t kpro_GetRxLink(Type_Kpro * Kpro)
{
	return (Kpro->rxLink);
}

uint16_t kpro_GetRxLinkLed(Type_Kpro * Kpro)
{
	return (Kpro->rxLinkLed);
}


void kprintf(uint16_t enable, Type_Kpro *Kpro, const uint8_t *fmt,...)
{
	if(enable==0) return; //
	
	uint8_t str[256];
	va_list ap;
	
	va_start(ap, fmt);
	vsprintf((char *) str,(const char *)fmt, ap);
	va_end(ap);
	
	int16_t i = 0;
	while(str[i]) Q_Put(&Kpro->txQ, str[i++]);
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


