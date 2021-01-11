/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/

#ifndef __KTYPE_H__
#define __KTYPE_H__ 

#include <stdio.h>
#include <stdlib.h>  // rand()
#include <stdarg.h> // printf()
#include <time.h>
#include <math.h>  // abs(), fabs()
//#include "arm_math.h"	 

#define ON  1
#define OFF 0
//#define SET 1
//#define RESET 0

#if defined(STM32F746xx)
	#include "stm32f7xx_hal.h"
	#define ARM_MATH_CM7
 	#define MSEC_VAL	1000		// unit 1us
	 
#elif defined(STM32F407xx)
	#include "stm32f4xx_hal.h"
	#define ARM_MATH_CM4
 	#define MSEC_VAL	1000		// unit 1us
	 
#elif defined(STM32F303xC)
	#include "stm32f3xx_hal.h"
	#define ARM_MATH_CM3
 	#define MSEC_VAL	1000		// unit 1us

#elif defined(STM32F103xB) | defined(STM32F103xBSTM32F103xx)
	#include "stm32f1xx_hal.h"
	#define ARM_MATH_CM3
 	#define MSEC_VAL	10	    // unit 100us
	 
#elif defined(STM32L431xx)
	#include "stm32l4xx_hal.h"
	#define ARM_MATH_CM4
 	#define MSEC_VAL	1000		// unit 100us
	
#elif defined(STM32L053xx) || defined(STM32L011xx)
	#include "stm32l0xx_hal.h"
	#define ARM_MATH_CM0
 	#define MSEC_VAL	1		// unit 1ms
	
#else
	#error  "Error! ../klib/stm32f.h"
	 
#endif
	 
typedef float  f32;
typedef double f64;

typedef uint8_t		u8;
typedef uint16_t	u16;
typedef uint32_t	u32;

typedef int16_t	s16;
typedef int16_t	s32;
typedef int8_t	s8;

typedef const uint8_t	 cu8;
typedef const uint16_t cu16;
typedef const uint32_t cu32;

typedef const int8_t 	cs8;
typedef const int16_t	cs16;
typedef const int32_t	cs32;

typedef __IO uint8_t	vu8;
typedef __IO uint16_t	vu16;
typedef __IO uint32_t	vu32;


#define BIT_0	0x00000001
#define BIT_1	0x00000002
#define BIT_2	0x00000004
#define BIT_3	0x00000008
#define BIT_4	0x00000010
#define BIT_5	0x00000020
#define BIT_6	0x00000040
#define BIT_7	0x00000080
#define BIT_8	0x00000100
#define BIT_9	0x00000200
#define BIT_10 0x00000400
#define BIT_11 0x00000800
#define BIT_12 0x00001000
#define BIT_13 0x00002000
#define BIT_14 0x00004000
#define BIT_15 0x00008000
#define BIT_16 0x00010000
#define BIT_17 0x00020000
#define BIT_18 0x00040000
#define BIT_19 0x00080000
#define BIT_20 0x00100000
#define BIT_21 0x00200000
#define BIT_22 0x00400000
#define BIT_23 0x00800000
#define BIT_24 0x01000000
#define BIT_25 0x02000000
#define BIT_26 0x04000000
#define BIT_27 0x08000000
#define BIT_28 0x10000000
#define BIT_29 0x20000000
#define BIT_30 0x40000000
#define BIT_31 0x80000000
#define BIT_ALL 0xFFFFFFFF


typedef struct {
	uint8_t seg :7;
	uint8_t dot :1;
}fnd_t;
#define TypeFND(value)			(*(fnd_t *)&value)

typedef struct
{
	uint8_t b0  :1;
	uint8_t b1  :1;
	uint8_t b2  :1;
	uint8_t b3  :1;
	uint8_t b4  :1;
	uint8_t b5  :1;
	uint8_t b6  :1;
	uint8_t b7  :1;
}bit8_t;
#define TypeBit8(value)				(*(bit8_t *)&value)

typedef struct
{
	uint16_t b0  :1;
	uint16_t b1  :1;
	uint16_t b2  :1;
	uint16_t b3  :1;
	uint16_t b4  :1;
	uint16_t b5  :1;
	uint16_t b6  :1;
	uint16_t b7  :1;
	uint16_t b8  :1;
	uint16_t b9  :1;
	uint16_t b10 :1;
	uint16_t b11 :1;
	uint16_t b12 :1;
	uint16_t b13 :1;
	uint16_t b14 :1;
	uint16_t b15 :1;
}bit16_t;
#define TypeBit16(value)			(*(bit16_t *)&value)

