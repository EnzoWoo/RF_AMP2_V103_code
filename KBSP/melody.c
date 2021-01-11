
/*-----------------------------------------
by Kang Woo Jung

2014.09.22

-------------------------------------------*/

#include "Melody.h"
#include "main.h"//////////////



Type_Melo Melo;

#define MTSCK	 (MELODY_TIM_SOURCE_CLK*10)
const u16 FREQ_TBL[]={
  0x0000,
  (MTSCK/327),(MTSCK/347),		// C1, C1#
  (MTSCK/367),(MTSCK/389),		// D1, D1#
  (MTSCK/412),					// E1
  (MTSCK/437),(MTSCK/463),		// F1, F1#
  (MTSCK/490),(MTSCK/519), 		// G1, G1#
  (MTSCK/550),(MTSCK/583),		// A1, A1#
  (MTSCK/617), 					// B1
	  
  (MTSCK/654),(MTSCK/693),		// C2, C2#
  (MTSCK/734),(MTSCK/778),		// D2, D2#
  (MTSCK/824), 					// E2
  (MTSCK/873),(MTSCK/925),		// F2, F2#
  (MTSCK/980),(MTSCK/1038),		// G2, G2#
  (MTSCK/1100),(MTSCK/1166),	// A2, A2#
  (MTSCK/1225), 				// B2

  (MTSCK/1308),(MTSCK/1386),	// C3, C3#
  (MTSCK/1469),(MTSCK/1556),	// D3, D3#
  (MTSCK/1648),					// E3
  (MTSCK/1746),(MTSCK/1850),	// F3, F3#
  (MTSCK/1960),(MTSCK/2077),  	// G3, G3#
  (MTSCK/2200),(MTSCK/2331),	// A3, A3#
  (MTSCK/2469),					// B3
  
  (MTSCK/2616),(MTSCK/2772),  	// C4, C4#     /* 기준  */
  (MTSCK/2937),(MTSCK/3112),  	// D4, D4#
  (MTSCK/3297),            		// E4
  (MTSCK/3492),(MTSCK/3700),  	// F4, F4#
  (MTSCK/3920),(MTSCK/4154),  	// G4, G4#
  (MTSCK/4400),(MTSCK/4662),  	// A4, A4#
  (MTSCK/4939),            		// B4
  
  (MTSCK/5233),(MTSCK/5544),  	// C5, C5#
  (MTSCK/5874),(MTSCK/6223),  	// D5, D5#
  (MTSCK/6593),            		// E5
  (MTSCK/6984),(MTSCK/7400),  	// F5, F5#
  (MTSCK/7840),(MTSCK/8307),  	// G5, G5#
  (MTSCK/8800),(MTSCK/9324),  	// A5, A5#
  (MTSCK/9878),            		// B5
  
  (MTSCK/10465),(MTSCK/11088), 	// C6, C6#
  (MTSCK/11748),(MTSCK/12446), 	// D6, D6#
  (MTSCK/13186),            	// E6
  (MTSCK/13968),(MTSCK/14800), 	// F6, F6#
  (MTSCK/15680),(MTSCK/16614), 	// G6, G6#
  (MTSCK/17600),(MTSCK/18648), 	// A6, A6#
  (MTSCK/19756),           		// B6
  
  (MTSCK/20930),(MTSCK/22176), 	// C7, C7#
  (MTSCK/23496),(MTSCK/24892), 	// D7, D7#
  (MTSCK/26372),			 	// E7
  (MTSCK/27936),(MTSCK/29600), 	// F7, F7#
  (MTSCK/31360),(MTSCK/33228), 	// G7, G7#
  (MTSCK/35200),(MTSCK/37296), 	// A7, A7#
  (MTSCK/39512),			 	// B7
	  
  (MTSCK/41860),(MTSCK/44352), 	// C8, C8#
  0xFFFF
};

/* Example for Melody Table */
u8 MELODY_BUF[]={
  C4,M8,
  D4,M8,
  E4,M8,
  F4,M8,
  G4,M8,
  A4,M8,
  B4,M8,
  C5,M8,
  MUT,M8,
  
  C5,M4,
  D5,M4,
  E5,M4,
  F5,M4,
  G5,M4,
  A5,M4,
  B5,M4,
  C6,M4,
  VOICE_END
};


void Melody_Driver(void)
{
	if(!Melo.b.play){ return;}
	
	if(Ev_Out(&Melo.Timer,MELO_M64*1000)){///////////
		if(Melo.length){
			Melo.length--;
		}
	}
	
	if(Melo.length){/* Wait 음장 길이 만큼 기다림..*/
		if(Melo.length==1 && Ev_GetTime(&Melo.Timer) > (MELO_M64*1000*2/3)){ //같은음이 연속될 때, 음을  MELO_GTM의 2/3 부터 무음처리.
			Melody_PwmOff();
		}
		return;
	}
	
	Melody_PwmOff();
	
	if(Melo.melody[Melo.pointer] != VOICE_END){
		Melo.voice = Melo.melody[Melo.pointer]; // Get 'Voice'
		if(Melo.voice > VOICE_TOP){
			Melody_Stop();
		}
		Melo.pointer++;
		if(Melo.melody[Melo.pointer] != VOICE_END){
			Melo.length = Melo.melody[Melo.pointer]; // Get 'Length'
			if(Melo.length<M32 || Melo.length>M0){
				Melo.b.play = OFF;
			}
			Melo.pointer++;
			if(Melo.voice != MUT){
				Melo.freq_period = FREQ_TBL[Melo.voice];
				Melody_PwmOn(Melo.freq_period); // Genert 'Tone'
			}
			else {
				Melody_PwmOff(); //Mute
			}
		}
		else {
			Melody_Stop();
		}
	}
	else {
		Melody_Stop();
	}
}

void Melody_Start(u8 *melody)
{
//  if(Melo.b.play) return;
  Ev_Reset(&Melo.Timer);
  Melo.melody = melody;
  Melo.b.play = ON;
  Melo.pointer = 0;
}


void Melody_Stop(void)
{
  Melody_PwmOff();
  Melo.b.play = OFF;
  Melo.voice = 0;
  Melo.length = 0;
}
 

