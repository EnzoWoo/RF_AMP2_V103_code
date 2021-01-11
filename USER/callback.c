/*---------------------------------------------------------------------
by Kang Woo Jung: 2019.02.18

-----------------------------------------------------------------------*/
//#include "ktype.h"
#include "usr.h"

//-----------------------------------------------------------------------------------
/*
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	//TP2 ^= 1; ///////////////////////////////////////////////////
	uint16_t i=0;
	for(uint16_t j=0 ; j<(ADC1_CH_NUM/2) ; j++){
		(*(uint32_t *) &an.adc[i]) = (*(uint32_t *) &an.adc1_buf[j]);  i+=2;
	}
	//static uint16_t i;
	//an.adc1_buf[i] = HAL_ADC_GetValue(hadc);
	//if(++i >= ADC1_CH_NUM) i=0;
}
*/
//-----------------------------------------------------------------------------------
/*
void HAL_COMP_TriggerCallback(COMP_HandleTypeDef *hcomp)
{
	if(hcomp->Instance == COMP1){
		
	}
	else if(hcomp->Instance == COMP2){
		
	}
}
*/
//-----------------------------------------------------------------------------------
/*
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == GPIO_PIN_3){
  }
}
*/


//-----------------------------------------------------------------------------------
/*
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)		// 인터럽트 시작은  HAL_TIM_Base_Start_IT(&htimXX);
//void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
//void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
//void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
//void HAL_TIM_TriggerCallback(TIM_HandleTypeDef *htim)
//void HAL_TIM_ErrorCallback(TIM_HandleTypeDef *htim)
{

	#define BEAM_TIM  TIM15
	if(htim->Instance == BEAM_TIM){ // 31.25us
		//TP11=1; /////////////////////////////////
		// Beam LED 출력 
		static int16_t cnt, cnt2;
		if(++cnt == 64){ // 2ms
			cnt = 0;
			if(++cnt2 == 50) cnt2 = 0; // 100ms
		}
		if(cnt < 10 && cnt2 < 10)	TIM15->CCR1 = TIM15->CCR2 = TIM15->ARR/3;
		else TIM15->CCR1 = TIM15->CCR2 = 0;		
    cnt++; cnt%=100;  //주기 3.125ms
    cnt2+=(cnt==0); cnt2%=100; //주기 312.5 ms
		BEAM_TIM->CCR1 = BEAM_TIM->CCR2 = (BEAM_TIM->ARR/3) * (cnt < 10 && cnt2 < 10);
		//TP11=0; /////////////////////////////////
	}

	
	if(htim->Instance == TIM16){
	}
}

*/
//-----------------------------------------------------------------------------------








