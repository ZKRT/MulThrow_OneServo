#include "sys.h"
#include "led.h"
#include "zkrt.h"
#include "adc.h"
#include "pwm.h"
#include "can.h"
#include "throw.h"
#include "key.h"
#include "flash.h"
#include "appcan.h"

void bsp_init(void) {
	SysTick_Init();
	STMFLASH_Init();
	TIM_Init();					//����ͨ����ʼ��
	LED_Init();					//5Vʹ�ܣ�����֮ǰPWM���Ѿ�Ĭ���޶�����
	ADC1_Init();
	CAN_Mode_Init(CAN_Mode_Normal);//CAN��ʼ������ģʽ,������1Mbps
	KEY_Init();
}
int main() {
	bsp_init();
	appcan_init();
	while (1) {
		appcan_prcs();
		KEY_Rock();
		stmflash_process();
		if (_100ms_count - TimingDelay >= 100) {						//100msһ��ʱ��Ƭ
			_100ms_count = TimingDelay;
			if ((_100ms_flag % 1) == 0) {												//ÿ100msһ�Σ���ȡ���ص�ѹ
				ADC_StartOfConversion(ADC1);
			}

			if ((_100ms_flag % 10) == 0) {											//ÿ1000msһ�Σ����ϵ�ѹ������ѹ����������
				bat_read();

				if (MAVLINK_TX_INIT_VAL - TimingDelay > 4000) {	//��ʼ����4S�ڲ�ִ�м�飬�Ժ�ÿ�ζ�ȡ���󶼼��
					bat_check();
				}

				if (MAVLINK_TX_INIT_VAL - TimingDelay > 5000) {	//��ʼ����5S�ڲ�ִ�з����������Ժ�ÿ�ζ���������
					check_throw_value();
					appcan_hbpacket_send();
				}
			}

			_100ms_flag++;
		}

		if (led_rx_count - TimingDelay > 50) {
			GPIO_SetBits(GPIOB, GPIO_Pin_7);
		}

		if (led_tx_count - TimingDelay > 50) {
			GPIO_SetBits(GPIOB, GPIO_Pin_6);
		}
	}
}
