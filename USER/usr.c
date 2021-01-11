/*---------------------------------------------------------------------
by Kang Woo Jung: 2020.03.28

-----------------------------------------------------------------------*/
#include "usr.h"

//---------------------------------------------------------------
usr_t 	usr;
edge_t 	eg_status1;
edge_t 	eg_status2;
//edge_t  eg_input;
edge_t  eg_output;
in_t    in;
key_t 	key;

pid_t pid_v;
pid_t pid_c;


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	usr.freq_cnt_high++;
/*	
	if(htim->Instance == TIM2){
		usr.freq_cnt_high++;
	}
*/	
}


//---------------------------------------------------------------
void usr_init(void)
{
	
  bsp_init(); //////////////////////////////// kwj
  com_init();

	V5_EN = 1; // Logic power hold
	NSS0 = 1;
	NSS1 = 1;
	NSS2 = 1;
	
	//Debug Port Selection
	UART_HandleTypeDef *debug_uart;
	debug_uart = &huart2;
	
	uprintf(debug_uart,(cu8*)"----------------------------------------------------\r\n");/////////////
	uprintf(debug_uart,(cu8*)"* VERSION: %s\r\n", VERSION);
	uprintf(debug_uart,(cu8*)"* UPDATE: %s\r\n", UPDATE);
	//usr_rtc_clock_set(debug_uart);
	
	usr_unique_id(debug_uart);
	//usr_random(debug_uart);
	usr_check_unique_crc(debug_uart);

	//RESET CAUSE
	uint32_t reset_code;
	const char *s = usr_reset_cause(&reset_code);
	uprintf(debug_uart,(cu8*)"* %s\r\n", s);
	//
	if(reset_code == RCC_FLAG_PINRST){
	}
	else if(reset_code == RCC_FLAG_IWDGRST){
	}
	//
	usr_auto_offset(debug_uart);
	usr_systick(debug_uart);
	
	uprintf(debug_uart,(cu8*)"----------------------------------------------------\r\n");/////////////
	
	usr_variable_init();
	usr.smc = USR_READY;
	usr.smc_delay = 0;
	usr.powon_delay = 0;
	
	//status1_lev.autom = 1;
}


