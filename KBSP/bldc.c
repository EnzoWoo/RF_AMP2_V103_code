/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/
#include "bsp.h"
#include "usr.h"/////////////////////
#include "bldc.h"
#include "bldc_sin.h"


#define SIN_TBL_60 	(SIN_TBL_SIZ/6)
enum SIN_Index {
	SIN_Index_0 = 0,
	SIN_Index_60 = (SIN_TBL_60),
	SIN_Index_120 = (SIN_TBL_60*2),
	SIN_Index_180 = (SIN_TBL_60*3),
	SIN_Index_240 = (SIN_TBL_60*4),
	SIN_Index_300 = (SIN_TBL_60*5)
};

//CW : HALL 4,5,1,3,2,6
const uint16_t sin_sync_cw[8] = {
	0,  //0
	SIN_Index_120, //1
	SIN_Index_240, //2
	SIN_Index_180, //3
	SIN_Index_0,   //4
	SIN_Index_60,  //5
	SIN_Index_300, //6
	0,  //7
};

//CCW : HALL 1,5,4,6,2,3
const uint16_t sin_sync_ccw[8] = {
	0,  //0
	SIN_Index_0,   //1
	SIN_Index_240, //2
	SIN_Index_300, //3
	SIN_Index_120, //4
	SIN_Index_60,  //5
	SIN_Index_180, //6
	0,  //7
};
	

struct {
	evt_t evt;   // rcc
	evt_t delay; // rpm
	tm_t cap;
	uint32_t cnt;
	uint32_t sum;
	uint16_t hall_prev; 
	float prev;	
}rpm;


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1){ // 16kHz
	  //TP13 = 1;/////////////////////////
		//
		static uint16_t hall_prev;
		uint16_t hall;
		hall = HALL_IN_PATTERN;
		if(bldc.hall_inv){
			// 60W
			hall ^= 0x07;
			hall &= 0x07;
		}
		bldc.hall = hall; ///////////////////////////////////
		
		if(hall_prev ^ hall){
			hall_prev = hall;
			bldc.pwm_step_num = bldc.pwm_step_cnt;
			bldc.pwm_step_cnt = 0;
			
			// SYNC HALL & SIN_Index
			if(bldc.ccw ^ bldc.dir)
    				bldc.sin_index = sin_sync_ccw[hall]; //CCW
			else	bldc.sin_index = sin_sync_cw[hall];  //CW
		}
		
		if(++bldc.pwm_step_cnt > BLDC_SIN_STEP_NUM_LIMIT){ // 60 rpm 이하
			bldc.pwm_step_cnt = BLDC_SIN_STEP_NUM_LIMIT;
			bldc.en_sin_ctr = 0; // Disable Sin Control
		}
		else {
			// SIN Control
			if(bldc.en_sin_ctr){
				bldc.sin_index += (SIN_TBL_SIZ / 6 / bldc.pwm_step_num);
				if(bldc.sin_index >= SIN_TBL_SIZ) bldc.sin_index = 0;

				if(bldc.pwm > BLDC_PWM_TOP) bldc.pwm = BLDC_PWM_TOP;
				
				uint16_t ui,vi,wi;
				ui = bldc.sin_index;
				ui %= SIN_TBL_SIZ;
				vi = bldc.sin_index+4000;
				vi %= SIN_TBL_SIZ;
				wi = bldc.sin_index+2000;
				wi %= SIN_TBL_SIZ;
				
				if(bldc.ccw ^ bldc.dir)	{
					//CCW
					if(ui < 2000)	bldc.pwm2 = (int32_t)SIN_TBL[ui]*bldc.pwm/SIN_MAX_VAL;
					else if(ui > 4000) bldc.pwm2 = 0;
					else 	bldc.pwm2 = (int32_t)SIN_TBL[ui-1000]*bldc.pwm/SIN_MAX_VAL;

					if(vi < 2000)	bldc.pwm1 = (int32_t)SIN_TBL[vi]*bldc.pwm/SIN_MAX_VAL;
					else if(vi > 4000) bldc.pwm1 = 0;
					else 	bldc.pwm1 = (int32_t)SIN_TBL[vi-1000]*bldc.pwm/SIN_MAX_VAL;
					
					if(wi < 2000)	bldc.pwm3 = (int32_t)SIN_TBL[wi]*bldc.pwm/SIN_MAX_VAL;
					else if(wi > 4000) bldc.pwm3 = 0;
					else 	bldc.pwm3 = (int32_t)SIN_TBL[wi-1000]*bldc.pwm/SIN_MAX_VAL;
				}
				else{
					//CW
					if(ui < 2000)	bldc.pwm1 = (int32_t)SIN_TBL[ui]*bldc.pwm/SIN_MAX_VAL;
					else if(ui > 4000) bldc.pwm1 = 0;
					else 	bldc.pwm1 = (int32_t)SIN_TBL[ui-1000]*bldc.pwm/SIN_MAX_VAL;
					
					if(vi < 2000)	bldc.pwm2 = (int32_t)SIN_TBL[vi]*bldc.pwm/SIN_MAX_VAL;
					else if(vi > 4000) bldc.pwm2 = 0;
					else 	bldc.pwm2 = (int32_t)SIN_TBL[vi-1000]*bldc.pwm/SIN_MAX_VAL;
					
					if(wi < 2000)	bldc.pwm3 = (int32_t)SIN_TBL[wi]*bldc.pwm/SIN_MAX_VAL;
					else if(wi > 4000) bldc.pwm3 = 0;
					else 	bldc.pwm3 = (int32_t)SIN_TBL[wi-1000]*bldc.pwm/SIN_MAX_VAL;
				}
				//DAC output for monitor
				HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, bldc.pwm1*4); // DAC_TOP 4095, PWM_TOP 999
				// PWM Out
				BLDC_PWM1 = bldc.pwm1;
				BLDC_PWM2 = bldc.pwm2;
				BLDC_PWM3 = bldc.pwm3;
			}
		}
		//
	  //TP13 = 0;/////////////////////////
	}
	else {
	}
}

