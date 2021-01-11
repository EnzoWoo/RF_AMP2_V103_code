/*---------------------------------------------------------------------
by Kang Woo Jung: 2020.03.28

-----------------------------------------------------------------------*/
#ifndef __USR_H__
#define __USR_H__

#include "an.h"
#include "bsp.h"
#include "com.h"


#define VERSION 	"RF_AMP2_V1.03"
#define UPDATE  	"2020-04-18"

//usr_watch_looptime(WATCH_TIME, WATCH_SENSITIVITY);
#define WATCH_TIME 		     500  // (us) watch  max looptime
#define WATCH_SENSITIVITY  100   // sensitivity = 50~200

/* ---------------------------------------------------------------GPIO */
#define TP10	 	PortB.b3	  // Loop Time
#define TP11	 	PortB.b9

#define V5_EN	  PortA.b7		//V5 Logic Power Enable, Active High
#define V12_EN 	PortA.b8		//V12 Logic Power Enable, Active High
#define CP_EN	  PortB.b4	  //Chopper Enable, Active High
#define RF_EN1  PortB.b8		//RF Gate Drive Enable, Active Low
#define RF_EN2  PortB.b5		//RF Gate Drive Enable, Active Low

/*
#define LED1	  PortC.b13
#define LED2	  PortC.b14
#define LED3	  PortC.b15
#define LED4	  PortB.b2
*/
#define LED4	  PortC.b13
#define LED3	  PortC.b14
#define LED2	  PortC.b15
#define LED1	  PortB.b2

#define NSS0    PortB.b12 // LTC6903
#define NSS1    PortD.b0	// HC165
#define NSS2    PortD.b1	// HC595

#define KEY_SW  PinA.b11


//TIM1 Chopper Control
#define TIM1_TOP  TIM1->ARR		//640-1, UP-DOWN mode, 50kHz
#define TIM1_PWM1 TIM1->CCR2  //50kHz
#define TIM1_PWM2 TIM1->CCR3  //50kHz

//Chopper PWM  
#define CP_PWM_TOP (TIM1_TOP>>1) //push-pull 에서 실질적인 TOP은 절 반 값임!!! 매우중요
#define CP_PWM(pwm) {\
	if(pwm > CP_PWM_TOP){\
		TIM1_PWM1 = CP_PWM_TOP;\
		TIM1_PWM2 = CP_PWM_TOP;\
	}\
	else {\
		TIM1_PWM1 = pwm;\
		TIM1_PWM2 = TIM1_TOP - pwm;\
	}\
}

//TIM2 RF Frequency Counter
#define FREQ_CNT  TIM2->CNT

//TIM3 using event.c

//TIM4 RF Frequency counter measure



/* ---------------------------------------------------------------PID parameter */
#define USR_PID_MIN_V 0.0f   // V
#define USR_PID_MAX_V 200.0f // V
#define USR_PID_MIN_C 0.0f	 // A
#define USR_PID_MAX_C 2.0f	 // A

#define USR_PID_KP_V  2.0f
#define USR_PID_KI_V  0.01f
#define USR_PID_KD_V  0.1f

#define USR_PID_KP_C  1.0f
#define USR_PID_KI_C  0.005f
#define USR_PID_KD_C  0.05f

#define USR_SET_PID_V()	pid_init(&pid_v, USR_PID_MIN_V, USR_PID_MAX_V, usr.pid_kp_v , usr.pid_ki_v , usr.pid_kd_v , (USR_PID_MAX_V / 2));
#define USR_RESET_PID_V() pid_reset(&pid_v)

#define USR_SET_PID_C()  pid_init(&pid_c, USR_PID_MIN_C, USR_PID_MAX_C, usr.pid_kp_c , usr.pid_ki_c , usr.pid_kd_c , (USR_PID_MAX_C / 2));
#define USR_RESET_PID_C() pid_reset(&pid_c)

/* ---------------------------------------------------------------Extern */
//extern clk_t 	clk;


/* ---------------------------------------------------------------State Machine */
typedef enum {
	USR_READY = 0,
	USR_AUTO_MODE,
	USR_MANUAL_MODE,
	USR_ERROR_MODE
}	USR_SMC_t;