/* ---------------------------------------------------------------usr_process() : User main function */
void usr_process(void)
{
	HAL_IWDG_Refresh(&hiwdg); //WatchDOG Clear!
	
	/* Main LOOP Scan Time Indicator */
	evt_process(); // in event.c
	TP10 ^= 1; ////////////
	//GPIOB->ODR ^= BIT_3; 
	
	//Analod ADC, COM Protocol
	an_process();
	com_process();
	
	//Edge process
	edge_process(&eg_status1, usr.status1);
	edge_process(&eg_status2, usr.status2);
	//edge_process(&eg_input, usr.input);
	edge_process(&eg_output, usr.output);
	
	//Input process
	//in.det <--- usr_spi_process();
	in_process(&in, 16); // noise kill
	usr.input = in.lev;
	
	//GPIO Input
	//static indel_t indel;
	//status_lev.cp_oc = indelay(&indel, 5, CP_OC);
	//status_lev.cp_oc = CP_OC;

	//Key process
	key_det.key_sw = ~KEY_SW;
	key_process(&key, 1);
	status2_lev.key_sw = key_lev.key_sw;

	//SPI Communication for LTC6903(Progrmable OSC), HC165(input), HC595(output)
	//usr_spi_process(&usr.oct_dac, &usr.input, &usr.output);
	usr_spi_process(&usr.oct_dac, ((uint16_t*)&in.det), &usr.output);
	
	// Power ON Work's
	if(evt_delayms(&usr.powon_delay, 2000)){
		usr.powon_delay = 2000;
	}
	if(usr.powon_delay == 500){
		status1_lev.v12_en = 1;
	}

	//Copy COM Connection Indication
	usr_com_connection_indication();
	
	// Debug Message
	//usr_message(&com1.kpro);
	usr_message(&com2.kpro);
	//usr_message(&com3.kpro);

	//GPIO Output
	usr_gpio_output();
	CP_PWM(usr.cp_pwm_out);

	
	//Watch Loop Time !!! 감도 없으면 mcu 폭주....ㅠ,.ㅠ
	//usr_watch_looptime(WATCH_TIME, WATCH_SENSITIVITY); //time = max looptime, sensitivity = 50~200

	//RF Frequency Measure, Set, Select
	usr_rf_freq_measure();
	usr_rf_freq_set_n_select();


	/* State Machine */
	switch(usr.smc){
	case USR_READY:
		usr_clear_all_control_bit();
		if(evt_delayms(&usr.smc_delay,100)){
			usr.smc_delay = 0;
			usr.smc++;
		}
		break;
		
	case USR_MANUAL_MODE:
		usr_manual_mode();
		if(status1_lev.auto_mode){
			status1_lev.pid_en = 0;
			usr_clear_all_control_bit();
		}
		else usr.smc_delay = 0;
		if(evt_delayms(&usr.smc_delay,100)){
			usr.smc_delay = 0;
			usr.smc = USR_AUTO_MODE;
		}
		break;

	case USR_AUTO_MODE:
		usr_auto_mode();
		if(!status1_lev.auto_mode){
			status1_lev.pid_en = 0;
			usr_clear_all_control_bit();
		}	
		else usr.smc_delay = 0;
		if(evt_delayms(&usr.smc_delay,100)){
			usr.smc_delay = 0;
			usr.smc = USR_MANUAL_MODE;
		}
		break;
		
	case USR_ERROR_MODE:
		break;
	
	default:
		usr.smc = USR_READY;
		break;
	}		

	//static pwm_t pwm1;
	//pwm_process(&pwm1,1000000,250000);
	//status_lev.clock = pwm1.out; /////////////////////////

  static evt_t evt1;
	if(evt_ms(&evt1, 500)){
		LED4 = status1_lev.run ? 1 : LED4^1;

		//static uint16_t cnt = 0;
		//if(++cnt == 10) cnt=0;
		//if(cnt == 0){
		//}

		// RS485 TX test
		//com_response01(&com1);
		//SET_COM1_RS485_nRE_DE();
		//kpro_transmit(&com1.kpro);			

		//clk_get(&clk);
		//kprintf(&kpro1,(cu8*)" %4d-%2d-%2d(%s), ", 2000+clk.year, clk.month, clk.date, clkDAY[clk.day]);
		//kprintf(&kpro1,(cu8*)" %2d:%0.2d:%0.2d\r\n", clk.hour, clk.min, clk.sec);
	}

/* // pwm Test	
	static uint16_t m, cnt, pwm;
	if(++cnt > 20){
		cnt=0;
		if(m){
			if(pwm < TIM1->ARR>>1)	pwm++;
			else m = 0;
		}
		else {
			if(pwm) pwm--;
			else m = 1;
		}
		TIM1->CCR2 = pwm;
		TIM1->CCR3 = TIM1->ARR - pwm;
	}
*/	
 // SPI output Test	
	static uint16_t m, cnt, cnt2;
	if(++cnt > 1000){
		cnt=0;
		if(m){
			usr.output >>= 1;
			if(++cnt2 == 12){	cnt2 = 0;	m = 0;}
		}
		else {
			usr.output <<= 1;
			if(++cnt2 == 12){	cnt2 = 0;	m = 1;}
		}
	}
	
}


