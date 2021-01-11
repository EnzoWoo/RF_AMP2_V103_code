
/*-----------------------------------------
by Kang Woo Jung

2016.06.24

-------------------------------------------*/

#ifndef _AD9834_H_
#define _AD9834_H_

#include "ktype.h"

/* GPIO pins */
#define PSEL_PIN_OUT    	// Add your code here
#define FSEL_PIN_OUT    	// Add your code here
#define PSEL_LOW        	// Add your code here
#define PSEL_HIGH       	// Add your code here
#define FSEL_LOW        	// Add your code here
#define FSEL_HIGH       	// Add your code here


/* Registers */
#define AD9834_REG_CMD		(0 << 14)
#define AD9834_REG_FREQ0	(1 << 14)
#define AD9834_REG_FREQ1	(2 << 14)
#define AD9834_REG_PHASE0	(6 << 13)
#define AD9834_REG_PHASE1	(7 << 13)

/* Command Control Bits */

#define AD9834_B28			(1 << 13)
#define AD9834_HLB			(1 << 12)
#define AD9834_FSEL0		(0 << 11)
#define AD9834_FSEL1		(1 << 11)
#define AD9834_PSEL0		(0 << 10)
#define AD9834_PSEL1		(1 << 10)
#define AD9834_CMD_PIN		(1 << 9)
#define AD9834_CMD_SW		(0 << 9)
#define AD9834_RESET		(1 << 8)
#define AD9834_SLEEP1		(1 << 7)
#define AD9834_SLEEP12		(1 << 6)
#define AD9834_OPBITEN		(1 << 5)
#define AD9834_SIGN_PIB		(1 << 4)
#define AD9834_DIV2			(1 << 3)
#define AD9834_MODE			(1 << 1)

#define AD9834_OUT_SINUS	((0 << 5) | (0 << 1))
#define AD9834_OUT_TRIANGLE	((0 << 5) | (1 << 1))



u8 AD9834_Init(void); // Initializes the SPI communication peripheral and resets the part

void AD9834_Reset(void);	//	Sets the Reset bit of the AD9834

void AD9834_ClearReset(void);		// Clears the Reset bit of the AD9834

void AD9834_SetRegisterValue(u16 regValue);	// Writes the value to a register

////////////////////////////////////////////////// f = ¥ÄPhase ¡¿ fMCLK/2^28,  Á¶°Ç0 < ¥ÄPhase < 2^28-1  kwj 160622
void AD9834_SetFrequency(u16 reg, u32 val);	// Writes to the frequency register

void AD9834_SetPhase(u16 reg, u16 val); // Writes to the phase registers

void AD9834_Setup(u16 freq, u16 phase); // Selects the Frequency,Phase and Waveform type

					
					
#endif // _AD9834_H
					
				
