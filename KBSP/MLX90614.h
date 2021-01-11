
/*-----------------------------------------
by Kang Woo Jung

2015.11.25

-------------------------------------------*/

#ifndef __MLX90614_H__
#define __MLX90614_H__

#include "ktype.h"

#define MLX90614_RAM    0x00
#define MLX90614_EEPROM 0x40

#define MLX90614_TOBJ1  (MLX90614_RAM+0x07)
#define MLX90614_PWMCTR (MLX90614_EEPROM+0x02)





u16 MLX90614_Read(u8 Command);
void MLX90614_Write(u8 Command, u16 Data);




//void MLX90614_Test(void){
/*
        printf("\n\r * MLX90614 ---> PWM \n\r");
        MLX90614_Write(0x22, 0x0000);////////////
        DelayMs(10);/////////////////////
        MLX90614_Write(0x22, 0x0203);////////////
        DelayMs(10);/////////////////////
*/
/*
    printf("\n\r * MLX90614 ---> I2C \n\r");
    MLX90614_Write(0x22, 0x0000);////////////
    DelayMs(10);/////////////////////
    MLX90614_Write(0x22, 0x0201);////////////
    DelayMs(10);/////////////////////
*/


/*
       
    printf("\n\r * MLX90614 ---> FLAG Set \n\r");
    MLX90614_Write(0x25, 0x0000);
    DelayMs(10);/////////////////////
		
//    MLX90614_Write(0x25, 0x97b0);   // Gain = 12.5  FIR1024 (Default.)
//    MLX90614_Write(0x25, 0xa7b0);   // Gain = 50, FIR1024
//    MLX90614_Write(0x25, 0xb7b0);   // Gain = 100, FIR1024
    MLX90614_Write(0x25, 0xb4b0);     // Gain = 100, FIR128
//    DelayMs(10);/////////////////////

*/

/*
	while(1){
		if(Ev_Out(&EvLed, 100000)){ // 10Hz

			LED1_TOGG(); //////////////

            printf("*MLX90416:");

		 Temp16 = MLX90614_Read(0x25);
           printf("    Flag:0x%0.4x", Temp16);

  		 Temp16 = MLX90614_Read(0x22);
           printf("    PWM:0x%0.4x", Temp16);

            Temp16 = MLX90614_Read(0x20);
//            printf("    MAX:0x%0.4x", Temp16);
            printf("    Max:%d", Temp16);

            Temp16 = MLX90614_Read(0x21);
//            printf("    MIN:0x%0.4x", Temp16);
            printf("    Min:%d", Temp16);


            Temp16 = MLX90614_Read(0x06);
            printf("    TA:%d", Temp16);

            Temp16 = MLX90614_Read(0x08);
//            printf("    Hex:0x%0.4x", Temp16);
            printf("    To2:%d", Temp16);

            Temp16 = MLX90614_Read(0x07);
//            printf("    To1:0x%0.4x", Temp16);
            printf("    To1:%d", Temp16);
            TempK = (float) Temp16 * 0.02f;
            printf("    K:%0.1f", TempK);
            printf("    C:%0.2f\r", TempK - 273.15f);
		    
		}
	}

}
*/            

#endif