void usr_manual_mode(void)
{
	if(status1_lev.auto_mode) return;
	
	//PID Setting
	if(status2_lev.set_pid_v)	USR_SET_PID_V();
	if(status2_lev.set_pid_c)	USR_SET_PID_C();

	if(status1_bot.pid_en) usr_clear_all_control_bit(); // 동작함!
	
	//CHOPPER Control
	if(status1_lev.pid_en){
		//Chopper PID Control
		float err_v, err_c;
		float amp_v, amp_c;
		static tm_t cnt;
		
		cnt += looptick_us;
		if(cnt > 1000){ // 1ms
			//PID CP Voltage Loop
			if(status1_lev.run){
				err_v = usr.cp_volt - an.ave1[AN_VOLT];
			}
			else {
				err_v = 0;
				USR_RESET_PID_V();
			}
			amp_v = pid(&pid_v, err_v);
			usr.pid_amp_v = (float)CP_PWM_TOP * amp_v / USR_PID_MAX_V;
			
			//PID CP Current Loop
			if(status1_lev.run){
				err_c = usr.cp_curr - an.ave1[AN_CURR];
			}
			else {
				err_c = 0;
				USR_RESET_PID_C();
			}
			amp_c = pid(&pid_c, err_c);
			usr.pid_amp_c = (float)CP_PWM_TOP * amp_c / USR_PID_MAX_C;
			
			//Choice CP Control Loop
			if(usr.pid_amp_v < usr.pid_amp_c){
				usr.cp_pwm_out = (uint16_t) usr.pid_amp_v;
				status1_lev.cc_mode = 0;
				status1_lev.cv_mode = 1;
			}
			else {
				usr.cp_pwm_out = (uint16_t) usr.pid_amp_c;
				status1_lev.cv_mode = 0;
				status1_lev.cc_mode = 1;
			}
		}
	}
	else {
		//Chopper PWM Manual Control
		usr_pwm_output(usr.cp_pwm);
	}
}


void usr_auto_mode(void)
{
	if(!status1_lev.auto_mode) return;

	static uint16_t smc;
	switch(smc){
	case 0:
		break;

	case 1:
		break;

	default:
		smc = 0;
		break;
	}

}

void usr_pwm_output(uint16_t pwm)
{
	static tm_t cnt;
	cnt += looptick_us;
	
	if(status1_lev.run){
		if(cnt > 1000){ // 1ms
			cnt = 0;
			if(pwm > usr.cp_pwm_out) usr.cp_pwm_out++;
			if(pwm < usr.cp_pwm_out) usr.cp_pwm_out--;
		}	
	}
	else {
		if(cnt > 200){ // 200us
			cnt = 0;
			if(usr.cp_pwm_out) usr.cp_pwm_out--;
		}			
	}
}


void usr_clear_all_control_bit(void)
{
	status1_lev.run = 0;
	status1_lev.cp_en = 0;
	status1_lev.rf1_en = 0;
	status1_lev.rf2_en = 0;
}


void usr_variable_init(void)
{
	usr.status1 = 0;
	usr.status2 = 0;
	status1_lev.v5_en = 1;  //Logic Power Hold
	status2_lev.sel_freq1 = 1; //Init freq1 sel

	
	usr.cp_pwm = 0;
	usr.cp_volt = 0;
	usr.pid_kp_v = USR_PID_KP_V;
	usr.pid_ki_v = USR_PID_KI_V;
	usr.pid_kd_v = USR_PID_KD_V;
	USR_SET_PID_V();
	
	usr.cp_curr = 0;
	usr.pid_kp_c = USR_PID_KP_C;
	usr.pid_ki_c = USR_PID_KI_C;
	usr.pid_kd_c = USR_PID_KD_C;
	USR_SET_PID_C();
	
	usr.pid_amp_v = 0;
	usr.pid_amp_c = 0;
	usr.cp_pwm_out = 0;

	usr.rf_freq1 = 1000;
	usr.rf_freq2 = 3000;
	usr.rf_freq3 = 10000;
	usr.oct_dac = LTC6903_Get_Code(usr.rf_freq1); // output freq1
	usr.oct_dac1 = LTC6903_Get_Code(usr.rf_freq1);
	usr.oct_dac2 = LTC6903_Get_Code(usr.rf_freq2);
	usr.oct_dac3 = LTC6903_Get_Code(usr.rf_freq3);	
	
	usr.rf_freq1_prev = usr.rf_freq1;
	usr.rf_freq2_prev = usr.rf_freq2;
	usr.rf_freq3_prev = usr.rf_freq3;
	
	usr.rf_freqFB_val = 0;	
	usr.rf_freqFB_ave = 0;	
	
}