/* ---------------------------------------------------------------User Variable */
typedef struct {
	//UART_HandleTypeDef *debug_uart;
	
	uint16_t status1;
	uint16_t status2;
	uint16_t input;
	uint16_t output;
	
	uint16_t cp_pwm;
	
	float cp_volt;
	float pid_kp_v;
	float pid_ki_v;
	float pid_kd_v;
	
	float cp_curr;
	float pid_kp_c;
	float pid_ki_c;
	float pid_kd_c;
	
	float pid_amp_v;
	float pid_amp_c;
	uint16_t cp_pwm_out;
	
	uint16_t rf_freq1;
	uint16_t rf_freq2;
	uint16_t rf_freq3;
	
	uint16_t oct_dac1;
	uint16_t oct_dac2;
	uint16_t oct_dac3;

	float reserve;

	uint16_t oct_dac;
	uint16_t rf_freq1_prev;
	uint16_t rf_freq2_prev;
	uint16_t rf_freq3_prev;

	uint16_t freq_cnt_high;
	float rf_freqFB_val;
	float rf_freqFB_ave;

	uint16_t smc;
	uint16_t smc_prev;
	tm_t smc_delay;

	tm_t powon_delay;
	tm_t looptime_over_val;
	
}usr_t;
extern usr_t  usr;


/* --------------------------------------------------------------- Status */
typedef struct {
	uint32_t run :1;	  // Switch
	uint32_t auto_mode :1; // Switch
	uint32_t pid_en :1; // Switch
	uint32_t v5_en :1;	// Switch	
	uint32_t v12_en :1;	// Switch
	uint32_t cp_en :1;	// Switch	
	uint32_t rf1_en :1;	// Switch
	uint32_t rf2_en :1;	// Switch
	
	uint32_t com1 :1;
	uint32_t com2 :1;
	uint32_t com3 :1;
	uint32_t b11 :1;
	uint32_t b12 :1;
	uint32_t b13 :1;
	uint32_t cv_mode :1;	// Chopper CV mode
	uint32_t cc_mode :1;	// Chopper CC mode

}status1_t;
extern edge_t 	eg_status1;
#define status1_lev   TypeCast(status1_t,usr.status1)
#define status1_mak	  TypeCast(status1_t,eg_status1.mak)
#define status1_bra 	TypeCast(status1_t,eg_status1.bra)
#define status1_bot 	TypeCast(status1_t,eg_status1.bot)
#define status1_hol 	TypeCast(status1_t,eg_status1.hol)

typedef struct {
	uint32_t set_pid_v :1;	// Push Button
	uint32_t set_pid_c :1;	// Push Button
	uint32_t sel_freq1 :1;	// Push Button	
	uint32_t sel_freq2 :1;	// Push Button	
	uint32_t sel_freq3 :1;	// Push Button	
	uint32_t b5 :1;	// Push Button	
	uint32_t b6 :1;	// Push Button	
	uint32_t b7 :1;	// Push Button
	
	uint32_t key_sw :1;	// Key Switch
	uint32_t b9 :1;
	uint32_t b10 :1;
	uint32_t b11 :1;
	uint32_t b12 :1;
	uint32_t b13 :1;
	uint32_t b14 :1;
	uint32_t looptime_over :1;

}status2_t;
extern edge_t 	eg_status2;
#define status2_lev   TypeCast(status2_t,usr.status2)
#define status2_mak	  TypeCast(status2_t,eg_status2.mak)
#define status2_bra 	TypeCast(status2_t,eg_status2.bra)
#define status2_bot 	TypeCast(status2_t,eg_status2.bot)
#define status2_hol 	TypeCast(status2_t,eg_status2.hol)

