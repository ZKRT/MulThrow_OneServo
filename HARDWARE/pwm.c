#include "pwm.h"
#include "throw.h"
#include "flash.h"

//说明：此版本使用PWM8-PA1引脚，支持多抛吊舱模块V4.0

//PWM1_T3C2，对应PB5
//PWM2_T3C1，对应PB4
//PWM3_T2C1，对应PA15
//PWM4_T3C4，对应PB1
//PWM5_T3C3，对应PB0
//PWM6_T2C4，对应PA3
//PWM7_T2C3，对应PA2
//PWM8_T2C2，对应PA1
//PA复用为AF2，用于TIM2；PB复用为AF1，用于TIM3
/**
 * @brief  配置TIM3复用输出PWM时用到的I/O
 * @param  无
 * @retval 无
 */
void TIM_Init(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;

  /* GPIOB Clocks enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  //配置PWM
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
  TIM_TimeBaseStructure.TIM_Prescaler = 48 - 1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 20000 - 1;                   //(48*20000/48)us=20ms，定时器重装载时间为20ms
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_Pulse = throw_init_value;

//  TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //modify by yanly
//  TIM_OC3Init(TIM2, &TIM_OCInitStructure);
  TIM_OC2Init(TIM2, &TIM_OCInitStructure);

  TIM_Cmd(TIM2, ENABLE);
}