void bldc_setup(bldc_t * bldc, float * torq)
{
	bldc->torq_pt = torq;
	//
	bldc->error.hall = 0;	
	bldc->error.direct = 0;	
}

void bldc_drive_process(bldc_t *bldc)
{
	bldc->hall = HALL_IN_PATTERN; // !!!

	//TP17 = TypeBit16(bldc->hall).b0;
	//TP18 = TypeBit16(bldc->hall).b1;
	//TP11 = TypeBit16(bldc->hall).b2;

	bldc_get_torq(bldc);
	bldc_get_rpm(bldc);
	bldc_get_disp(bldc);

	
	//Set SIN-Control ?
	if(bldc->en_sin_ctr == 0){
		if(bldc->pwm){
			//SIN rms 
			uint32_t tmp;
			tmp = bldc->pwm * 50;
			bldc->pwm_rect = (uint16_t)(tmp/100);
			
			// Switch SIN Control 
			if(bldc->rpm_ave1 > BLDC_SIN_CTR_START_RPM) bldc->en_sin_ctr = 1; // > 240rmp : Set SIN-Control
			
			// DRIVE MODE SEL
			static uint16_t mode;
			mode = 1;//////////////////////////
			if(mode == 0){ // Normal Drive
				//bldc->hall = bldc_simulation(120,bldc->ccw,400); ///////////////////////////////////// 시험용
				bldc_drive_rectangle(bldc);
			}
			else if(mode == 1){ // Triangle
				//bldc->hall = bldc_simulation(120,bldc->ccw,400); ///////////////////////////////////// 시험용
				bldc_drive_triangle(bldc);
			}
			else if(mode == 2){ // Sin Drive Simulation #1
				bldc_drive_sin1(bldc);
			}
			else if(mode == 3){ // Sin Drive Simulation #2
				bldc_drive_sin2(bldc);
			}
		}
		else {
			bldc->pwm1 = bldc->pwm2 = bldc->pwm3 = 0;
			//bldc->pwm1 = bldc->pwm2 = bldc->pwm3 = BLDC_PWM_MEAN;
		}
		//DAC output for monitor
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (bldc->pwm1-BLDC_PWM_MEAN)*4+2018); // DAC_TOP 4095, PWM_TOP 999
		// PWM output
		BLDC_PWM1 = bldc->pwm1;
		BLDC_PWM2 = bldc->pwm2;
		BLDC_PWM3 = bldc->pwm3;
	}

	// 예외처리
	if(bldc->pwm && bldc->rpm_ave2 > 180){
		if(bldc->hall_ccw != bldc->ccw) bldc->error.direct = 1;
		else  bldc->error.direct = 0;
	}
	else {
		bldc->error.direct = 0;
	}
}

