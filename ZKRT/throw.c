#include "throw.h"
#include "zkrt.h"	//������Ҫ
#include "can.h"	//����CAN���ֽ�

uint8_t throw_ack_flag;
uint8_t can_value;

//����ģ���У�����ֽڵ��жϽ��յ����ݣ���zkrtָ������Ȼ�������õľ��������˷�����Ӧ���Ƕ�ȡ�ľͶ�ȡ���˷���ֵ��������װ��zkrt�����ٷ���
zkrt_packet_t sub_throw_zkrt_packet_can1_rx;
void sub_throw_zkrt_recv_decode_and_zkrt_encode_ack(void)
{
	while (CAN1_rx_check() == 1)
	{
		can_value = CAN1_rx_byte();
		if (zkrt_decode_char(&sub_throw_zkrt_packet_can1_rx,can_value)==1)//ÿ���ַ����ж�һ�£������յ�����������ʱ����1
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
							throw_ack_flag = 1;  //��ʾ��Ͷ1��									
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
							throw_ack_flag = 3; //��ʾ��Ͷ1��2��
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
							throw_ack_flag = 7; //��ʾ��Ͷ1��2��3��
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