typedef struct
{
	uint32_t b0  :1;
	uint32_t b1  :1;
	uint32_t b2  :1;
	uint32_t b3  :1;
	uint32_t b4  :1;
	uint32_t b5  :1;
	uint32_t b6  :1;
	uint32_t b7  :1;
	uint32_t b8  :1;
	uint32_t b9  :1;
	uint32_t b10 :1;
	uint32_t b11 :1;
	uint32_t b12 :1;
	uint32_t b13 :1;
	uint32_t b14 :1;
	uint32_t b15 :1;
	uint32_t b16  :1;
	uint32_t b17  :1;
	uint32_t b18  :1;
	uint32_t b19  :1;
	uint32_t b20  :1;
	uint32_t b21  :1;
	uint32_t b22  :1;
	uint32_t b23  :1;
	uint32_t b24  :1;
	uint32_t b25  :1;
	uint32_t b26  :1;
	uint32_t b27  :1;
	uint32_t b28  :1;
	uint32_t b29  :1;
	uint32_t b30  :1;
	uint32_t b31  :1;
}bit32_t;
#define TypeBit32(value)		 (*(bit32_t *)&value)
#define TypeCast(type,value) (*(type *)&value)


typedef struct
{
	__IO uint16_t b0 	:1;
	__IO uint16_t b1 	:1;
	__IO uint16_t b2 	:1;
	__IO uint16_t b3 	:1;
	__IO uint16_t b4 	:1;
	__IO uint16_t b5 	:1;
	__IO uint16_t b6 	:1;
	__IO uint16_t b7 	:1;
	__IO uint16_t b8 	:1;
	__IO uint16_t b9 	:1;
	__IO uint16_t b10 :1;
	__IO uint16_t b11 :1;
	__IO uint16_t b12 :1;
	__IO uint16_t b13	:1;
	__IO uint16_t b14	:1;
	__IO uint16_t b15 :1;
} port_bit_t;

#define PinA (*(port_bit_t*)(&GPIOA->IDR))
#define PinB (*(port_bit_t*)(&GPIOB->IDR))
#define PinC (*(port_bit_t*)(&GPIOC->IDR))
#define PinD (*(port_bit_t*)(&GPIOD->IDR))
#define PinE (*(port_bit_t*)(&GPIOE->IDR))
#define PinF (*(port_bit_t*)(&GPIOF->IDR))
#define PinG (*(port_bit_t*)(&GPIOG->IDR))
#define PinH (*(port_bit_t*)(&GPIOH->IDR))
#define PinI (*(port_bit_t*)(&GPIOI->IDR))
#define PinJ (*(port_bit_t*)(&GPIOJ->IDR))
#define PinK (*(port_bit_t*)(&GPIOK->IDR))
#define PortA	(*(port_bit_t*)(&GPIOA->ODR))
#define PortB	(*(port_bit_t*)(&GPIOB->ODR))
#define PortC	(*(port_bit_t*)(&GPIOC->ODR))
#define PortD	(*(port_bit_t*)(&GPIOD->ODR))
#define PortE	(*(port_bit_t*)(&GPIOE->ODR))
#define PortF	(*(port_bit_t*)(&GPIOF->ODR))
#define PortG	(*(port_bit_t*)(&GPIOG->ODR))
#define PortH	(*(port_bit_t*)(&GPIOH->ODR))
#define PortI	(*(port_bit_t*)(&GPIOI->ODR))
#define PortJ	(*(port_bit_t*)(&GPIOJ->ODR))
#define PortK	(*(port_bit_t*)(&GPIOK->ODR))

typedef struct
{
	__IO uint8_t low;
	__IO uint8_t high;
} port_t;
#define PINA	(*(port_t*)(&GPIOA->IDR))
#define PINB	(*(port_t*)(&GPIOB->IDR))
#define PINC	(*(port_t*)(&GPIOC->IDR))
#define PIND	(*(port_t*)(&GPIOD->IDR))
#define PINE	(*(port_t*)(&GPIOE->IDR))
#define PINF	(*(port_t*)(&GPIOF->IDR))
#define PING	(*(port_t*)(&GPIOG->IDR))
#define PINH	(*(port_t*)(&GPIOH->IDR))
#define PINI	(*(port_t*)(&GPIOI->IDR))
#define PINJ	(*(port_t*)(&GPIOJ->IDR))
#define PINK	(*(port_t*)(&GPIOK->IDR))
#define PORTA	(*(port_t*)(&GPIOA->ODR))
#define PORTB	(*(port_t*)(&GPIOB->ODR))
#define PORTC	(*(port_t*)(&GPIOC->ODR))
#define PORTD	(*(port_t*)(&GPIOD->ODR))
#define PORTE	(*(port_t*)(&GPIOE->ODR))
#define PORTF	(*(port_t*)(&GPIOF->ODR))
#define PORTG	(*(port_t*)(&GPIOG->ODR))
#define PORTH	(*(port_t*)(&GPIOH->ODR))
#define PORTI	(*(port_t*)(&GPIOI->ODR))
#define PORTJ	(*(port_t*)(&GPIOJ->ODR))
#define PORTK	(*(port_t*)(&GPIOK->ODR))


#endif