//time = max looptime, sensitivity = 50~200
void usr_watch_looptime(tm_t time, int16_t sensitivity)
{
	static int16_t cnt = 0;

	if(looptick_us > time){
		if(++cnt > sensitivity){
			usr.looptime_over_val = looptick_us;
			status2_lev.looptime_over = 1;
		}
	}
	if(looptick_us < time){
		if(--cnt < -sensitivity){
			status2_lev.looptime_over = 0;
		}
	}
}


void usr_com_connection_indication(void)
{
	//Copy COM Connection Indication
	status1_lev.com1 = com1.kpro.connect;
	status1_lev.com2 = com2.kpro.connect;
	status1_lev.com3 = com3.kpro.connect;
}

void usr_message(kpro_t *kpro)
{
	if(!kpro->connect) return;
	
	if(status1_mak.run) kprintf(kpro, (cu8*)"M:\\> RUN!\n\r");
	if(status1_bra.run) kprintf(kpro, (cu8*)"M:\\> STOP!\n\r");

	if(status1_mak.auto_mode) kprintf(kpro, (cu8*)"M:\\> AUTO mode\n\r");
	if(status1_bra.auto_mode) kprintf(kpro, (cu8*)"M:\\> MANUAL mode\n\r");
	if(status1_lev.auto_mode & status1_bot.pid_en) kprintf(kpro, (cu8*)"M:\\> Fail! This is manual mode commmand!\n\r");
	else {
		if(status1_mak.pid_en) kprintf(kpro, (cu8*)"M:\\> PID Enable\n\r");
		if(status1_bra.pid_en) kprintf(kpro, (cu8*)"M:\\> PID Disable\n\r");
	}

	if(status1_mak.v5_en) kprintf(kpro, (cu8*)"M:\\> V5 ON\n\r");
	if(status1_bra.v5_en) kprintf(kpro, (cu8*)"M:\\> V5 OFF\n\r");
	if(status1_mak.v12_en) kprintf(kpro, (cu8*)"M:\\> V12 ON\n\r");
	if(status1_bra.v12_en) kprintf(kpro, (cu8*)"M:\\> V12 OFF\n\r");
	if(status1_mak.cp_en) kprintf(kpro, (cu8*)"M:\\> CP Enable\n\r");
	if(status1_bra.cp_en) kprintf(kpro, (cu8*)"M:\\> CP Disable\n\r");
	if(status1_mak.rf1_en) kprintf(kpro, (cu8*)"M:\\> RF1 Enable\n\r");
	if(status1_bra.rf1_en) kprintf(kpro, (cu8*)"M:\\> RF1 Disable\n\r");
	if(status1_mak.rf2_en) kprintf(kpro, (cu8*)"M:\\> RF2 Enable\n\r");
	if(status1_bra.rf2_en) kprintf(kpro, (cu8*)"M:\\> RF2 Disable\n\r");
	
	if(status1_mak.com1) kprintf(kpro, (cu8*)"M:\\> COM1 Connected\n\r");
	if(status1_bra.com1) kprintf(kpro, (cu8*)"M:\\> COM1 Disconnected\n\r");
	if(status1_mak.com2) kprintf(kpro, (cu8*)"M:\\> COM2 Connected\n\r");
	if(status1_bra.com2) kprintf(kpro, (cu8*)"M:\\> COM2 Disconnected\n\r");
	if(status1_mak.com3) kprintf(kpro, (cu8*)"M:\\> COM3 Connected\n\r");
	if(status1_bra.com3) kprintf(kpro, (cu8*)"M:\\> COM3 Disconnected\n\r");
	
	if(status1_mak.cv_mode) kprintf(kpro, (cu8*)"M:\\> CV mode\n\r");
	if(status1_mak.cc_mode) kprintf(kpro, (cu8*)"M:\\> CC mode\n\r");

	if(status1_lev.auto_mode & (status2_bot.set_pid_v | status2_bot.set_pid_c))
		kprintf(kpro, (cu8*)"M:\\> Fail! This is manual mode commmand!\n\r");
	else {
		if(status2_mak.set_pid_v) kprintf(kpro, (cu8*)"M:\\> Set PID V Enable\n\r");
		if(status2_bra.set_pid_v) kprintf(kpro, (cu8*)"M:\\> Set PID V Disable\n\r");
		if(status2_mak.set_pid_c) kprintf(kpro, (cu8*)"M:\\> Set PID C Enable\n\r");
		if(status2_bra.set_pid_c) kprintf(kpro, (cu8*)"M:\\> Set PID C Disable\n\r");
	}
	
	if(status2_mak.sel_freq1) kprintf(kpro, (cu8*)"M:\\> RF Freq1 Selected\n\r");
	if(status2_mak.sel_freq2) kprintf(kpro, (cu8*)"M:\\> RF Freq2 Selected\n\r");
	if(status2_mak.sel_freq3) kprintf(kpro, (cu8*)"M:\\> RF Freq3 Selected\n\r");

	if(status2_mak.key_sw) kprintf(kpro, (cu8*)"M:\\> Key Switch ON\n\r");
	if(status2_mak.looptime_over) kprintf(kpro, (cu8*)"M:\\> LoopTime OVER = %d (usec)\n\r", usr.looptime_over_val);
}


