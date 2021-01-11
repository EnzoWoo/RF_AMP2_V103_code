/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/
#ifndef __NTC_TABLE_H__
#define __NTC_TABLE_H__


#define NTC_TABLE_UNIT   	5.0f
#define NTC_TABLE_BOT  	-50.0f
#define NTC_TABLE_TOP  	150.0f

#define ADC_TOP	(4096.0F-1.0F)
#define Vref				3.3F		// ADC Vref
#define Vsens		3.3F	  // NTC VS
#define Rtop			10.0F  // Top side R   //[k目]
#define Rbot			4.7F	  // Bottom side R [k目]


/* NTC location is BOTTOM side.
 Vntc = NTC / (Rtop + NTC) * Vsens
 ADC = ADC_TOP / Vref * Vntc
 ADC = (Vsens / Vref) * ADC_TOP / (1 + Rtop / NTC)
*/

/* NTC-D20K (10K) */
const float NTC_D20K[NTC_TABLE_SIZE]={   //[k目]
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/169.74066F),  // -50
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/169.74066F),  // -45
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/169.74066F),  // -40
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/169.74066F),  // -35
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/169.74066F),  // -30
  	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/127.08017F),  // -25
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/95.77289F),  // -20
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/72.65589F),  // -15
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/55.48034F),  // -10
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/42.63962F),  // -5
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/32.98F),  //  0
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/25.66856F),  //  5
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/20.10071F),  // 10
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/15.83515F),  // 15
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/12.54794F),  // 20
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/10.0F),  // 25
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/8.01384F),  // 30
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/6.45699F),  // 35
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/5.23002F),  // 40
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/4.25792F),  // 45
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/3.48376F),  // 50
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/2.86414F),  // 55
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/2.36577F),  // 60
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/1.96302F),  // 65
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/1.63603F),  // 70
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/1.36935F),  // 75
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/1.1509F),  // 80
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.9712F),  // 85
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.82276F),  // 90
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.69965F),  // 95
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.59715F),  // 100
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.51148F),  // 105
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.43962F),  // 110
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.37912F),  // 115
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.32801F),  // 120
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.28469F),  // 125
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.24784F),  // 130
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.2164F),  // 135
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.1895F),  // 140
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.16641F),  // 145
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.14652F)   // 150
};


/* ERTJ1VG103F */
const float ERTJ1VG103F[NTC_TABLE_SIZE]={   //[k目]
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/202.38F),  // -50
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/202.38F),  // -45
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/202.38F),  // -40
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/152.92F),  // -35
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/116.696F),  // -30
  	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/89.88F),  // -25
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/69.84F),  // -20
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/54.72F),  // -15
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/43.22F),  // -10
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/34.39F),  // -5
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/27.55F),  //  0
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/22.21F),  //  5
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/18.03F),  // 10
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/14.73F),  // 15
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/12.1F),  // 20
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/10.0F),  // 25
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/8.309F),  // 30
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/6.941F),  // 35
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/5.826F),  // 40
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/4.914F),  // 45
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/4.164F),  // 50
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/3.543F),  // 55
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/3.027F),  // 60
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/2.596F),  // 65
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/2.233F),  // 70
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/1.928F),  // 75
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/1.67F),  // 80
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/1.451F),  // 85
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/1.26F),  // 90
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/1.097F),  // 95
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.957F),  // 100
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.837F),  // 105
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.7338F),  // 110
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.6448F),  // 115
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.5678F),  // 120
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.5011F),  // 125
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.5011F),  // 130
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.5011F),  // 135
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.5011F),  // 140
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.5011F),  // 145
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.5011F)  // 150
};

/* LY1-395F103F-25RT (10K) */
//  Chiller Water Temp NTC module
const float LY1_395F103F_25RT[NTC_TABLE_SIZE]={   //[k目]
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/569.426F),  // -50
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/413.457F),  // -45
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/302.552F),  // -40
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/223.092F),  // -35
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/165.740F),  // -30
  	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/124.044F),  // -25
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/93.514F),  // -20
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/71.004F),  // -15
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/54.292F),  // -10
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/41.802F),  // -5
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/32.404F),  //  0
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/25.287F),  //  5
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/19.863F),  // 10
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/15.703F),  // 15
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/12.493F),  // 20
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/10.0F),  // 25
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/8.053F),  // 30
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/6.524F),  // 35
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/5.315F),  // 40
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/4.355F),  // 45
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/3.588F),  // 50
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/2.972F),  // 55
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/2.474F),  // 60
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/2.071F),  // 65
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/1.741F),  // 70
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/1.471F),  // 75
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/1.249F),  // 80
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/1.065F),  // 85
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.912F),  // 90
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.785F),  // 95
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.678F),  // 100
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.587F),  // 105
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.511F),  // 110
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.447F),  // 115
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.392F),  // 120
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.345F),  // 125
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.304F),  // 130
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.27F),  // 135
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.24F),  // 140
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.213F),  // 145
	(Vsens/Vref)*ADC_TOP/(1.0f+Rtop/0.191F)   // 150	
};


