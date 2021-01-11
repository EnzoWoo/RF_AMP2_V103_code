/*---------------------------------------------------------------------
by Kang Woo Jung: 2020.03.28

-----------------------------------------------------------------------*/
#include "usr.h"
#include "com.h"

com_t com1;
com_t com2;
com_t com3;


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
#ifdef USART1
  if(huart->Instance == USART1) {
		static uint8_t dat; // 필 전역변수!!!
    if(HAL_UART_Receive_IT(&huart1, &dat,1) == HAL_OK){
			kpro_callback(&com1.kpro, dat);
		}
  }
#endif	
#ifdef USART2
  if(huart->Instance == USART2) {
		static uint8_t dat; // 필 전역변수!!!
    if(HAL_UART_Receive_IT(&huart2,&dat,1) == HAL_OK){
			kpro_callback(&com2.kpro, dat);
		}
	}
#endif	
#ifdef USART3
  if(huart->Instance == USART3) {
		static uint8_t dat; // 필 전역변수!!!
    if(HAL_UART_Receive_IT(&huart3,&dat,1) == HAL_OK){
			kpro_callback(&com3.kpro, dat);
		}
	}
#endif	
}

void com_init(void)
{
	COM1_RS485_nRE_DE = 0;
	
	kpro_init(&com1.kpro, &huart1, KP_Little_Endian, KP_CRC16);
	kpro_init(&com2.kpro, &huart2, KP_Little_Endian, KP_CRC16);
	kpro_init(&com3.kpro, &huart3, KP_Little_Endian, KP_CRC16);
}

void com_process(void)
{
	static tm_t delay1, delay2, delay3; // for LED indicator
	
	//HAL_IWDG_Refresh(&hiwdg); // WatchDOG Clear!!!

	//USART1
	if(kpro_receive_time(&com1.kpro, 5, 500)){
		kpro_t *kpro;
		uint8_t tag_num;
		kpro = &com1.kpro;
		get_U8(tag_num);
		if(tag_num == 1){
			com_get_tag01(&com1);
			com_response01(&com1);
			SET_COM1_RS485_nRE_DE(); // RS485 Flow
			kpro_transmit(&com1.kpro);			
		}
		else if(tag_num == 2){
		}
		else {
		}
		delay1 = 0;  LED1 = 1; //LED indicator
	}
	if(com1.kpro.huart->TxXferCount == com1.kpro.huart->TxXferSize){
		
	}
	
	//USART2 *Debug Port*
	if(kpro_receive_time(&com2.kpro, 5, 500)){
		kpro_t *kpro;
		uint8_t tag_num;
		kpro = &com2.kpro;
		get_U8(tag_num);
		if(tag_num == 1){
			com_get_tag01(&com2);
			com_response01(&com2);
			kpro_transmit(&com2.kpro);			
		}
		else if(tag_num == 2){
		}
		else {
		}
		delay2 = 0;  LED2 = 1; //LED indicator
	}

	//USART3
	if(kpro_receive_time(&com3.kpro, 5, 500)){
		kpro_t *kpro;
		uint8_t tag_num;
		kpro = &com3.kpro;
		get_U8(tag_num);
		if(tag_num == 1){
			com_get_tag01(&com3);
			com_response01(&com3);
			kpro_transmit(&com3.kpro);			
		}
		else if(tag_num == 2){
		}
		else {
		}
		delay3 = 0;  LED3 = 1; //LED indicator
	}

	
	//LED indicator
	if(evt_delayms(&delay1, 50)){	delay1 = 50; LED1 = 0; }
	if(evt_delayms(&delay2, 50)){	delay2 = 50; LED2 = 0; }
	if(evt_delayms(&delay3, 50)){	delay3 = 50; LED3 = 0; }
	
	//RS485 Hardware Flow Control
	if(COM1_RS485_nRE_DE == 1 && com1.kpro.huart->TxXferCount == 0){
		if(com1.rs485_ctr_del) com1.rs485_ctr_del--;
		else COM1_RS485_nRE_DE = 0;
	}
}