void usr_rf_freq_measure(void)
{
	uint32_t cap;
	static evt_t evt;
	if(evt_us(&evt, 50050)){
		cap = usr.freq_cnt_high<<16;
		cap |= FREQ_CNT;
		FREQ_CNT = 0;
		usr.freq_cnt_high = 0;
		
		usr.rf_freqFB_val = (float) cap;
		usr.rf_freqFB_val /= 50;
		usr.rf_freqFB_ave = Filter2(0.1f, (float)usr.rf_freqFB_val, usr.rf_freqFB_ave);
	}
}

void usr_rf_freq_set_n_select(void)
{
	//Selection RF Frequency
	if(status2_mak.sel_freq1){
		status2_lev.sel_freq2 = 0;
		status2_lev.sel_freq3 = 0;
		usr.oct_dac = usr.oct_dac1;
	}
	if(status2_mak.sel_freq2){
		status2_lev.sel_freq1 = 0;
		status2_lev.sel_freq3 = 0;
		usr.oct_dac = usr.oct_dac2;
	}
	if(status2_mak.sel_freq3){
		status2_lev.sel_freq1 = 0;
		status2_lev.sel_freq2 = 0;
		usr.oct_dac = usr.oct_dac3;
	}
	
	//Set RF Frequency
	if(usr.rf_freq1 != usr.rf_freq1_prev){
		usr.rf_freq1_prev = usr.rf_freq1;
		usr.oct_dac1 = LTC6903_Get_Code(usr.rf_freq1);
		usr.oct_dac = usr.oct_dac1;
	}
	if(usr.rf_freq2 != usr.rf_freq2_prev){
		usr.rf_freq2_prev = usr.rf_freq2;
		usr.oct_dac2 = LTC6903_Get_Code(usr.rf_freq2);
		usr.oct_dac = usr.oct_dac2;
	}
	if(usr.rf_freq3 != usr.rf_freq3_prev){
		usr.rf_freq3_prev = usr.rf_freq3;
		usr.oct_dac3 = LTC6903_Get_Code(usr.rf_freq3);
		usr.oct_dac = usr.oct_dac3;
	}
}


