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
	TIM_Init();					//挨个通道初始化
	LED_Init();					//5V使能，在这之前PWM波已经默认无动作了
	ADC1_Init();
	CAN_Mode_Init(CAN_Mode_Normal);//CAN初始化环回模式,波特率1Mbps
	KEY_Init();
}
int main() {
	bsp_init();
	appcan_init();
	while (1) {
		appcan_prcs();
		KEY_Rock();
		stmflash_process();
		if (_100ms_count - TimingDelay >= 100) {						//100ms一个时间片
			_100ms_count = TimingDelay;
			if ((_100ms_flag % 1) == 0) {												//每100ms一次，读取板载电压
				ADC_StartOfConversion(ADC1);
			}

			if ((_100ms_flag % 10) == 0) {											//每1000ms一次，整合电压、检测电压、发送心跳
				bat_read();

				if (MAVLINK_TX_INIT_VAL - TimingDelay > 4000) {	//初始化的4S内不执行检查，以后每次读取到后都检查
					bat_check();
				}

				if (MAVLINK_TX_INIT_VAL - TimingDelay > 5000) {	//初始化的5S内不执行发送心跳，以后每次都发送心跳
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