#define sw01_UPDATE(var)   if(sw01_bot(com).var) status1_lev.var = sw01_lev(com).var;
#define pb01_UPDATE(var)   if(pb01_bot(com).var) status2_lev.var = pb01_lev(com).var;
#define pb01_TOGGLE(var)   if(pb01_mak(com).var) status2_lev.var ^= 1;
#define tag01_UPDATE(var){\
  if(com->tag01.var != com->tag01_prv.var){\
		usr.var  = com->tag01.var;\
		com->tag01_prv.var = com->tag01.var;\
	}\
}
#define tag01_RENEWAL(var){\
	usr.var  = com->tag01.var;\
	com->tag01_prv.var = com->tag01.var;\
}

void com_get_tag01(com_t *com)
{
	kpro_t *kpro;
	kpro = &com->kpro;
	
	get_U8(com->tag01.sw);
	get_U8(com->tag01.pb);
	get_U16(com->tag01.cp_pwm);
	get_U32(com->tag01.cp_volt);
	get_U32(com->tag01.pid_kp_v);
	get_U32(com->tag01.pid_ki_v);
	get_U32(com->tag01.pid_kd_v);
	get_U32(com->tag01.cp_curr);
	get_U32(com->tag01.pid_kp_c);
	get_U32(com->tag01.pid_ki_c);
	get_U32(com->tag01.pid_kd_c);
	get_U16(com->tag01.rf_freq1);
	get_U16(com->tag01.rf_freq2);
	get_U16(com->tag01.rf_freq3);
	get_U16(com->tag01.output);

	edge_process(&com->eg_sw01, com->tag01.sw);
	sw01_UPDATE(auto_mode);
	sw01_UPDATE(pid_en);
	sw01_UPDATE(run);
	sw01_UPDATE(v5_en);
	sw01_UPDATE(v12_en);
	sw01_UPDATE(cp_en);
	sw01_UPDATE(rf1_en);
	sw01_UPDATE(rf2_en);
	
	edge_process(&com->eg_pb01, com->tag01.pb);
	pb01_TOGGLE(set_pid_v);
	pb01_TOGGLE(set_pid_c);
	pb01_TOGGLE(sel_freq1);
	pb01_TOGGLE(sel_freq2);
	pb01_TOGGLE(sel_freq3);
	pb01_TOGGLE(b5);
	pb01_TOGGLE(b6);
	pb01_TOGGLE(b7);
	
	tag01_UPDATE(cp_pwm);
	
	tag01_UPDATE(cp_volt);
	if(status2_lev.set_pid_v){
		tag01_UPDATE(pid_kp_v);
		tag01_UPDATE(pid_ki_v);
		tag01_UPDATE(pid_kd_v);
	}
	if(status2_mak.set_pid_v){
		tag01_RENEWAL(pid_kp_v);
		tag01_RENEWAL(pid_ki_v);
		tag01_RENEWAL(pid_kd_v);
	}
	
	tag01_UPDATE(cp_curr);
	if(status2_lev.set_pid_c){
		tag01_UPDATE(pid_kp_c);
		tag01_UPDATE(pid_ki_c);
		tag01_UPDATE(pid_kd_c);
	}
	if(status2_mak.set_pid_c){
		tag01_RENEWAL(pid_kp_c);
		tag01_RENEWAL(pid_ki_c);
		tag01_RENEWAL(pid_kd_c);
	}
	
	tag01_UPDATE(rf_freq1);
	tag01_UPDATE(rf_freq2);
	tag01_UPDATE(rf_freq3);
	tag01_UPDATE(output);
}


