
/*-----------------------------------------
by Kang Woo Jung

2016.06.24

-------------------------------------------*/

#include "AD9834.h"
#include "main.h"


u8 AD9834_Init(void)
{
    //SPI_Init(0, 1000000, 1, 1);
    AD9834_SetRegisterValue(AD9834_REG_CMD | AD9834_RESET);
	
    return (1);
}

void AD9834_Reset(void)
{
    AD9834_SetRegisterValue(AD9834_REG_CMD | AD9834_RESET);
}

void AD9834_ClearReset(void)
{
	  AD9834_SetRegisterValue(AD9834_REG_CMD);
}

void AD9834_SetRegisterValue(u16 regValue)
{
/*
	u8 data[5] = {0x03, 0x00, 0x00};	
	
	data[1] = (u8)((regValue & 0xFF00) >> 8);
	data[2] = (u8)((regValue & 0x00FF) >> 0);
	
	ADI_CS_LOW;	    
	SPI_Write(data,2);
	ADI_CS_HIGH;
*/
	//U1_printf("SetVAL: 0x%0.4x \n\r", regValue);////////////////////////
	//SPI3_nCS_Low();
	Spi_Write(SPI3, regValue);
	//SPI3_nCS_High();
}


////////////////////////////////////////////////// f = ¥ÄPhase ¡¿ fMCLK/2^28,  Á¶°Ç0 < ¥ÄPhase < 2^28-1  kwj 160622
void AD9834_SetFrequency(u16 reg, u32 val)
{
	u16 freqHi = reg;
	u16 freqLo = reg;
/*	
	FSEL_PIN_OUT;		// Declare FSEL pin as output
	PSEL_PIN_OUT;		// Declare PSEL pin as output
	PSEL_LOW;
	FSEL_LOW;
*/
	
	freqHi |= (val & 0xFFFC000) >> 14 ;
	freqLo |= (val & 0x3FFF);

	SPI3_nCS_Low();
	DelayUs(1);
	AD9834_SetRegisterValue(AD9834_B28|AD9834_FSEL0|AD9834_RESET|AD9834_OPBITEN|AD9834_DIV2); 
	
//	AD9834_SetRegisterValue(AD9834_B28|AD9834_FSEL0|AD9834_OPBITEN|AD9834_SIGN_PIB|AD9834_DIV2); 
	AD9834_SetRegisterValue(AD9834_B28|AD9834_FSEL0|AD9834_OPBITEN|AD9834_DIV2); 
//	AD9834_SetRegisterValue(AD9834_B28|AD9834_FSEL0); 
//	AD9834_SetRegisterValue(AD9834_B28);
	AD9834_SetRegisterValue(freqLo);
	AD9834_SetRegisterValue(freqHi);

AD9834_SetRegisterValue(AD9834_B28|AD9834_FSEL0|AD9834_OPBITEN|AD9834_DIV2); 
	DelayUs(1);
	SPI3_nCS_Low();
	
}

void AD9834_SetPhase(u16 reg, u16 val)
{
	u16 phase = reg;
	phase |= val;
	AD9834_SetRegisterValue(phase);
}

void AD9834_Setup(u16 freq,  u16 phase)
{
	u16 val = 0;
	val = freq | phase;
	AD9834_SetRegisterValue(val);

}