const uint16_t bldc_wave_top[8]={1,1,2,2,4,1,4,2}; // b0=U, b1=V, b2=W
const uint16_t bldc_wave_bot[8]={2,4,1,4,2,2,1,1}; // b0=U, b1=V, b2=W
void bldc_drive_rectangle(bldc_t * bldc)
{
	if(bldc->pwm_rect > BLDC_PWM_MEAN) bldc->pwm_rect = BLDC_PWM_MEAN;
	if(bldc->ccw ^ bldc->dir)	{ //
		// reverse :: top과 bot pattern swap
		if(TypeBit16(bldc_wave_bot[bldc->hall]).b0){      bldc->pwm1 = BLDC_PWM_MEAN + bldc->pwm_rect; }
		else if(TypeBit16(bldc_wave_top[bldc->hall]).b0){	bldc->pwm1 = BLDC_PWM_MEAN - bldc->pwm_rect; }
		else { bldc->pwm1 = BLDC_PWM_MEAN; }
		if(TypeBit16(bldc_wave_bot[bldc->hall]).b1){      bldc->pwm2 = BLDC_PWM_MEAN + bldc->pwm_rect; }
		else if(TypeBit16(bldc_wave_top[bldc->hall]).b1){	bldc->pwm2 = BLDC_PWM_MEAN - bldc->pwm_rect; }
		else { bldc->pwm2 = BLDC_PWM_MEAN;	}
		if(TypeBit16(bldc_wave_bot[bldc->hall]).b2){      bldc->pwm3 = BLDC_PWM_MEAN + bldc->pwm_rect; }
		else if(TypeBit16(bldc_wave_top[bldc->hall]).b2){	bldc->pwm3 = BLDC_PWM_MEAN - bldc->pwm_rect; }
		else { bldc->pwm3 = BLDC_PWM_MEAN; }
	}
	else {
		// forward
		if(TypeBit16(bldc_wave_top[bldc->hall]).b0){      bldc->pwm1 = BLDC_PWM_MEAN + bldc->pwm_rect; }
		else if(TypeBit16(bldc_wave_bot[bldc->hall]).b0){	bldc->pwm1 = BLDC_PWM_MEAN - bldc->pwm_rect; }
		else { bldc->pwm1 = BLDC_PWM_MEAN; }
		if(TypeBit16(bldc_wave_top[bldc->hall]).b1){      bldc->pwm2 = BLDC_PWM_MEAN + bldc->pwm_rect; }
		else if(TypeBit16(bldc_wave_bot[bldc->hall]).b1){	bldc->pwm2 = BLDC_PWM_MEAN - bldc->pwm_rect; }
		else { bldc->pwm2 = BLDC_PWM_MEAN; }
		if(TypeBit16(bldc_wave_top[bldc->hall]).b2){      bldc->pwm3 = BLDC_PWM_MEAN + bldc->pwm_rect; }
		else if(TypeBit16(bldc_wave_bot[bldc->hall]).b2){	bldc->pwm3 = BLDC_PWM_MEAN - bldc->pwm_rect; }
		else { bldc->pwm3 = BLDC_PWM_MEAN; }
	}
}