#define sw02_UPDATE(var)   if(sw02_bot(com).var) status1_lev.var = sw02_lev(com).var;
#define pb02_TOGGLE(var)   if(pb02_bot(com).var) status2_lev.var ^= 1;
#define tag02_UPDATE(var){\
  if(com->tag02.var != com->tag02_prv.var){\
		usr.var  = com->tag02.var;\
		com->tag02_prv.var = com->tag02.var;\
	}\
}
#define tag02_RENEWAL(var){\
	usr.var  = com->tag02.var;\
	com->tag02_prv.var = com->tag02.var;\
}

void com_get_tag02(com_t *com)
{
	kpro_t *kpro;
	kpro = &com->kpro;
	
	get_U8(com->tag02.sw);
	get_U8(com->tag02.pb);
	get_U16(com->tag02.var1);
	get_U32(com->tag02.var2);

	edge_process(&com->eg_sw02, com->tag02.sw);
	sw02_UPDATE(auto_mode);
	sw02_UPDATE(pid_en);
	sw02_UPDATE(run);
	
	edge_process(&com->eg_pb02, com->tag02.pb);
	pb02_TOGGLE(sel_freq2);
	pb02_TOGGLE(sel_freq2);
	pb02_TOGGLE(sel_freq2);
	
}


void com_response01(com_t *com)
{
	kpro_t *kpro;
	kpro = &com->kpro;
	
	put_U8(0x01); // Response Number
	put_U16(usr.status1);
	put_U16(usr.status2);
	put_U16(usr.input);
	put_U16(usr.output);
	put_U16(looptick_us);

	put_U32(an.ave2[AN_VOLT]);
	put_U32(an.ave2[AN_CURR]);
	put_U32(an.ave2[AN_V24]);
	put_U32(an.ave2[AN_V12]);
	put_U32(an.ave2[AN_V5]);
	put_U32(an.ave2[AN_CPU_TEMP]);
	
	put_U16(usr.cp_pwm);
	
	put_U32(usr.cp_volt);
	put_U32(usr.pid_kp_v);
	put_U32(usr.pid_ki_v);
	put_U32(usr.pid_kd_v);

	put_U32(usr.cp_curr);
	put_U32(usr.pid_kp_c);
	put_U32(usr.pid_ki_c);
	put_U32(usr.pid_kd_c);
	
	put_U32(usr.pid_amp_v);
	put_U32(usr.pid_amp_c);
	put_U16(usr.cp_pwm_out);

	put_U32(usr.rf_freqFB_ave);
	put_U16(usr.rf_freq1);
	put_U16(usr.rf_freq2);
	put_U16(usr.rf_freq3);
	
	put_U16(usr.oct_dac1);
	put_U16(usr.oct_dac2);
	put_U16(usr.oct_dac3);

	put_U32(usr.reserve);
}


void com_response02(com_t *com)
{
	kpro_t *kpro;
	kpro = &com->kpro;

	put_U8(0x02); // Response Number
	put_U16(usr.status1);
	put_U16(usr.status2);
	put_U16(usr.input);
	put_U16(usr.output);
	put_U16(looptick_us);

	

}





/*
if(kpro1.rx_buf[0] < COMM_BUF_SIZE){
	// Branch
	get_packet[kpro1.rx_buf[0]](&kpro1, &tag1, COM_FORMAT);
	// Response
	put_packet[kpro1.rx_buf[0]](&kpro1, COM_FORMAT);
	kpro_transmit(&kpro1, COM_FORMAT); /////////////////////////
}
else {
}

void get_dummy(kpro_t * kpro, tag_t * tag) { }

//@ Receive Packe Function Table
void (* get_packet[COMM_BUF_SIZE])(kpro_t * kpro, tag_t * tag)={
	get_packet_auto,
	get_packet_engineer,
	get_dummy
};

void put_dummy(kpro_t * kpro) { }

//@ Transmit Packe Function Table
void (* put_packet[COMM_BUF_SIZE])(kpro_t * kpro)={
	put_packet_auto,
	put_packet_engineer,
	put_dummy
};

*/
