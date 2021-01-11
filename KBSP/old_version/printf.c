/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/
#include "bsp.h"
#include "fnd.h"

const u8 FND_CHA[]={
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 0x00~0x07
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 0x08~0x0f
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 0x10~0x17
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 0x18~0x1f
	0x00, 0x86, 0x22, 0x00, 0x00, 0x00, 0x00, 0x02,  // 0x20~0x27  " !"#$%&'"
	0x39, 0x0f, 0x40, 0x46, 0x80, 0x40, 0x80, 0x52,  // 0x28~0x2f  "'()*+,-./"
	0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27,  // 0x30~0x37  "01234567"
	0x7f, 0x6f, 0x30, 0xb0, 0xb9, 0x09, 0x8f, 0xd3,  //  0x38~0x3f  "89:;<=>?
	0x7b, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x3d,  // 0x40~0x47  "@ABCDEFG"
	0x76, 0x30, 0x1e, 0x75, 0x38, 0x55, 0x37, 0x3F,  // 0x48~0x4f  "HIJKLMNO"
	0x73, 0x67, 0x31, 0x6d, 0x78, 0x3e, 0x1c, 0x6a,  // 0x50~0x57  "PQRSTUVW "
	0xf6, 0x6e, 0x1b, 0x39, 0xea, 0x0f, 0x21, 0x08,  // 0x58~0x5f  "XYZ[\]^_"
	0x20, 0x4f, 0x7c, 0x58, 0x5e, 0x79, 0x71, 0x6f,  // 0x60~0x67  "`abcdefg"
	0x74, 0x10, 0x0e, 0x75, 0x18, 0x55, 0x54, 0x5c,  // 0x68~0x6f  "hijklmno"
	0x73, 0x67, 0x50, 0x6c, 0x78, 0x1c, 0x9c, 0x6a,  // 0x70~0x77  "pqrstuvw"
	0xf6, 0x6e, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00,  // 0x78~0x7f  "xyz{|}~ "
};

uint8_t fnd[3+1]; // dummy 1byte

void fnd_drive(void)
{
  static uint16_t step;
  static evt_t evtd;
  if(event(&evtd, 1000)){
	//SPI_CS = 0;
	//if(HAL_SPI_Transmit(&hspi2,fnd,4,1) == HAL_OK) SPI_CS = 1;
		
	SEG(0);
	DIGIT0 = ~(fnd[0]>>step);
	DIGIT1 = ~(fnd[1]>>step);
	DIGIT2 = ~(fnd[2]>>step);
	SEG(1<<step);
	step++;
	step &= 7;
  }
}


void dprintf(uint8_t *fnd, uint8_t digit_num, const uint8_t *fmt,...)
{
	uint8_t str[64];
	va_list ap;
	
	va_start(ap, fmt);
	vsprintf((char *) str,(const char *)fmt, ap);
	va_end(ap);

	int16_t i=0, d=0;
	while(str[i]){
		if(str[i] != '.'){
			fnd[d] = FND_CHA[str[i++]];
			if(str[i] == '.'){
				fnd[d] |= FND_CHA[str[i++]];
			}
		}
		else {
			fnd[d] = FND_CHA[str[i++]];
		}
		if(++d >= digit_num) break;
	}
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

