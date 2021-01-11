/*---------------------------------------------------------------------
by Kang Woo Jung: 2019.12.26

-----------------------------------------------------------------------*/
#ifndef __LTC6903_H__
#define __LTC6903_H__

#include "kType.h"

/*
f ≥      f <       OCT
34.05MHz 68.03MHz  15
17.02MHz 34.01MHz  14
8.511MHz 17.01MHz  13
4.256MHz 8.503MHz  12
2.128MHz 4.252MHz  11
1.064MHz 2.126MHz  10
532kHz   1063kHz   9
266kHz   531.4kHz  8
133kHz   265.7kHz  7
66.5kHz  132.9kHz  6
33.25kHz 66.43kHz  5
16.62kHz 33.22kHz  4
8.312kHz 16.61kHz  3
4.156kHz 8.304kHz  2
2.078kHz 4.152kHz  1
1.039kHz 2.076kHz  0
*/

//Output Configuration 
#define LTC6903_CLK0  0x00    // Clock on, inverted clock on 
#define LTC6903_CLK1  0x01    // Clock off, inverted clock on 
#define LTC6903_CLK2  0x02    // Clock on, inverted clock off 
#define LTC6903_CLK3  0x03    // Powers down 

uint16_t LTC6903_Get_Freq(uint16_t code);  // return kHz
uint16_t LTC6903_Get_Code(uint16_t freq_khz);  // kHz

#endif 