void usr_gpio_output(void)
{
	//GPIO Port Output
	V5_EN = status1_lev.v5_en;
	V12_EN = status1_lev.v12_en;
	CP_EN = status1_lev.cp_en;
	RF_EN1 = status1_lev.rf1_en;
	RF_EN2 = status1_lev.rf2_en;
}

void usr_spi_process(uint16_t *oct_dac, uint16_t *input, uint16_t *output)
{
	/* SPI process
	NSS0 = LTC6903
	NSS1 = HC165
	NSS2 = HC595
	*/
	static uint16_t mc = 0;
	static uint8_t spi_rx[2], spi_tx[2];
	
	switch(mc){
	case 0:
		NSS1 = 0;
		NSS0 = 1;
		((uint8_t*)&input)[1] = spi_rx[0];
		((uint8_t*)&input)[0] = spi_rx[1];
	
		NSS2 = 0;
		spi_tx[0] = ((uint8_t*)output)[1];
		spi_tx[1] = ((uint8_t*)output)[0];

		NSS1 = 1;
		mc++;
		break;
	case 1: // HC165, HC595
		if(HAL_SPI_TransmitReceive_IT(&hspi2, spi_tx, spi_rx, 2) == HAL_OK){
			mc++;
		}
		break;
	case 2:
		NSS1 = 0;
		NSS2 = 1;
		((uint8_t*)input)[1] = spi_rx[0];
		((uint8_t*)input)[0] = spi_rx[1];
	
		NSS0 = 0;
		spi_tx[0] = ((uint8_t*)oct_dac)[1];
		spi_tx[1] = ((uint8_t*)oct_dac)[0];
	
		NSS1 = 1;
		mc++;
		break;
	case 3: // HC165, LTC6903
		if(HAL_SPI_TransmitReceive_IT(&hspi2, spi_tx, spi_rx, 2) == HAL_OK){
			mc = 0;
		}
		break;
	default : 
		mc = 0;
		break;
	}
}


void usr_auto_offset(UART_HandleTypeDef *huart)
{
  uprintf(&huart1,(cu8*)"* AUTO OFFSET\r\n");
	evt_t delay = 0;
	while(1){
		HAL_IWDG_Refresh(&hiwdg);
		evt_process();
		an_process();
		if(evt_delayms(&delay, 500)) break;
	}
	an.ofs[AN_CURR] = an.ave2[AN_CURR];
  uprintf(huart,(cu8*)"* CURR Offset = %0.3f A\r\n", an.ofs[AN_CURR]); //Sensitivity = 185 mV/A
}

void usr_unique_id(UART_HandleTypeDef *huart)
{
	uprintf(huart, (cu8*)"* IDCODE = 0x%0.8X\r\n", DBGMCU->IDCODE);
	uprintf(huart, (cu8*)"* DEVID = 0x%0.8X\r\n", HAL_GetDEVID());
	uprintf(huart, (cu8*)"* REVID = 0x%0.8X\r\n", HAL_GetREVID());

	// Unique ID
	for(uint16_t i=0 ; i<3 ; i++){
		uprintf(huart,(cu8*)"* UID_%d = 0x%0.8X\r\n",i,((uint32_t*)UID_BASE)[i]);
	}
/*	
	uprintf(huart, (cu8*)"* UID_0 = 0x%0.8X\r\n", HAL_GetUIDw0());
	uprintf(huart, (cu8*)"* UID_1 = 0x%0.8X\r\n", HAL_GetUIDw1());
	uprintf(huart, (cu8*)"* UID-2 = 0x%0.8X\r\n", HAL_GetUIDw2());
*/	
}

