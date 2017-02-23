#include "key.h"
#include "throw.h"

//������ʼ������
void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//������һ�˽�GND����ô�����ʼ��Ϊ�ߵ�ƽ����Ϊ����״̬������ʱ��ͨ��GND����IO���γɵ͵�ƽ
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_3;//KEY1/KEY2
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;					//KEY3
  GPIO_Init(GPIOA, &GPIO_InitStructure);
} 
//������������
//���ذ���ֵ
uint8_t key_up=1;//KEY_UP=1�������Ѿ��ɿ���
uint8_t Key_value1 = 1;
uint8_t Key_value2 = 1;
uint8_t Key_value3 = 1;
uint16_t key_in_test = 0;
uint8_t KEY_Scan(void)
{
	Key_value1 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4);
	Key_value2 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3);
	Key_value3 = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15);
	
	if((key_up == 1)&&((Key_value1 == 0)||(Key_value2 == 0)||(Key_value3 ==0))&&(_key_count - TimingDelay > 500))//���ɿ������£�������Ӧ��ֵ�����Ҽ�¼���±�־λ�����ø�ʱ�����ڷ�ֹ��δ���������500ms
	{
		_key_count = TimingDelay;
		key_up=0;
		if(Key_value1==0)        
		{
			key_in_test++;			//������Է��֣�ż���ԵĻ����2�Σ�������������Ҫ���ø�ʱ��α�������������ᱻ��������
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
	else if((Key_value1 == 1)&&(Key_value2 == 1)&&(Key_value3 == 1))						//�Ӱ��µ��ɿ�����¼��ǰ���ɿ��ġ�����ǰһ�μ�¼��״̬��ֻҪ��ȡ���ļ�ֵ��ȫ�ɿ��ģ��Ǿͼ�¼ȫ�ɿ�
	{
		key_up = 1;
	}
	
	return KEY_DEFAULT;// �ް�������
}

//�԰�����������ѯ����
uint8_t key_value=0;
void KEY_Rock(void)
{
	key_value=KEY_Scan();		//�õ���ֵ
	
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
 
















