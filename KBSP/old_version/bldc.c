/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/
#include "bsp.h"
#include "usr.h"/////////////////////
#include "bldc.h"


/*	60도 Hall pattern 1,3,7,6,4,0 // bit0 = U, bit1= V, bit2 = W
    120도 Hall pattern 5,1,3,2,6,4 // bit0 = U, bit1= V, bit2 = W	
    //120도 Hall pattern 1,3,2,6,4,5 // bit0 = U, bit1= V, bit2 = W	// 60도 지연
		//CW
		TOP pattern  1,2,2,4,4,1	// bit0 = U, bit1= V, bit2 = W 
		BOT pattern  4,4,1,1,2,2	// bit0 = U, bit1= V, bit2 = W 
		//CCW
		TOP pattern  4,4,1,1,2,2	// bit0 = U, bit1= V, bit2 = W 
		BOT pattern  1,2,2,4,4,1	// bit0 = U, bit1= V, bit2 = W 
*/
#if	HALL_MODE == HALL_MODE_60		//hall 60도 mode
	uint16_t bldc_hall_pattern[6]={1,3,7,6,4,0};
	uint16_t bldc_top_pattern[8]={1,1,0,2,4,0,4,2};
	uint16_t bldc_bot_pattern[8]={2,4,0,4,2,0,1,1};
	
#elif HALL_MODE == HALL_MODE_120		//hall 120도 mode
	uint16_t bldc_hall_pattern[6]={1,3,2,6,4,5}; //60도 지연
	//uint16_t bldc_hall_pattern[6]={5,1,3,2,6,4};
	uint16_t bldc_top_pattern[8]={0,1,2,2,4,1,4,0};
	uint16_t bldc_bot_pattern[8]={0,4,1,4,2,2,1,0};
#endif

void bldc_position_init(bldc_t *bldc)
{
	// Position
	bldc->pos_cnt = POS_VAL_OFFSET / BLDC_mmPP;
	bldc->pos_val = POS_VAL_OFFSET;
	bldc->pos_ave1 = 0;
	bldc->pos_ave2 = 0;
}
void bldc_velocity_init(bldc_t *bldc)
{
  // Velocity
	bldc->rpm_val = 0;
	bldc->rpm_ave1 = 0;
	bldc->rpm_ave2 = 0;
	bldc->vel_val = 0;
	bldc->vel_ave1 = 0;
	bldc->vel_ave2 = 0;
}

void bldc_torque_init(bldc_t *bldc)
{
	// Torque
	bldc->tor_val = 0;
	bldc->tor_ave1 = 0;
	bldc->tor_ave2 = 0;
}

void bldc_init(bldc_t *bldc)
{
	bldc_position_init(bldc);
	bldc_velocity_init(bldc);
	bldc_torque_init(bldc);
/*	
	// Position
	bldc->pos_cnt = POS_VAL_OFFSET / BLDC_mmPP;
	bldc->pos_val = POS_VAL_OFFSET;
	bldc->pos_ave1 = 0;
	bldc->pos_ave2 = 0;
  // Velocity
	bldc->rpm_val = 0;
	bldc->rpm_ave1 = 0;
	bldc->rpm_ave2 = 0;
	bldc->vel_val = 0;
	bldc->vel_ave1 = 0;
	bldc->vel_ave2 = 0;
	// Torque
	bldc->watt = watt;
	bldc->tor_val = 0;
	bldc->tor_ave1 = 0;
	bldc->tor_ave2 = 0;
*/	
}