uint16_t usr_check_unique_crc(UART_HandleTypeDef *huart)
{
	uint32_t key1 = 0x00000000;
	uint32_t key2 = 0x00000000;
	
	uint32_t crc1 = HAL_CRC_Calculate(&hcrc, (uint32_t*)UID_BASE, 12);
	uint32_t crc2 = crc32_buf(0xffffffff, (uint8_t*)UID_BASE, 12);

	uprintf(huart, (cu8*)"* CRC1 = 0x%0.8X\n\r", crc1);
	uprintf(huart, (cu8*)"* CRC2 = 0x%0.8X\n\r", crc2);
	uprintf(huart, (cu8*)"* KEY1 = 0x%0.8X\n\r", key1);
	uprintf(huart, (cu8*)"* KEY2 = 0x%0.8X\n\r", key2);
	
	if(crc1 == key1){
		if(crc2 == key2){
			uprintf(huart, (cu8*)"* SYSTEM KEY PASS!\r\n");
			return 1;
		}
	}
	uprintf(huart, (cu8*)"* SYSTEM KEY FAIL!\r\n");
	return 0;
}


void usr_random(UART_HandleTypeDef *huart)
{
	// Random Number
  uint32_t tmp = 0;
	//HAL_RNG_GenerateRandomNumber(&hrng, &tmp);
  uprintf(huart, (cu8*)"* RAN = 0x%0.8X\r\n",tmp);
}

void usr_systick(UART_HandleTypeDef *huart)
{
	// Tick
	uprintf(huart, (cu8*)"* SysTick = %d\r\n", HAL_GetTick());
}

const char * usr_reset_cause(uint32_t *reset_code)
{
	const char * reset_cause = "UNKNOWN_RESET";

	//__HAL_RCC_CLEAR_RESET_FLAGS();
	
	if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST)){
		*reset_code = RCC_FLAG_LPWRRST;
		reset_cause = "LOW_POWER_RESET";
	}
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST)){
		*reset_code = RCC_FLAG_WWDGRST;
		reset_cause = "WINDOW_WATCHDOG_RESET";
	}
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST)){
		*reset_code = RCC_FLAG_IWDGRST;
		reset_cause = "INDEPENDENT_WATCHDOG_RESET";
	}
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST)){
		*reset_code = RCC_FLAG_SFTRST;
		reset_cause = "SOFTWARE_RESET"; // This reset is induced by calling the ARM CMSIS `NVIC_SystemReset()` function!
	}
/*	else if(__HAL_RCC_GET_FLAG(RCC_FLAG_FWRST)){
		*reset_code = RCC_FLAG_FWRST;
		reset_cause = "FIREWALL_RESET";
	}

	// Needs to come *after* checking the `RCC_FLAG_PORRST` flag in order to ensure first that the reset cause is 
	// NOT a POR/PDR reset. See note below. 
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_OBLRST)){
		*reset_code = RCC_FLAG_OBLRST;
		reset_cause = "OBLRST_RESET";
	}
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_BORRST)){
		*reset_code = RCC_FLAG_BORRST;
		reset_cause = "BROWNOUT_RESET";
	}
*/	
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST)){
		*reset_code = RCC_FLAG_PINRST;
		reset_cause = "PIN_RESET";
	}
	//else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST)){
	//  *reset_code = RCC_FLAG_PORRST;
	//  reset_cause = "POWER-ON_RESET (POR) / POWER-DOWN_RESET (PDR)";
	//}
	else {
		reset_cause = "UNKNOWN_RESET";
	}

	// Clear all the reset flags or else they will remain set during future resets until system power is fully removed.
	__HAL_RCC_CLEAR_RESET_FLAGS();

	return reset_cause; 
}

void usr_rtc_clock_set(UART_HandleTypeDef *huart)
{
	//RTC clock
	//clk_set_date(00, 00, 00, DAY_MON); ////// RTC
	//clk_set_time(00, 00, 00); ////// RTC
	//clk_get(&clk);
	//uprintf(debug_uart,(cu8 *)"* %4d-%d-%d(%s) ", 2000+clk.year, clk.month, clk.date, clkDAY[clk.day]);
	//uprintf(debug_uart,(cu8 *)"%2d:%0.2d:%0.2d\r\n", clk.hour, clk.min, clk.sec);
	//
}
