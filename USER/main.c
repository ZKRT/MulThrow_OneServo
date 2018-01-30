#include "sys.h"
#include "led.h"
#include "zkrt.h"
#include "adc.h"
#include "pwm.h"
#include "can.h"
#include "throw.h"
#include "key.h"

/*
头文件顺序
sys.h
led.h
zkrt.h
adc.h
pwm.h

can.h包含了zkrt.h
throw包含了can.h、zkrt.h、pwm.h
*/

void bsp_init(void)
{
	SystemInit ();		/*系统初始化*/
	RCC_Configuration();
	SysTick_Init();
//	TIM_Init();					//挨个通道初始化
	LED_Init();					//5V使能，在这之前PWM波已经默认无动作了
	ADC1_Init();
	CAN_Mode_Init(CAN_Mode_Normal);//CAN初始化环回模式,波特率1Mbps
	KEY_Init();
}


uint8_t status_throw[8] = {0XAA, 0XBB, 0XCC, 0XDD, 0XEE, 0X06, 0X00, 0X00};

uint8_t fire_warning;
uint8_t fire_alarm;
uint8_t fire_status;
#define setBit(x,b)         (x) |=  (1U<<(b))
#define clearBit(x,b)		    (x) &= ~(1U<<(b))
int main()
{
  bsp_init();
	
	while (1)
	{
		sub_throw_zkrt_recv_decode_and_zkrt_encode_ack();
		KEY_Rock();
//		fire_warning = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5);
//		fire_alarm = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15);
		fire_warning = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9);		
		if (_100ms_count - TimingDelay >= 100)							//100ms一个时间片
		{
			_100ms_count = TimingDelay;
			if ((_100ms_flag%1) == 0)													//每100ms一次，读取板载电压
			{
				ADC_StartOfConversion(ADC1);
			}
			
			if ((_100ms_flag%10) == 0)												//每1000ms一次，整合电压、检测电压、发送心跳
			{
				bat_read();
				
				if (MAVLINK_TX_INIT_VAL - TimingDelay > 4000)		//初始化的4S内不执行检查，以后每次读取到后都检查
				{
					bat_check();
				}
				
				if (MAVLINK_TX_INIT_VAL - TimingDelay > 5000)		//初始化的5S内不执行发送心跳，以后每次都发送心跳
				{
					if(fire_warning)//normal
					{
						clearBit(fire_status, 0);
					}
					else
					{
						setBit(fire_status, 0);
					}
					if(fire_alarm)
					{
						clearBit(fire_status, 1);
					}			
					else
					{
						setBit(fire_status, 1);
					}
//					if (GET_PWM1 == _1_THROW_UNLOCK)
//					{
//						throw_ack_flag = 1;  //表示抛投1开
//					}
//					else if (GET_PWM1 == _2_THROW_UNLOCK)
//					{
//						throw_ack_flag = 3; //表示抛投1、2开
//					}
//					else if (GET_PWM1 == _3_THROW_UNLOCK)
//					{
//						throw_ack_flag = 7; //表示抛投1、2、3开
//					}
//					else
//					{
//						throw_ack_flag = 0;
//						THROW_PWM1(_ALL_THROW_LOCK);
//					}
					
					status_throw[6] = fire_status;
					status_throw[7]++;
					if (status_throw[7] == 0XFF)
					{
						status_throw[7] = 0;
					}
					Can_Send_Msg(status_throw, 8);								//这个简单的语句，便于调试响应
				}
			}
			
			_100ms_flag++;
		}
		
		if (led_rx_count - TimingDelay > 50)
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_7);
		}
		
		if (led_tx_count - TimingDelay > 50)
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_6);
		}
	}
}