void bldc_drive(bldc_t *bldc)
{
	//uint16_t hall = HALL_IN_PATTERN;
	
	//hall = bldc_simulation(BLDC_CW, 60); /// 시험용
	//TP3 = TypeBit16(hall).b0;
	//TP4 = TypeBit16(hall).b1;
	//TP5 = TypeBit16(hall).b2;

	BLDC_nF_R = bldc->ccw ^ bldc->dir; ////////////////
	
	if(bldc->pwm == 0){ // For MOSFET Hi-Side Boost Power
		BLDC_PWM = 0;
		//BLDC_TOP1 = 0;
		//BLDC_TOP2 = 0;
		//BLDC_TOP3 = 0;
		//BLDC_PWM1 = 0;
		//BLDC_PWM2 = 0;
		//BLDC_PWM3 = 0;
		return;
	}
	else {
		uint16_t pwm = bldc->pwm;
		if(pwm > BLDC_PWM_TOP) pwm = BLDC_PWM_TOP;
		BLDC_PWM = pwm; //////////////////////////////////// 
		
		// motor phase drive out 
		if(bldc->ccw ^ bldc->dir)	{ // 
			// reverse :: top과 bot pattern swap
			/*
			BLDC_PWM1 = pwm * TypeBit16(bldc_top_pattern[hall]).b0;	// Bottom pwm 0, U
			BLDC_PWM2 = pwm * TypeBit16(bldc_top_pattern[hall]).b1;	// Bottom pwm 1, V
			BLDC_PWM3 = pwm * TypeBit16(bldc_top_pattern[hall]).b2;	// Bottom pwm 2, W
			BLDC_TOP1 = (pwm > 0) & TypeBit16(bldc_bot_pattern[hall]).b0;	// Top logic 0, U
			BLDC_TOP2 = (pwm > 0) & TypeBit16(bldc_bot_pattern[hall]).b1;	// Top logic 1, V
			BLDC_TOP3 = (pwm > 0) & TypeBit16(bldc_bot_pattern[hall]).b2;	// Top logic 2, W
			*/
		}
		else {
			// forward
			/*
			BLDC_PWM1 = pwm * TypeBit16(bldc_bot_pattern[hall]).b0;	// Bottom pwm 0, U
			BLDC_PWM2 = pwm * TypeBit16(bldc_bot_pattern[hall]).b1;	// Bottom pwm 1, V
			BLDC_PWM3 = pwm * TypeBit16(bldc_bot_pattern[hall]).b2;	// Bottom pwm 2, W
			BLDC_TOP1 = (pwm > 0) & TypeBit16(bldc_top_pattern[hall]).b0;	// Top logic 0, U
			BLDC_TOP2 = (pwm > 0) & TypeBit16(bldc_top_pattern[hall]).b1;	// Top logic 1, V
			BLDC_TOP3 = (pwm > 0) & TypeBit16(bldc_top_pattern[hall]).b2;	// Top logic 2, W
			*/
		}
	}
}

