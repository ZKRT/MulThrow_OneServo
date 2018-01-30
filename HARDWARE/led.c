#include "led.h"

//PB3 5V使能位，高电平有效
//PB6状态指示灯ALARM
//PB7状态指示灯RUN
//PA812V电压使能，高电平有效
void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7| GPIO_Pin_3;//for led
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_3);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;//for 12V EN
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	
	
//PWM1_T3C2，对应PB5
//PWM2_T3C1，对应PB4
//PWM3_T2C1，对应PA15
//PWM4_T3C4，对应PB1
  
	//1输入上拉
	//2输出低电平
	//3输入上拉
	//4输出低电平
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	GPIO_ResetBits(GPIOB, GPIO_Pin_1|GPIO_Pin_4);
//	
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);	


	//pa9输入上拉
	//pa10输出低电平
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_10);
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	delay_ms(500);
}
