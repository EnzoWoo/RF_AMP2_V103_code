/*---------------------------------------------------------------------
by Kang Woo Jung: 2020.03.28

-----------------------------------------------------------------------*/
#ifndef __COM_H__
#define __COM_H__

#include "edge.h"
#include "kpro.h"


#define COM1_RS485_nRE_DE  PortA.b12   // RS485 nRE_DE

#define SET_COM1_RS485_nRE_DE() {\
	COM1_RS485_nRE_DE = 1;\
	com1.rs485_ctr_del = 2;\
}


/* --------------------------------------------------------------- Tag Type */
typedef struct {
	uint8_t sw; // switch
	uint8_t pb; // push button
	
	uint16_t cp_pwm;
	float cp_volt;
	float pid_kp_v;
	float pid_ki_v;
	float pid_kd_v;
	float cp_curr;
	float pid_kp_c;
	float pid_ki_c;
	float pid_kd_c;
	uint16_t rf_freq1;
	uint16_t rf_freq2;
	uint16_t rf_freq3;
	uint16_t output;
} Tag01_t;


typedef struct {
	uint16_t sw; // switch
	uint16_t pb; // push button

	uint16_t var1;
	uint16_t var2;

	
} Tag02_t;



/* --------------------------------------------------------------- COM */
typedef struct {
	kpro_t 	kpro;
	
	Tag01_t tag01;
	Tag01_t tag01_prv;
	Tag02_t tag02;
	Tag02_t tag02_prv;
	// ......
	// TagN_t tagN;
	// TagN_t tagN_prv;
	
	edge_t   eg_sw01;
	edge_t   eg_pb01;
	edge_t   eg_sw02;
	edge_t   eg_pb02;

	uint16_t rs485_ctr_del;
} com_t;
extern com_t  com1;
extern com_t  com2;
extern com_t  com3;


/* --------------------------------------------------------------- Switch */
typedef struct {
	uint16_t run :1;
	uint16_t auto_mode :1;
	uint16_t pid_en :1;
	uint16_t v5_en :1;	
	uint16_t v12_en :1;	
	uint16_t cp_en :1;	
	uint16_t rf1_en :1;
	uint16_t rf2_en :1;
	
} SwitchType01_t;
#define sw01_lev(com)  TypeCast(SwitchType01_t, com->tag01.sw)
#define sw01_mak(com)  TypeCast(SwitchType01_t, com->eg_sw01.mak)
#define sw01_bra(com)  TypeCast(SwitchType01_t, com->eg_sw01.bra)
#define sw01_bot(com)  TypeCast(SwitchType01_t, com->eg_sw01.bot)


typedef struct {
	uint16_t run :1;
	uint16_t auto_mode :1;
	uint16_t pid_en :1;
	uint16_t b3 :1;	
	uint16_t b4 :1;	
	uint16_t b5 :1;	
	uint16_t b6 :1;	
	uint16_t b7 :1;
	uint16_t b8 :1;
	
} SwitchType02_t;
#define sw02_lev(com)  TypeCast(SwitchType02_t, com->tag02.sw)
#define sw02_mak(com)  TypeCast(SwitchType02_t, com->eg_sw02.mak)
#define sw02_bra(com)  TypeCast(SwitchType02_t, com->eg_sw02.bra)
#define sw02_bot(com)  TypeCast(SwitchType02_t, com->eg_sw02.bot)


/* --------------------------------------------------------------- Push Button */

typedef struct {
	uint16_t set_pid_v :1;	
	uint16_t set_pid_c :1;	
	uint16_t sel_freq1 :1;	
	uint16_t sel_freq2 :1;	
	uint16_t sel_freq3 :1;	
	uint16_t b5 :1;	
	uint16_t b6 :1;	
	uint16_t b7 :1;
} PushButtonType01_t;
#define pb01_lev(com)  TypeCast(PushButtonType01_t, com->tag01.pb)
#define pb01_mak(com)  TypeCast(PushButtonType01_t, com->eg_pb01.mak)
#define pb01_bra(com)  TypeCast(PushButtonType01_t, com->eg_pb01.bra)
#define pb01_bot(com)  TypeCast(PushButtonType01_t, com->eg_pb01.bot)


typedef struct {
	uint16_t sel_freq1 :1;	
	uint16_t sel_freq2 :1;	
	uint16_t sel_freq3 :1;	
	uint16_t b3 :1;	
	uint16_t b4 :1;	
	uint16_t b5 :1;	
	uint16_t b6 :1;	
	uint16_t b7 :1;

} PushButtonType02_t;
#define pb02_lev(com)  TypeCast(PushButtonType02_t, com->tag02.pb)
#define pb02_mak(com)  TypeCast(PushButtonType02_t, com->eg_pb02.mak)
#define pb02_bra(com)  TypeCast(PushButtonType02_t, com->eg_pb02.bra)
#define pb02_bot(com)  TypeCast(PushButtonType02_t, com->eg_pb02.bot)



/* --------------------------------------------------------------- Functions */
void com_init(void);
void com_process(void);

void com_get_tag01(com_t *com);
void com_get_tag02(com_t *com);

void com_response01(com_t *com);
void com_response02(com_t *com);


#endif