void bldc_measurement(bldc_t *bldc, float watt)
{
	// Position
	uint16_t hall = HALL_IN_PATTERN;
	if(bldc->hall_prev ^ hall){
		bldc->period = ev_capture(&bldc->ev_cap);
		bldc->vel_rst = 0;
		ev_reset(&bldc->ev_rst);
		if( ((hall==bldc_hall_pattern[0]) && (bldc->hall_prev==bldc_hall_pattern[5]))
			||((hall==bldc_hall_pattern[1]) && (bldc->hall_prev==bldc_hall_pattern[0]))
			||((hall==bldc_hall_pattern[2]) && (bldc->hall_prev==bldc_hall_pattern[1]))
			||((hall==bldc_hall_pattern[3]) && (bldc->hall_prev==bldc_hall_pattern[2]))
			||((hall==bldc_hall_pattern[4]) && (bldc->hall_prev==bldc_hall_pattern[3]))
			||((hall==bldc_hall_pattern[5]) && (bldc->hall_prev==bldc_hall_pattern[4]))
			) {
			if(bldc->dir) bldc->pos_cnt--;
			else  bldc->pos_cnt++;
			//if(bldc->dir) bldc->pos_cnt++;
			//else  bldc->pos_cnt--;
		}
		else if( ((hall==bldc_hall_pattern[0]) && (bldc->hall_prev==bldc_hall_pattern[1]))
			||((hall==bldc_hall_pattern[1]) && (bldc->hall_prev==bldc_hall_pattern[2]))
			||((hall==bldc_hall_pattern[2]) && (bldc->hall_prev==bldc_hall_pattern[3]))
			||((hall==bldc_hall_pattern[3]) && (bldc->hall_prev==bldc_hall_pattern[4]))
			||((hall==bldc_hall_pattern[4]) && (bldc->hall_prev==bldc_hall_pattern[5]))
			||((hall==bldc_hall_pattern[5]) && (bldc->hall_prev==bldc_hall_pattern[0]))
			) {
			if(bldc->dir) bldc->pos_cnt++;
			else  bldc->pos_cnt--;
			//if(bldc->dir) bldc->pos_cnt--;
			//else  bldc->pos_cnt++;
	  }
		bldc->hall_prev = hall;
	}

	// RPM, Velocity
	bldc->vel_rst |= event(&bldc->ev_rst, 1000000);
	if(bldc->vel_rst){
		//RPM, Velocity RESET
		bldc->rpm_val = 0;
		bldc->rpm_ave1 = 0;
		bldc->rpm_ave2 = 0;
		bldc->vel_val = 0;
		bldc->vel_ave1 = 0;
		bldc->vel_ave2 = 0;
	}
	else {
		//RPM CAl
		bldc->rpm_val = (1000000*60/BLDC_MG_NUM/BLDC_STEP_NUM) / bldc->period; //  rpm = (1MHz * 60 SEC / 4 POLE) / 6 STEP) / Period
		bldc->rpm_ave1 = Filter(0.001F, bldc->rpm_val, bldc->rpm_ave1);
		bldc->rpm_ave2 = Filter(0.001F, bldc->rpm_ave1, bldc->rpm_ave2);
		
		//Velocity CAl
		bldc->vel_val = bldc->rpm_val * BLDC_CIRCUMFERENCE / (BLDC_GEER_RATIO * 60.0F); // mm/s
		bldc->vel_ave1 = Filter(0.005F, bldc->vel_val, bldc->vel_ave1);
		bldc->vel_ave2 = Filter(0.001F, bldc->vel_ave1, bldc->vel_ave2);
		
		//Position CAl
		bldc->pos_val = (float)bldc->pos_cnt;
		bldc->pos_val *= BLDC_mmPP;   // unit: mm/pulse
		bldc->pos_ave1 = Filter(0.01F, bldc->pos_val - POS_VAL_OFFSET, bldc->pos_ave1);  ////////////////// POS_VAL_OFFSET = 50 이면, 중요!!! val=50 일 때  ave1=0 표시!!!
		bldc->pos_ave2 = Filter(0.01F, bldc->pos_ave1, bldc->pos_ave2);
	}
	
	// Torque
	// 1W = 1N.m = 0.1097kgf.m
	if(event(&bldc->ev_tor, 100000)){ // 0.1sec
		bldc->tor_val = watt * 0.1968F; // W --> kgf.m
		bldc->tor_val *= BLDC_GEER_RATIO;
		bldc->tor_val *= (BLDC_PULLY_PIE/2000.0F);
		//bldc->tor_val *= (fabs(bldc->pos_val - bldc->pos_pre)/100.0F);  //  "/100.0F" is mean "/1000(mm -> m) *10(0.1s->sec)"
		//bldc->pos_pre = bldc->pos_val; // backup
		//bldc->tor_val *= (bldc->vel_val/10000.0f);   //  "/10000.0F" is mean "/1000(mm -> m) /10(sec -> 0.1sec)"
		bldc->tor_ave1 = Filter(0.5F, bldc->tor_val, bldc->tor_ave1);
		bldc->tor_ave2 = Filter(0.5F, bldc->tor_ave1, bldc->tor_ave2);
	}
  // 충격감지 알고리즘: (ave1 > ave2 + set_val), 즉 ave2의 추종시간(factor 값)과 set_val에  따라 감도가 조정 됨.
  // 필터의 factor	값과 set_val가 낮을 수록 감도가 예민함.

}

uint16_t bldc_simulation(uint16_t ccw, uint16_t  rpm) // 120도
{
	static evt_t evt;
	static uint16_t step;
	uint16_t hall;
	
	hall = bldc_hall_pattern[step];
	
	/* Event process */
	// T = 1/(rpm/60) /6step --> T[us] = T/(10^-6) = 60*10^6/6step/rpm = 10^7/rpm
	if(!event(&evt, 10000000L / rpm) ) return hall;

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



