/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/

#ifndef __CLCD_SPI_H__
#define __CLCD_SPI_H__

#include "kType.h"

/*
  SPI Clock = 1.25MHz
*/
#define SPI2_NSS  PortD.b0
/*
SPI Tx Data Map
	bit0 : Blink
	bit1 : RS
	bit2 : WR
	bit3 : E
	bit4~bit7 : DATA nible
	
SPI Rx Data Map
	bit0 : ENCODER A
	bit1 : ENCODER B
	bit2 : ENCODER SW
	bit3 : NC
	bit4~bit7 : DIP SW
*/

#define  CLCD_SPI_LINE_LEN   16
#define  CLCD_SPI_LINE_NUM   2
#define  CLCD_SPI_LINE1      0
#define  CLCD_SPI_LINE2     (CLCD_SPI_LINE_LEN+16)
#define  CLCD_SPI_BUF_SIZE  (CLCD_SPI_LINE2*CLCD_SPI_LINE_NUM)

typedef struct {
	uint8_t blink :1;
	uint8_t rs :1;
	uint8_t wr :1;
	uint8_t e :1;
	uint8_t data :4;
}spi_tx_t;

typedef struct {
	uint8_t enc_ab :2;
	uint8_t enc_sw :1;
	uint8_t nc :1;
	uint8_t dip :4;
}spi_rx_t;

typedef struct {
	uint8_t spi_tx[1];
	uint8_t spi_rx[1];
	uint8_t buf[CLCD_SPI_BUF_SIZE];
}clcd_spi_t;


void clcd_spi_init(void);
void clcd_spi_process(clcd_spi_t *clcd);
void clcd_spi_clear(clcd_spi_t *clcd);



#endif



