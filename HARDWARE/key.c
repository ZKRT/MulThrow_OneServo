#include "key.h"
#include "throw.h"
#include "flash.h"

//按键初始化函数
void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//由于另一端接GND，那么这里初始化为高电平，作为空闲状态；按下时导通，GND拉低IO口形成低电平
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;//KEY1/KEY2
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;					//KEY3
  GPIO_Init(GPIOA, &GPIO_InitStructure);
} 
//按键处理函数
//返回按键值
uint8_t key_up=1;//KEY_UP=1代表着已经松开了
uint8_t Key_value1 = 1;
uint8_t Key_value2 = 1;
uint8_t Key_value3 = 1;
uint16_t key_in_test = 0;
uint8_t key_mode=1; //1-normal, 0-fine tune;
uint8_t fine_tune_flag = 0; //微调+-, 1+,2-,0none
uint8_t fine_turn_key_up=1;//KEY_UP=1代表着已经松开了
uint32_t fine_tune=0;
uint8_t key_pres_num=0;
uint8_t KEY_Scan(void)
{
	Key_value1 = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5);
	Key_value2 = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6);
	Key_value3 = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7);
	
	if((fine_turn_key_up == 1)&&(Key_value1 == 0)&&(Key_value2 == 0)&&(Key_value3 == 0))
	{
		fine_turn_key_up = 0;
		if(key_mode)
			key_mode =0;
		else
			key_mode =1;
		return KEY_DEFAULT;
	}
	else if((Key_value1 == 1)&&(Key_value2 == 1)&&(Key_value3 == 1))
	{
		fine_turn_key_up = 1;
	}	
		
	
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
uint8_t KEY_fine_tune_Scan(void)
{
	uint8_t _key_value;
	if(fine_tune_flag ==1)
	{
		_key_value = Key_value3;
	}
	else 	if(fine_tune_flag ==2)
	{
		_key_value = Key_value1;
	}	
	else
	{
		return 0;
	}
	
	if((fine_turn_key_up == 1)&&(_key_value == 0))//从松开到按下：返回相应键值，并且记录按下标志位。设置个时间用于防止多次触发，这里500ms
	{
		fine_turn_key_up = 0;
		return 1;
	}
	else if(_key_value == 0)						//从按下到松开：记录当前是松开的。不管前一次记录的状态，只要读取到的键值是全松开的，那就记录全松开
	{
		fine_turn_key_up = 1;
	}
	
	return 0;// 无按键按下
}
void KEY_fine_tune(void)
{
	if(Key_value1 == 0)
		key_pres_num++;
	if(Key_value2 == 0)
		key_pres_num++;
	if(Key_value3 == 0)
		key_pres_num++;
	
	if(key_pres_num >=2)
	{
		if((Key_value1 == 0)&&(Key_value2 == 0))
		{
			fine_tune_flag = 1;
		}
		else if((Key_value3 == 0)&&(Key_value2 == 0))
		{
			fine_tune_flag = 2;
		}
		else
		{
			fine_tune_flag = 0;
		}
	}
	else
	{
		fine_tune_flag =0;
	}
	key_pres_num = 0;
	
	if(KEY_fine_tune_Scan()) //微调触发
	{
		if(fine_tune_flag ==1)
		{
			throw_init_value+=100;
		}	
		if(fine_tune_flag ==2)
		{
			throw_init_value-=100;
		}
	}		
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
	
	if(key_mode)
	{
		switch(key_value)
		{
			case KEY1_PRES:
				if (GET_PWM1 == _1_THROW_LOCK(throw_init_value))
				{
					THROW_PWM1(_1_THROW_UNLOCK(throw_init_value));
				}
				else
				{
					THROW_PWM1(_1_THROW_LOCK(throw_init_value));
				}
				break;
			case KEY2_PRES:
				if (GET_PWM1 == _2_THROW_LOCK(throw_init_value))
				{
					THROW_PWM1(_2_THROW_UNLOCK(throw_init_value));
				}
				else
				{
					THROW_PWM1(_2_THROW_LOCK(throw_init_value));
				}
				break;
			case KEY3_PRES:
				if (GET_PWM1 == _3_THROW_LOCK(throw_init_value))
				{
					THROW_PWM1(_3_THROW_UNLOCK(throw_init_value));
				}
				else
				{
					THROW_PWM1(_3_THROW_LOCK(throw_init_value));
				}
				break;
		}		
	}
	else
	{
		switch(key_value)
		{
			case KEY1_PRES:
				throw_init_value+=10;
				if(throw_init_value >2000)
					throw_init_value = 2000;
				THROW_PWM1(_1_THROW_LOCK(throw_init_value));
				break;
			case KEY3_PRES:
				throw_init_value-=10;
				if(throw_init_value <1000)
					throw_init_value = 1000;
				THROW_PWM1(_1_THROW_LOCK(throw_init_value));
				break;
		}			
	}	

}
 

















