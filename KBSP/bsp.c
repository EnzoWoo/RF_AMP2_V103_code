/*---------------------------------------------------------------------
by Kang Woo Jung: 2018.11.28

-----------------------------------------------------------------------*/
#include "bsp.h"
#include "usr.h" ///////////////////////////

void bsp_ADC_start(void)
{
	//HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
	HAL_ADCEx_Calibration_Start(&hadc1);
  HAL_ADC_Start_DMA(&hadc1, an.adc1_buf, ADC1_CH_NUM);
	
	//HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
  //HAL_ADC_Start_DMA(&hadc2, an.adc2_buf, ADC2_CH_NUM);
	
	//HAL_ADCEx_Calibration_Start(&hadc3, ADC_SINGLE_ENDED);
  //HAL_ADC_Start_DMA(&hadc3, an.adc3_buf, ADC3_CH_NUM);
}
void bsp_COMP_start(void)
{
#ifdef COMP1
  //if(HAL_COMP_Start_IT(&hcomp1) != HAL_OK) Error_Handler(); 
#endif
#ifdef COMP2
  //if(HAL_COMP_Start_IT(&hcomp2) != HAL_OK) Error_Handler(); 
#endif
}
void bsp_DAC_start(void)
{
#ifdef DAC1
  //HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
  //HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 2048);
  //HAL_DAC_Start(&hdac1, DAC_CHANNEL_2);
  //HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 2048);
#endif	
}
void bsp_TIM_start(void)
{
#ifdef TIM1
  //TIM1->CCR1 = TIM1->ARR/4;
  TIM1->CCR2 = 0;
  TIM1->CCR3 = TIM1->ARR;
  //TIM1->CCR4 = TIM1->ARR/4;
  //HAL_TIM_Base_Start(&htim1);
  //HAL_TIM_Base_Start_IT(&htim1);
  //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
  //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
  //HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
  //HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_4);
	//HAL_TIMEx_OCN_Start(&htim1, TIM_CHANNEL_1);
	//HAL_TIMEx_OCN_Start(&htim1, TIM_CHANNEL_2);
	//HAL_TIMEx_OCN_Start(&htim1, TIM_CHANNEL_3);
	//HAL_TIMEx_OCN_Start(&htim1, TIM_CHANNEL_4);
	
#endif	
#ifdef TIM2	
  //TIM2->CCR1 = TIM2->ARR/8;
  //TIM2->CCR2 = TIM2->ARR/4;
  //TIM2->CCR3 = TIM2->ARR/3;
  //TIM2->CCR4 = TIM2->ARR/3;
  HAL_TIM_Base_Start_IT(&htim2);
  //HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  //HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  //HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
  //HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
  //HAL_TIMEx_HallSensor_Start_(&htim2); ///////////////// Hall Sensor
  //HAL_TIMEx_HallSensor_Start_IT(&htim2); ///////////////// Hall Sensor
	
#endif	
#ifdef TIM3		
  //TIM3->CCR1 = TIM3->ARR/2 - 4;
  //TIM3->CCR2 = TIM3->ARR - (TIM3->ARR/2 - 4);
  //TIM3->CCR3 = TIM3->ARR/4;
  //TIM3->CCR4 = TIM3->ARR/4;
  HAL_TIM_Base_Start(&htim3); // for event.c
  //HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  //HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
  //HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
  //HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
  //HAL_TIM_IC_Start(&htim3, TIM_CHANNEL_1);
  //HAL_TIM_IC_Start(&htim3, TIM_CHANNEL_2);
  //HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
  //HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
#endif	
#ifdef TIM4	
  //TIM4->CCR1 = TIM4->ARR/4;
  //TIM4->CCR2 = TIM4->ARR/4;
  //TIM4->CCR3 = TIM4->ARR/4;
  //TIM4->CCR4 = TIM4->ARR/2;
  //HAL_TIM_Base_Start_IT(&htim4);
  //HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
  //HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
  //HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
  //HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
  //HAL_TIMEx_HallSensor_Start_(&htim4); ///////////////// Hall Sensor
  //HAL_TIMEx_HallSensor_Start_IT(&htim4); ///////////////// Hall Sensor
#endif	
#ifdef TIM5		
  //TIM5->CCR1 = TIM5->ARR/4;
  //TIM5->CCR2 = TIM5->ARR/4;
  //TIM5->CCR3 = TIM5->ARR/4;
  //TIM5->CCR4 = TIM5->ARR/4;
  //HAL_TIM_Base_Start(&htim5);	// for Event
  //HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
  //HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2);
  //HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_3);
  //HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_4);
#endif	
#ifdef TIM6		
  //TIM6->ARR = 50-1;
  //HAL_TIM_Base_Start(&htim6); // for DAC output
  //HAL_TIM_Base_Start_IT(&htim6);
#endif	
#ifdef TIM7		
  //TIM7->ARR = 50-1;
  HAL_TIM_Base_Start(&htim7);
  //HAL_TIM_Base_Start_IT(&htim7);
#endif	
#ifdef TIM8		
  //TIM8->CCR1 = TIM8->ARR/4;
  //TIM8->CCR2 = TIM8->ARR/4;
  //TIM8->CCR3 = TIM8->ARR/2;
  //TIM8->CCR4 = TIM8->ARR/4;
  //HAL_TIM_Base_Start(&htim8);
  //HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
  //HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
  //HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
  //HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);
  //HAL_TIMEx_PWMN_Start(&htim8, TIM_CHANNEL_1);
  //HAL_TIMEx_PWMN_Start(&htim8, TIM_CHANNEL_2);
  //HAL_TIMEx_PWMN_Start(&htim8, TIM_CHANNEL_3);
  //HAL_TIMEx_PWMN_Start(&htim8, TIM_CHANNEL_4);
#endif	
#ifdef TIM9		
  //TIM9->CCR1 = TIM9->ARR/4;
  //TIM9->CCR2 = TIM9->ARR/4;
  //HAL_TIM_Base_Start(&htim9);
  //HAL_TIM_IC_Start(&htim9, TIM_CHANNEL_1);
  //HAL_TIM_IC_Start(&htim9, TIM_CHANNEL_2);
  //HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
  //HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_2);
#endif	
#ifdef TIM10		
  //TIM10->CCR1 = TIM10->ARR/4;
  //HAL_TIM_Base_Start(&htim10);
  //HAL_TIM_IC_Start(&htim10, TIM_CHANNEL_1);
  //HAL_TIM_PWM_Start(&htim10, TIM_CHANNEL_1);
#endif	
#ifdef TIM11		
  //TIM11->CCR1 = TIM11->ARR/4;
  //HAL_TIM_Base_Start(&htim11);
  //HAL_TIM_IC_Start(&htim11, TIM_CHANNEL_1);
  //HAL_TIM_PWM_Start(&htim11, TIM_CHANNEL_1);
#endif	
#ifdef TIM12	
  //TIM12->CCR1 = TIM12->ARR/2;
  //TIM12->CCR2 = TIM12->ARR/3;
  //HAL_TIM_Base_Start(&htim12);
  //HAL_TIM_IC_Start(&htim12, TIM_CHANNEL_1);
  //HAL_TIM_IC_Start(&htim12, TIM_CHANNEL_2);
  //HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
  //HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_2);
#endif	
#ifdef TIM13		
  //TIM13->CCR1 = TIM13->ARR/4;
  //HAL_TIM_Base_Start(&htim13);
  //HAL_TIM_IC_Start(&htim13, TIM_CHANNEL_1);
  //HAL_TIM_PWM_Start(&htim13, TIM_CHANNEL_1);
#endif	
#ifdef TIM14	
  //TIM14->CCR1 = TIM14->ARR/4;
  //HAL_TIM_Base_Start(&htim14);
  //HAL_TIM_IC_Start(&htim14, TIM_CHANNEL_1);
  //HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);

#endif	
#ifdef TIM15	
  //TIM15->CCR1 = TIM15->ARR/4;
  //TIM15->CCR2 = TIM15->ARR/4;
  //HAL_TIM_Base_Start_IT(&htim15); //
  //HAL_TIM_IC_Start(&htim15, TIM_CHANNEL_1);
  //HAL_TIM_IC_Start(&htim15, TIM_CHANNEL_2);
  //HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1);
  //HAL_TIMEx_PWMN_Start(&htim15, TIM_CHANNEL_1);
  //HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_2);

#endif	
#ifdef TIM16	
  //TIM16->CCR1 = TIM16->ARR;
  //TIM16->CCR1 = 1;
  //HAL_TIM_Base_Start(&htim16);
  //HAL_TIM_Base_Start_IT(&htim16);
  //HAL_TIM_IC_Start(&htim16, TIM_CHANNEL_1);
  //HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);

#endif	
#ifdef TIM17	
  //TIM17->CCR1 = TIM17->ARR/4;
  //HAL_TIM_Base_Start(&htim17);
  //HAL_TIM_IC_Start(&htim17, TIM_CHANNEL_1);
  //HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1);

#endif	
}

