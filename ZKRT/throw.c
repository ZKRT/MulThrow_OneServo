#include "throw.h"
#include "zkrt.h"	//调用需要
#include "can.h"	//解析CAN的字节

uint8_t throw_ack_flag;
uint8_t can_value;

//在子模块中，逐个字节的判断接收的数据，对zkrt指令解包，然后是设置的就设置完了返回响应，是读取的就读取完了返回值，将它封装成zkrt包，再返回
zkrt_packet_t sub_throw_zkrt_packet_can1_rx;
void sub_throw_zkrt_recv_decode_and_zkrt_encode_ack(void)
{
	while (CAN1_rx_check() == 1)
	{
		can_value = CAN1_rx_byte();
		if (zkrt_decode_char(&sub_throw_zkrt_packet_can1_rx,can_value)==1)//每个字符都判断一下，当接收到完整的数据时返回1
		{
			switch (sub_throw_zkrt_packet_can1_rx.command)
			{
				case DEFAULT_NUM:          
				{
					if ((sub_throw_zkrt_packet_can1_rx.data[1] == 0XEE)&&(sub_throw_zkrt_packet_can1_rx.data[2] == 0XEE))
					{
						if (sub_throw_zkrt_packet_can1_rx.data[0] == 1)
						{
							THROW_PWM1(_1_THROW_UNLOCK);
							throw_ack_flag = 1;  //表示抛投1开									
						}
						else if (sub_throw_zkrt_packet_can1_rx.data[0] == 0)
						{
							THROW_PWM1(_1_THROW_LOCK);
							throw_ack_flag = 0;
						}
					}
					else if ((sub_throw_zkrt_packet_can1_rx.data[0] == 0XEE)&&(sub_throw_zkrt_packet_can1_rx.data[2] == 0XEE))
					{
						if (sub_throw_zkrt_packet_can1_rx.data[1] == 1)				
						{
							THROW_PWM1(_2_THROW_UNLOCK);
							throw_ack_flag = 3; //表示抛投1、2开
						}
						else if (sub_throw_zkrt_packet_can1_rx.data[1] == 0)
						{
							THROW_PWM1(_2_THROW_LOCK);
							throw_ack_flag = 0;
						}
					}
					else if ((sub_throw_zkrt_packet_can1_rx.data[0] == 0XEE)&&(sub_throw_zkrt_packet_can1_rx.data[1] == 0XEE))
					{
						if (sub_throw_zkrt_packet_can1_rx.data[2] == 1)
						{
							THROW_PWM1(_3_THROW_UNLOCK);
							throw_ack_flag = 7; //表示抛投1、2、3开
						}
						else if (sub_throw_zkrt_packet_can1_rx.data[2] == 0)
						{
							THROW_PWM1(_3_THROW_LOCK);
							throw_ack_flag = 0;
						}
					}
					
					break;
				}
				default:
				{
					break;
				}
			}
		}
	}
}


