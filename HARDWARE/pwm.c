#include "pwm.h"
#include "throw.h"

//PWM1_T3C2����ӦPB5
//PWM2_T3C1����ӦPB4
//PWM3_T2C1����ӦPA15
//PWM4_T3C4����ӦPB1
//PWM5_T3C3����ӦPB0
//PWM6_T2C4����ӦPA3
//PWM7_T2C3����ӦPA2
//PWM8_T2C2����ӦPA1
//PA����ΪAF2������TIM2��PB����ΪAF1������TIM3
 /**
  * @brief  ����TIM3�������PWMʱ�õ���I/O
  * @param  ��
  * @retval ��
  */
void TIM_Init(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

  /* GPIOB Clocks enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);	
	
	//����PWM
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2 | GPIO_Pin_1;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_2);
	
  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  
  /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 48-1; 
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 20000-1;											//(48*20000/48)us=20ms����ʱ����װ��ʱ��Ϊ20ms
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;							
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;		
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;			
  TIM_OCInitStructure.TIM_Pulse = _ALL_THROW_LOCK;										
	
//	TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //modify by yanly
//	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);

	TIM_Cmd(TIM2, ENABLE);
}



