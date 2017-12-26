#include "throw.h"
#include "flash.h"
#include "appcan.h"

//uint8_t throw_ack_flag;

//在子模块中，逐个字节的判断接收的数据，对zkrt指令解包，然后是设置的就设置完了返回响应，是读取的就读取完了返回值，将它封装成zkrt包，再返回
recv_zkrt_packet_handlest recv_handle={0};
zkrt_packet_t *sub_throw_zkrt_packet_can1_rx = &recv_handle.packet;
void sub_throw_zkrt_recv_decode_and_zkrt_encode_ack(void)
{
	if(zkrt_decode(&recv_handle) ==0)
		return;
	
	switch (sub_throw_zkrt_packet_can1_rx->command)
	{
		case ZK_COMMAND_NORMAL:          
		{
			if ((sub_throw_zkrt_packet_can1_rx->data[1] == 0XEE)&&(sub_throw_zkrt_packet_can1_rx->data[2] == 0XEE))
			{
				if (sub_throw_zkrt_packet_can1_rx->data[0] == 1)
				{
					THROW_PWM1(_1_THROW_UNLOCK(throw_init_value));
//					throw_ack_flag = 1;  //表示抛投1开									
				}
				else if (sub_throw_zkrt_packet_can1_rx->data[0] == 0)
				{
					THROW_PWM1(_1_THROW_LOCK(throw_init_value));
//				throw_ack_flag = 0;
				}
			}
			else if ((sub_throw_zkrt_packet_can1_rx->data[0] == 0XEE)&&(sub_throw_zkrt_packet_can1_rx->data[2] == 0XEE))
			{
				if (sub_throw_zkrt_packet_can1_rx->data[1] == 1)				
				{
					THROW_PWM1(_2_THROW_UNLOCK(throw_init_value));
//					throw_ack_flag = 3; //表示抛投1、2开
				}
				else if (sub_throw_zkrt_packet_can1_rx->data[1] == 0)
				{
					THROW_PWM1(_2_THROW_LOCK(throw_init_value));
//					throw_ack_flag = 0;
				}
			}
			else if ((sub_throw_zkrt_packet_can1_rx->data[0] == 0XEE)&&(sub_throw_zkrt_packet_can1_rx->data[1] == 0XEE))
			{
				if (sub_throw_zkrt_packet_can1_rx->data[2] == 1)
				{
					THROW_PWM1(_3_THROW_UNLOCK(throw_init_value));
//					throw_ack_flag = 7; //表示抛投1、2、3开
				}
				else if (sub_throw_zkrt_packet_can1_rx->data[2] == 0)
				{
					THROW_PWM1(_3_THROW_LOCK(throw_init_value));
//					throw_ack_flag = 0;
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

void check_throw_value(void)
{
	if (GET_PWM1 == _1_THROW_UNLOCK(throw_init_value))
	{
	//						throw_ack_flag = 1;  //表示抛投1开
		throwchb->v1 = 1;
		throwchb->v2 = 0;
		throwchb->v3 = 0;
	}
	else if (GET_PWM1 == _2_THROW_UNLOCK(throw_init_value))
	{
	//						throw_ack_flag = 3; //表示抛投1、2开
		throwchb->v1 = 1;
		throwchb->v2 = 1;
		throwchb->v3 = 0;
	}
	else if (GET_PWM1 == _3_THROW_UNLOCK(throw_init_value))
	{
	//						throw_ack_flag = 7; //表示抛投1、2、3开
		throwchb->v1 = 1;
		throwchb->v2 = 1;
		throwchb->v3 = 1;
	}
	else
	{
	//						throw_ack_flag = 0;
		throwchb->v1 = 0;
		throwchb->v2 = 0;
		throwchb->v3 = 0;
		THROW_PWM1(throw_init_value);
	}
}