//Triangle
#define BLDC_TOP_MAK(pwm_set,k,pwm,pwm_out) 	{ if(pwm_set > pwm+k){ pwm+=k; pwm_out = BLDC_PWM_MEAN + pwm; } else { pwm_out = BLDC_PWM_MEAN + pwm_set; }}
#define BLDC_TOP_BRA(pwm_set,k,pwm,pwm_out) 	{ if(pwm > k){ pwm-=k; pwm_out = BLDC_PWM_MEAN + pwm;} else{ pwm_out = BLDC_PWM_MEAN; }}
#define BLDC_BOT_MAK(pwm_set,k,pwm,pwm_out) 	{ if(pwm_set > pwm+k){ pwm+=k; pwm_out = BLDC_PWM_MEAN - pwm; } else { pwm_out = BLDC_PWM_MEAN - pwm_set; }}
#define BLDC_BOT_BRA(pwm_set,k,pwm,pwm_out) 	{ if(pwm > k){ pwm-=k; pwm_out = BLDC_PWM_MEAN - pwm;} else{ pwm_out = BLDC_PWM_MEAN; }}
void bldc_drive_triangle(bldc_t * bldc)
{
	static uint16_t pwm1,pwm2,pwm3;
	//Triangle 시정수!  매우중요!!!! 속도에 따라 튜닝 필요!!!!
	if(bldc->pwm > BLDC_PWM_MEAN) bldc->pwm = BLDC_PWM_MEAN;
 
	uint32_t k = bldc->pwm/10;
	k *= k;
	k *= k;
	k /= 4000; // 수가 클수록 기울기 커짐 // loop time을 단위로 시간계산
	k++; //중요!
	k=10;
/*
	uint32_t k = bldc->pwm;
	k *= k;
	//k /= 1500; // 수가 클수록 기울기 커짐 // loop time을 단위로 시간계산
	k /= 4000; // 수가 클수록 기울기 커짐 // loop time을 단위로 시간계산
	k++; //중요!
*/
	if(bldc->ccw ^ bldc->dir){ //Reverse
				 if(bldc->hall==2||bldc->hall==7||bldc->hall==6){ BLDC_TOP_MAK(bldc->pwm_rect,k,pwm1,bldc->pwm1); }
		else if(bldc->hall==3)               { BLDC_TOP_BRA(bldc->pwm_rect,k,pwm1,bldc->pwm1); }
		else if(bldc->hall==5||bldc->hall==0||bldc->hall==1){ BLDC_BOT_MAK(bldc->pwm_rect,k,pwm1,bldc->pwm1); }
		else if(bldc->hall==4)               { BLDC_BOT_BRA(bldc->pwm_rect,k,pwm1,bldc->pwm1); }

				 if(bldc->hall==4||bldc->hall==5||bldc->hall==0){ BLDC_TOP_MAK(bldc->pwm_rect,k,pwm2,bldc->pwm2); }
		else if(bldc->hall==6)               { BLDC_TOP_BRA(bldc->pwm_rect,k,pwm2,bldc->pwm2); }
		else if(bldc->hall==3||bldc->hall==2||bldc->hall==7){ BLDC_BOT_MAK(bldc->pwm_rect,k,pwm2,bldc->pwm2); }
		else if(bldc->hall==1)               { BLDC_BOT_BRA(bldc->pwm_rect,k,pwm2,bldc->pwm2); }

				 if(bldc->hall==1||bldc->hall==3){ BLDC_TOP_MAK(bldc->pwm_rect,k,pwm3,bldc->pwm3); }
		else if(bldc->hall==5||bldc->hall==0||bldc->hall==0){ BLDC_TOP_BRA(bldc->pwm_rect,k,pwm3,bldc->pwm3); }
		else if(bldc->hall==6||bldc->hall==4){ BLDC_BOT_MAK(bldc->pwm_rect,k,pwm3,bldc->pwm3); }
		else if(bldc->hall==2||bldc->hall==7)               { BLDC_BOT_BRA(bldc->pwm_rect,k,pwm3,bldc->pwm3); }
	}
	else { // forward
				 if(bldc->hall==5||bldc->hall==0||bldc->hall==1){ BLDC_TOP_MAK(bldc->pwm_rect,k,pwm1,bldc->pwm1); }
		else if(bldc->hall==3)               { BLDC_TOP_BRA(bldc->pwm_rect,k,pwm1,bldc->pwm1); }
		else if(bldc->hall==2||bldc->hall==7||bldc->hall==6){ BLDC_BOT_MAK(bldc->pwm_rect,k,pwm1,bldc->pwm1); }
		else if(bldc->hall==4)               { BLDC_BOT_BRA(bldc->pwm_rect,k,pwm1,bldc->pwm1); }

				 if(bldc->hall==3||bldc->hall==2||bldc->hall==7){ BLDC_TOP_MAK(bldc->pwm_rect,k,pwm2,bldc->pwm2); }
		else if(bldc->hall==6)               { BLDC_TOP_BRA(bldc->pwm_rect,k,pwm2,bldc->pwm2); }
		else if(bldc->hall==4||bldc->hall==5||bldc->hall==0){ BLDC_BOT_MAK(bldc->pwm_rect,k,pwm2,bldc->pwm2); }
		else if(bldc->hall==1)               { BLDC_BOT_BRA(bldc->pwm_rect,k,pwm2,bldc->pwm2); }

				 if(bldc->hall==6||bldc->hall==4){ BLDC_TOP_MAK(bldc->pwm_rect,k,pwm3,bldc->pwm3); }
		else if(bldc->hall==5||bldc->hall==0)               { BLDC_TOP_BRA(bldc->pwm_rect,k,pwm3,bldc->pwm3); }
		else if(bldc->hall==1||bldc->hall==3){ BLDC_BOT_MAK(bldc->pwm_rect,k,pwm3,bldc->pwm3); }
		else if(bldc->hall==2||bldc->hall==7)               { BLDC_BOT_BRA(bldc->pwm_rect,k,pwm3,bldc->pwm3); }
	}
}


