/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/
#include "bsp.h"//////////////
#include "clcd_spi.h"

/* SPI Tx Data Map
bit0 : Blink
bit1 : RS
bit2 : WR
bit3 : E
bit4~bit7 : DATA nible
*/

void clcd_spi_write_nible(uint8_t buf)
{
	SPI2_NSS = 0;
	buf |= 0x8; // E set
//	while(HAL_SPI_Transmit_IT(&hspi2,&buf,1) != HAL_OK);
	HAL_Delay(1);
	SPI2_NSS = 1;
	//
	SPI2_NSS = 0;
	buf &= (~0x8); // E clear
//	while(HAL_SPI_Transmit_IT(&hspi2,&buf,1) != HAL_OK);
	HAL_Delay(1);
	SPI2_NSS = 1;
}

void clcd_spi_4bit_mode(void)
{
	clcd_spi_write_nible(0x29);
	clcd_spi_write_nible(0x29);
	clcd_spi_write_nible(0xc9);
}

void clcd_spi_command(uint8_t data)
{
	clcd_spi_write_nible((data&0xf0)|0x9);
	clcd_spi_write_nible((data<<4)|0x9);
}

void clcd_spi_data(uint8_t data)
{
	clcd_spi_write_nible((data&0xf0)|0xb);
	clcd_spi_write_nible((data<<4)|0xb);
}

void clcd_spi_init(void)
{
  //lcd_command(0x3c); // Function : 8bit mode, display on
	clcd_spi_4bit_mode(); // 4bit mode, display on
	clcd_spi_command(0x0c); //display on, cursor off, blink off
	clcd_spi_command(0x01); //display clear all
	clcd_spi_command(0x06); //entry mode set
}


void clcd_spi_process(clcd_spi_t *clcd)
{
	// SPI M/C
	static uint16_t spi_mc;
	switch(spi_mc){
	case 0:
		SPI2_NSS = 0;
		//if(HAL_SPI_TransmitReceive_IT(&hspi2, clcd->spi_tx, clcd->spi_rx, 1) == HAL_OK) spi_mc++;
		break;
	case 1:
		SPI2_NSS = 1;
	  spi_mc = 0;
		break;
	default:
		spi_mc = 0;
		break;
	}


	// LCD M/C (BUF --> LCD)
	static uint16_t cnt, lcd_mc, index;
	if(++cnt<3) return;
	cnt = 0;
	switch(lcd_mc){
	// Line1 Adrr
	case 0:	clcd->spi_tx[0] = 0x89; lcd_mc++;	break; //E=1
	case 1:	clcd->spi_tx[0] = 0x81; lcd_mc++;	break; //E=0
	case 2:	clcd->spi_tx[0] = 0x09; lcd_mc++;	break; //E=1
	case 3:	clcd->spi_tx[0] = 0x01; lcd_mc++;	break; //E=0
	// Line1 Data 16
	case 4:	clcd->spi_tx[0] = (clcd->buf[index]&0xf0)|0xb; lcd_mc++;	break; //E=1
	case 5:	clcd->spi_tx[0] = (clcd->buf[index]&0xf0)|0x3; lcd_mc++;	break; //E=0
	case 6:	clcd->spi_tx[0] = (clcd->buf[index]<<4)|0xb; lcd_mc++;	break; //E=1
	case 7:
		clcd->spi_tx[0]= (clcd->buf[index]<<4)|0x3; //E=0
		if(++index<CLCD_SPI_LINE_LEN) lcd_mc = 4;
		else {
			index = CLCD_SPI_LINE2;
			lcd_mc = 10;
		}
		break;
	// Line2 Adrr
	case 10:	clcd->spi_tx[0] = 0xc9; lcd_mc++;	break; //E=1
	case 11:	clcd->spi_tx[0] = 0xc1; lcd_mc++;	break; //E=0
	case 12:	clcd->spi_tx[0] = 0x09; lcd_mc++;	break; //E=1
	case 13:	clcd->spi_tx[0] = 0x01; lcd_mc++;	break; //E=0
	// Line2 Data 16
	case 14:	clcd->spi_tx[0] = (clcd->buf[index]&0xf0)|0xb; lcd_mc++;	break; //E=1
	case 15:	clcd->spi_tx[0] = (clcd->buf[index]&0xf0)|0x3; lcd_mc++;	break; //E=0
	case 16:	clcd->spi_tx[0] = (clcd->buf[index]<<4)|0xb; lcd_mc++;	break; //E=1
	case 17:
		clcd->spi_tx[0] = (clcd->buf[index]<<4)|0x3; //E=0
		if(++index<(CLCD_SPI_LINE2+CLCD_SPI_LINE_LEN)) lcd_mc = 14;
		else {
			index = CLCD_SPI_LINE1;
			lcd_mc = 0;
		}
		break;
	default :
		index = CLCD_SPI_LINE1;
		lcd_mc = 0;
		break;
	}	
}

void clcd_spi_clear(clcd_spi_t *clcd){
	for(uint16_t i=0 ; i<CLCD_SPI_BUF_SIZE ; i++)	clcd->buf[i] = ' ';
}