/* NTC location is TOP side.
 Vbot = Rbot / (Rbot + NTC) * Vsens
 ADCbot = ADC_TOP / Vref * Vbot
 ADCbot = (Vsens / Vref) * ADC_TOP / (1 + NTC / Rbot)
*/
// For FairChild IGBT module SPM45Series
const float SPM_45_SERIES[NTC_TABLE_SIZE]={   //[k目]
	(Vsens/Vref)*ADC_TOP/(1.0f+480.0f/Rbot),  // -50
	(Vsens/Vref)*ADC_TOP/(1.0f+480.0f/Rbot),  // -45
	(Vsens/Vref)*ADC_TOP/(1.0f+480.0f/Rbot),  // -40
	(Vsens/Vref)*ADC_TOP/(1.0f+480.0f/Rbot),  // -35
	(Vsens/Vref)*ADC_TOP/(1.0f+480.0f/Rbot),  // -30
  	(Vsens/Vref)*ADC_TOP/(1.0f+480.0f/Rbot),  // -25
	(Vsens/Vref)*ADC_TOP/(1.0f+480.0f/Rbot),   // -20
	(Vsens/Vref)*ADC_TOP/(1.0f+360.0f/Rbot),   // -15
	(Vsens/Vref)*ADC_TOP/(1.0f+270.0f/Rbot),   // -10
	(Vsens/Vref)*ADC_TOP/(1.0f+210.0f/Rbot),   // -5
	(Vsens/Vref)*ADC_TOP/(1.0f+158.2144f/Rbot),   //  0
	(Vsens/Vref)*ADC_TOP/(1.0f+122.2594f/Rbot),   //  5
	(Vsens/Vref)*ADC_TOP/(1.0f+95.2267f/Rbot),   // 10
	(Vsens/Vref)*ADC_TOP/(1.0f+74.7302f/Rbot),   // 15
	(Vsens/Vref)*ADC_TOP/(1.0f+59.0647f/Rbot),   // 20
	(Vsens/Vref)*ADC_TOP/(1.0f+47.0f/Rbot),   // 25
	(Vsens/Vref)*ADC_TOP/(1.0f+37.6431f/Rbot),   // 30
	(Vsens/Vref)*ADC_TOP/(1.0f+30.3339f/Rbot),   // 35
	(Vsens/Vref)*ADC_TOP/(1.0f+24.5907f/Rbot),   // 40
	(Vsens/Vref)*ADC_TOP/(1.0f+20.0478f/Rbot),   // 45
	(Vsens/Vref)*ADC_TOP/(1.0f+16.4325f/Rbot),   // 50
	(Vsens/Vref)*ADC_TOP/(1.0f+13.5385f/Rbot),   // 55
	(Vsens/Vref)*ADC_TOP/(1.0f+11.2091f/Rbot),   // 60
	(Vsens/Vref)*ADC_TOP/(1.0f+9.3279f/Rbot),   // 65
	(Vsens/Vref)*ADC_TOP/(1.0f+7.7979f/Rbot),  // 70
	(Vsens/Vref)*ADC_TOP/(1.0f+6.5443f/Rbot),   // 75
	(Vsens/Vref)*ADC_TOP/(1.0f+5.5178f/Rbot),   // 80
	(Vsens/Vref)*ADC_TOP/(1.0f+4.6736f/Rbot),   // 85
	(Vsens/Vref)*ADC_TOP/(1.0f+3.9717f/Rbot),   // 90
	(Vsens/Vref)*ADC_TOP/(1.0f+3.3878f/Rbot),   // 95
	(Vsens/Vref)*ADC_TOP/(1.0f+2.9019f/Rbot),   // 100
	(Vsens/Vref)*ADC_TOP/(1.0f+2.4943f/Rbot),  // 105
	(Vsens/Vref)*ADC_TOP/(1.0f+2.1496f/Rbot),   // 110
	(Vsens/Vref)*ADC_TOP/(1.0f+1.8598f/Rbot),   // 115
	(Vsens/Vref)*ADC_TOP/(1.0f+1.6153f/Rbot),   // 120
	(Vsens/Vref)*ADC_TOP/(1.0f+1.6153f/Rbot),   // 125
	(Vsens/Vref)*ADC_TOP/(1.0f+1.6153f/Rbot),   // 130
	(Vsens/Vref)*ADC_TOP/(1.0f+1.6153f/Rbot),   // 135
	(Vsens/Vref)*ADC_TOP/(1.0f+1.6153f/Rbot),   // 140
	(Vsens/Vref)*ADC_TOP/(1.0f+1.6153f/Rbot),   // 145
	(Vsens/Vref)*ADC_TOP/(1.0f+1.6153f/Rbot)   // 150
};

#endif