void bldc_drive_sin1(bldc_t * bldc)
{
	static uint16_t ci,ui,vi,wi,pwm;
	
	pwm = bldc->pwm /2;
	ci += (pwm/2);
	ci %= SIN_TBL_SIZ;
	
	ui = ci+0;
	ui %= SIN_TBL_SIZ;
	vi = ci+4000;
	vi %= SIN_TBL_SIZ;
	wi = ci+2000;
	wi %= SIN_TBL_SIZ;

	
	if(bldc->ccw ^ bldc->dir)	{
		bldc->pwm1 = BLDC_PWM_MEAN + (int32_t)SIN_TBL[ui]*pwm/SIN_MAX_VAL;
		bldc->pwm3 = BLDC_PWM_MEAN + (int32_t)SIN_TBL[vi]*pwm/SIN_MAX_VAL;
		bldc->pwm2 = BLDC_PWM_MEAN + (int32_t)SIN_TBL[wi]*pwm/SIN_MAX_VAL;
	}
	else{
		bldc->pwm1 = BLDC_PWM_MEAN + (int32_t)SIN_TBL[ui]*pwm/SIN_MAX_VAL;
		bldc->pwm2 = BLDC_PWM_MEAN + (int32_t)SIN_TBL[vi]*pwm/SIN_MAX_VAL;
		bldc->pwm3 = BLDC_PWM_MEAN + (int32_t)SIN_TBL[wi]*pwm/SIN_MAX_VAL;
	}
}

