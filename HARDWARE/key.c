#include "key.h"
#include "throw.h"

//按键初始化函数
void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//由于另一端接GND，那么这里初始化为高电平，作为空闲状态；按下时导通，GND拉低IO口形成低电平
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_3;//KEY1/KEY2
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;					//KEY3
  GPIO_Init(GPIOA, &GPIO_InitStructure);
} 
//按键处理函数
//返回按键值
uint8_t key_up=1;//KEY_UP=1代表着已经松开了
uint8_t Key_value1 = 1;
uint8_t Key_value2 = 1;
uint8_t Key_value3 = 1;
uint16_t key_in_test = 0;
uint8_t KEY_Scan(void)
{
	Key_value1 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4);
	Key_value2 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3);
	Key_value3 = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15);
	
	if((key_up == 1)&&((Key_value1 == 0)||(Key_value2 == 0)||(Key_value3 ==0))&&(_key_count - TimingDelay > 500))//从松开到按下：返回相应键值，并且记录按下标志位。设置个时间用于防止多次触发，这里500ms
	{
		_key_count = TimingDelay;
		key_up=0;
		if(Key_value1==0)        
		{
			key_in_test++;			//这里测试发现，偶发性的会进入2次，所以在外面需要设置个时间段保护这个操作不会被连续调用
			return KEY1_PRES;
		}
		else if(Key_value2 == 0)
		{
			return KEY2_PRES;
		}
		else if(Key_value3 == 0)
		{
			return KEY3_PRES;
		}
	}
	else if((Key_value1 == 1)&&(Key_value2 == 1)&&(Key_value3 == 1))						//从按下到松开：记录当前是松开的。不管前一次记录的状态，只要读取到的键值是全松开的，那就记录全松开
	{
		key_up = 1;
	}
	
	return KEY_DEFAULT;// 无按键按下
}

//对按键操作的轮询动作
uint8_t key_value=0;
void KEY_Rock(void)
{
	key_value=KEY_Scan();		//得到键值
	
	if (key_value == KEY_DEFAULT)
	{
		return;
	}
	
	switch(key_value)
	{
		case KEY1_PRES:
			if (GET_PWM1 == _1_THROW_LOCK)
			{
				THROW_PWM1(_1_THROW_UNLOCK);
			}
			else
			{
				THROW_PWM1(_1_THROW_LOCK);
			}
			break;
		case KEY2_PRES:
			if (GET_PWM1 == _2_THROW_LOCK)
			{
				THROW_PWM1(_2_THROW_UNLOCK);
			}
			else
			{
				THROW_PWM1(_2_THROW_LOCK);
			}
			break;
		case KEY3_PRES:
			if (GET_PWM1 == _3_THROW_LOCK)
			{
				THROW_PWM1(_3_THROW_UNLOCK);
			}
			else
			{
				THROW_PWM1(_3_THROW_LOCK);
			}
			break;
	}
}
 

















