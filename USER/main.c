#include "sys.h"
#include "led.h"
#include "zkrt.h"
#include "adc.h"
#include "pwm.h"
#include "can.h"
#include "throw.h"
#include "key.h"

/*
ͷ�ļ�˳��
sys.h
led.h
zkrt.h
adc.h
pwm.h

can.h������zkrt.h
throw������can.h��zkrt.h��pwm.h
*/

void bsp_init(void)
{
	SystemInit ();		/*ϵͳ��ʼ��*/
	RCC_Configuration();
	SysTick_Init();
//	TIM_Init();					//����ͨ����ʼ��
	LED_Init();					//5Vʹ�ܣ�����֮ǰPWM���Ѿ�Ĭ���޶�����
	ADC1_Init();
	CAN_Mode_Init(CAN_Mode_Normal);//CAN��ʼ������ģʽ,������1Mbps
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
		if (_100ms_count - TimingDelay >= 100)							//100msһ��ʱ��Ƭ
		{
			_100ms_count = TimingDelay;
			if ((_100ms_flag%1) == 0)													//ÿ100msһ�Σ���ȡ���ص�ѹ
			{
				ADC_StartOfConversion(ADC1);
			}
			
			if ((_100ms_flag%10) == 0)												//ÿ1000msһ�Σ����ϵ�ѹ������ѹ����������
			{
				bat_read();
				
				if (MAVLINK_TX_INIT_VAL - TimingDelay > 4000)		//��ʼ����4S�ڲ�ִ�м�飬�Ժ�ÿ�ζ�ȡ���󶼼��
				{
					bat_check();
				}
				
				if (MAVLINK_TX_INIT_VAL - TimingDelay > 5000)		//��ʼ����5S�ڲ�ִ�з����������Ժ�ÿ�ζ���������
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
//						throw_ack_flag = 1;  //��ʾ��Ͷ1��
//					}
//					else if (GET_PWM1 == _2_THROW_UNLOCK)
//					{
//						throw_ack_flag = 3; //��ʾ��Ͷ1��2��
//					}
//					else if (GET_PWM1 == _3_THROW_UNLOCK)
//					{
//						throw_ack_flag = 7; //��ʾ��Ͷ1��2��3��
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
					Can_Send_Msg(status_throw, 8);								//����򵥵���䣬���ڵ�����Ӧ
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