void bldc_drive_sin2(bldc_t * bldc)
{
	static uint16_t ci,ui,vi,wi,pwm;
	pwm = bldc->pwm;
	ci += (bldc->pwm/6);
	ci %= SIN_TBL_SIZ;
	
	ui = ci+0;
	ui %= SIN_TBL_SIZ;
	vi = ci+4000;
	vi %= SIN_TBL_SIZ;
	wi = ci+2000;
	wi %= SIN_TBL_SIZ;
	
	if(bldc->ccw ^ bldc->dir)	{
		if(ui < 2000)	bldc->pwm1 = (int32_t)SIN_TBL[ui]*pwm/SIN_MAX_VAL;
		else if(ui > 4000) bldc->pwm1 = 0;
		else 	bldc->pwm1 = (int32_t)SIN_TBL[ui-1000]*pwm/SIN_MAX_VAL;

		if(vi < 2000)	bldc->pwm3 = (int32_t)SIN_TBL[vi]*pwm/SIN_MAX_VAL;
		else if(vi > 4000) bldc->pwm3 = 0;
		else 	bldc->pwm3 = (int32_t)SIN_TBL[vi-1000]*pwm/SIN_MAX_VAL;
		
		if(wi < 2000)	bldc->pwm2 = (int32_t)SIN_TBL[wi]*pwm/SIN_MAX_VAL;
		else if(wi > 4000) bldc->pwm2 = 0;
		else 	bldc->pwm2 = (int32_t)SIN_TBL[wi-1000]*pwm/SIN_MAX_VAL;
	}
	else{
		if(ui < 2000)	bldc->pwm1 = (int32_t)SIN_TBL[ui]*pwm/SIN_MAX_VAL;
		else if(ui > 4000) bldc->pwm1 = 0;
		else 	bldc->pwm1 = (int32_t)SIN_TBL[ui-1000]*pwm/SIN_MAX_VAL;
		
		if(vi < 2000)	bldc->pwm2 = (int32_t)SIN_TBL[vi]*pwm/SIN_MAX_VAL;
		else if(vi > 4000) bldc->pwm2 = 0;
		else 	bldc->pwm2 = (int32_t)SIN_TBL[vi-1000]*pwm/SIN_MAX_VAL;
		
		if(wi < 2000)	bldc->pwm3 = (int32_t)SIN_TBL[wi]*pwm/SIN_MAX_VAL;
		else if(wi > 4000) bldc->pwm3 = 0;
		else 	bldc->pwm3 = (int32_t)SIN_TBL[wi-1000]*pwm/SIN_MAX_VAL;
	}	
}

void bldc_get_torq(bldc_t * bldc)
{
	bldc->torq_val = *bldc->torq_pt;
	bldc_get_dTorq(bldc);
}


void bldc_get_rpm(bldc_t * bldc)
{
  // Cal RPM
	if(rpm.hall_prev ^ bldc->hall){
		rpm.hall_prev = bldc->hall;
		rpm.delay = 0;
		bldc->hall_period = evt_capus(&rpm.cap);
		
		rpm.sum += bldc->hall_period; // for rms cal
		if(++rpm.cnt == 6){ // sampling (6-Step)
			bldc->rpm_ave1 = (1000000*60/BLDC_PULSE_NUM) * rpm.cnt / rpm.sum; // 24Pulse/Turn  (6step * 4pole)
			rpm.cnt = 0;
			rpm.sum = 0;
			//			
			bldc->rpm_ave2 = Filter(0.01f, bldc->rpm_ave1, bldc->rpm_ave2); // rpm feedback!!  facktor 값 수정 불가!!!!
		}
		//bldc->rpm_val = (1000000*60/BLDC_PULSE_NUM) / bldc->hall_period; // 24Pulse/Turn  (6step * 4pole)
	}
	// Clear RPM
	if(evt_delayms(&rpm.delay,100)){ 
		//rpm.delay = 100;
		//bldc->rpm_val = 0;
		bldc->rpm_ave1 = 0;
		bldc->rpm_ave2 = 0;
		bldc->hall_period = 10000; // PID control 주기로 사용
	}

	bldc_get_dRpm(bldc);
}