/* --------------------------------------------------------------- SPI Bit Input */
typedef struct {
	uint32_t in0 :1;	
	uint32_t in1 :1;	
	uint32_t in2 :1;	
	uint32_t in3 :1;	
	uint32_t in4 :1;	
	uint32_t in5 :1;	
	uint32_t in6 :1;	
	uint32_t in7 :1;	
	uint32_t in8 :1;	
	uint32_t in9 :1;	
	uint32_t in10 :1;	
	uint32_t in11 :1;	
	uint32_t in12 :1;	
	uint32_t in13 :1;	
	uint32_t in14 :1;	
	uint32_t in15 :1;	
} input_t;
/*
extern edge_t  eg_input;
#define input_lev  TypeCast(input_t,usr.input)
#define input_mak  TypeCast(input_t,eg_input.mak)
#define input_bra  TypeCast(input_t,eg_input.bra)
#define input_bot  TypeCast(input_t,eg_input.bot)
#define input_hol  TypeCast(input_t,eg_input.hol)
*/

// in_process() noise killed
extern in_t  in;
#define in_det  TypeCast(input_t,in.det)
#define in_lev  TypeCast(input_t,in.lev)
#define in_mak  TypeCast(input_t,in.mak)
#define in_bra  TypeCast(input_t,in.bra)
#define in_bot  TypeCast(input_t,in.bot)
#define in_hol  TypeCast(input_t,in.hol)



/* --------------------------------------------------------------- SPI Bit Output */
typedef struct {
	uint32_t out0 :1;	
	uint32_t out1 :1;	
	uint32_t out2 :1;	
	uint32_t out3 :1;	
	uint32_t out4 :1;	
	uint32_t out5 :1;	
	uint32_t out6 :1;	
	uint32_t out7 :1;	
	uint32_t out8 :1;	
	uint32_t out9 :1;	
	uint32_t out10 :1;	
	uint32_t out11 :1;	
	uint32_t out12 :1;	
	uint32_t out13 :1;	
	uint32_t out14 :1;	
	uint32_t out15 :1;	
} output_t;
extern edge_t  eg_output;
#define output_lev  TypeCast(output_t,usr.output)
#define output_mak  TypeCast(output_t,eg_output.mak)
#define output_bra  TypeCast(output_t,eg_output.bra)
#define output_bot  TypeCast(output_t,eg_output.bot)
#define output_hol  TypeCast(output_t,eg_output.hol)


/* --------------------------------------------------------------- Key */
typedef struct {
	uint32_t key_sw:1;
	uint32_t b1:1;
	uint32_t b2:1;
	uint32_t b3:1;
	uint32_t b4:1;
	uint32_t b5:1;
	uint32_t b6:1;
	uint32_t b7:1;

	
}key_sw_t;
extern key_t	  key;
#define key_det   TypeCast(key_sw_t, key.det)
#define key_lev   TypeCast(key_sw_t, key.lev)
#define key_mak 	TypeCast(key_sw_t, key.mak)
#define key_bra 	TypeCast(key_sw_t, key.bra)
#define key_bot 	TypeCast(key_sw_t, key.bot)
#define key_hol 	TypeCast(key_sw_t, key.hol)
#define key_one 	TypeCast(key_sw_t, key.one)
#define key_two 	TypeCast(key_sw_t, key.two)
#define key_thr 	TypeCast(key_sw_t, key.thr)
#define key_spe 	TypeCast(key_sw_t, key.spe)



/* ---------------------------------------------------------------User Function */

void usr_init(void);
void usr_process(void);

void usr_manual_mode(void);
void usr_auto_mode(void);
void usr_pwm_output(uint16_t pwm);
void usr_clear_all_control_bit(void);
void usr_variable_init(void);
void usr_pid_init_process(void);
void usr_watch_looptime(tm_t time, int16_t sensitivity); //time = max looptime, sensitivity = 50~200
void usr_com_connection_indication(void);
void usr_message(kpro_t *kpro);
void usr_rf_freq_measure(void);
void usr_rf_freq_set_n_select(void);
void usr_gpio_output(void);
void usr_spi_process(uint16_t *oct_dac, uint16_t *input, uint16_t *output);
void usr_auto_offset(UART_HandleTypeDef *huart);

void usr_unique_id(UART_HandleTypeDef *huart);
uint16_t usr_check_unique_crc(UART_HandleTypeDef *huart);
void usr_random(UART_HandleTypeDef *huart);
void usr_systick(UART_HandleTypeDef *huart);
const char * usr_reset_cause(uint32_t *reset_code);
void usr_rtc_clock_set(UART_HandleTypeDef *huart);


#endif