void bsp_UART_Test(uint16_t num)
{
  uint8_t msg[] = "uart test.\r\n";
	msg[4] = '0'+num;
  HAL_UART_Transmit_DMA(&huart1, msg, 5);
}

void bsp_UART_start(void)
{
	static uint8_t rd;

	HAL_UART_Receive_IT(&huart1,&rd,1);  //중요! CallBack 함수 Start! !"rd"는 전역변수 사용!!!
	//bsp_UART_Test(1);
	HAL_UART_Receive_IT(&huart2,&rd,1);  //중요! CallBack 함수 Start! !"rd"는 전역변수 사용!!!
	//bsp_UART_Test(2);
	//HAL_LIN_Init(&huart2,10); /////////////////////////////////////
	HAL_UART_Receive_IT(&huart3,&rd,1);  //중요! CallBack 함수 Start! !"rd"는 전역변수 사용!!!
	//bsp_UART_Test(3);
	//HAL_UART_Receive_IT(&huart4,&rd,1);  //중요! CallBack 함수 Start! !"rd"는 전역변수 사용!!!
	//bsp_UART_Test(4);
	//HAL_UART_Receive_IT(&huart5,&rd,1);  //중요! CallBack 함수 Start! !"rd"는 전역변수 사용!!!
	//bsp_UART_Test(5);
	//HAL_UART_Receive_IT(&huart6,&rd,1);  //중요! CallBack 함수 Start! !"rd"는 전역변수 사용!!!
	//bsp_UART_Test(6);
	//HAL_UART_Receive_IT(&huart7,&rd,1);  //중요! CallBack 함수 Start! !"rd"는 전역변수 사용!!!
	//bsp_UART_Test(7);
	//HAL_UART_Receive_IT(&huart8,&rd,1);  //중요! CallBack 함수 Start! !"rd"는 전역변수 사용!!!
	//bsp_UART_Test(8);

}



//-----------------------------------------------------------------------------------
void bsp_init(void)
{
  /* Peripheral Configuration */ 
	//HAL_IWDG_Refresh(&hiwdg);
	bsp_ADC_start();
	//bsp_COMP_start();
	bsp_DAC_start();
	bsp_TIM_start();
  bsp_UART_start();

	//HAL_Delay(200); ///////////////// Delay
/* // RANDOM
  RngHandle.Instance = RNG;
  if(HAL_RNG_Init(&RngHandle) != HAL_OK)
    // Initialization Error
    Error_Handler();
  }
	aRandom32bit[counter] = HAL_RNG_GetRandomNumber(&RngHandle);
*/	
	// C Function
	//srand((uint16_t)an.adc2_buf[1]);
	//kprintf(&kpro1, (cu8 *)"%d,", rand());

}