void bldc_get_disp(bldc_t * bldc)
{
	static uint16_t hall_prev;
	
	if(hall_prev ^ bldc->hall){
		if( ((bldc->hall==5) && (hall_prev==4))
			||((bldc->hall==1) && (hall_prev==5))
			||((bldc->hall==3) && (hall_prev==1))
			||((bldc->hall==2) && (hall_prev==3))
			||((bldc->hall==6) && (hall_prev==2))
			||((bldc->hall==4) && (hall_prev==6))
			) {
			if(bldc->dir){
				bldc->disp_val--; // CCW
				bldc->hall_ccw = 1;
			}
			else {
				bldc->disp_val++; //CW
				bldc->hall_ccw = 0;
			}
			bldc->error.hall = 0;
		}
		else if(
				((bldc->hall==4) && (hall_prev==5))
			||((bldc->hall==6) && (hall_prev==4))
			||((bldc->hall==2) && (hall_prev==6))
			||((bldc->hall==3) && (hall_prev==2))
			||((bldc->hall==1) && (hall_prev==3))
			||((bldc->hall==5) && (hall_prev==1))
			) {
			if(bldc->dir){
				bldc->disp_val++; // CW
				bldc->hall_ccw = 0;
			}
			else {
				bldc->disp_val--; // CCW
				bldc->hall_ccw = 1;
			}
			bldc->error.hall = 0;
		}
		else {
			//HALL Error
			if(bldc->pwm) bldc->error.hall = 1;
			else bldc->error.hall = 0;
		}
		
		// Backup DISP
		HAL_RTCEx_BKUPWrite(&hrtc, BK_DISP_ADDR, bldc->disp_val);
		
		//
		hall_prev = bldc->hall;
	}
	
	bldc_get_dDisp(bldc);
}

void bldc_get_dTorq(bldc_t * bldc)
{
	static evt_t evt;
	static float prev;
	if(evt_ms(&evt,200)){
		bldc->dTorq_val = (bldc->torq_val - prev);
		prev = bldc->torq_val;
	}
}

void bldc_get_dRpm(bldc_t * bldc)
{
	static evt_t evt;
	static float prev;
	if(evt_ms(&evt,200)){
		bldc->dRpm_val = fabs(bldc->rpm_ave1 - prev);
		prev = bldc->rpm_ave1;
	}
}

void bldc_get_dDisp(bldc_t * bldc)
{
	static evt_t evt;
	static int32_t prev;
	if(evt_ms(&evt,200)){
		bldc->dDisp_val = abs(bldc->disp_val - prev);
		prev = bldc->disp_val;
	}
}


uint16_t bldc_simulation(uint16_t  deg, uint16_t ccw, uint16_t  rpm)
{
  static uint16_t bldc_hall__60do[6]={0,1,3,7,6,4};     // b0=H1,b1=H2,b2=H3
  static uint16_t bldc_hall_120do[6]={5,1,3,2,6,4};     // b0=H1,b1=H2,b2=H3
	
	static evt_t evt;
	static uint16_t step;
	uint16_t hall;
	
	if(deg == 60)	hall = bldc_hall__60do[step];
	else if(deg == 120)	hall = bldc_hall_120do[step];
	else {	}
	
	/* Event process */
	// T = 1/(rpm/60) /6step --> T[us] = T/(10^-6) = 60*10^6/6step/rpm = 10^7/rpm
	if(!evt_us(&evt, 10000000L / (rpm*4)) ) return hall;

	if(ccw){//reverse
		if(step) step--;
		else step = 5;
	}
	else {//forward
		step++;
		step %= 6;
	}
	
	return hall;
}
