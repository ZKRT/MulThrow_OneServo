/**
  ******************************************************************************
  * @file    appcan.c 
  * @author  ZKRT
  * @version V1.0
  * @date    9-May-2017
  * @brief   can应用数据通信
	*					 + (1) init
	*                       
  ******************************************************************************
  * @attention
  *
  * ...
  *
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "can.h"
#include "appcan.h"
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
msg_handle_st hb_msg;
common_data_plst* commondata;
common_hbd_plst* commonhb;
throw_hbccplst* throwchb;
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  appcan_init
  * @param  None
  * @retval None
  */
void appcan_init(void)
{
//heartbeat fixed pack init	
	commondata = (common_data_plst*)hb_msg.packet.data;
	commonhb = (common_hbd_plst*)commondata->type_data;
	throwchb = (throw_hbccplst*)commonhb->hb_data;
	
	zkrt_init_packet(&hb_msg.packet);
	hb_msg.packet.cmd = SUBDEV_TO_UAV;
	hb_msg.packet.command = ZK_COMMAND_COMMON;
	hb_msg.packet.UAVID[ZK_DINDEX_DEVTYPE] = DEVICE_TYPE_SELF;
	commondata->type_num = TN_HEARTBEAT;
	commonhb->hb_flag = TNHB_FLAG;	
	hb_msg.packet.length = THHB_FIXED_LEN+sizeof(throw_hbccplst);
}
///**
//  * @brief  
//  * @param  None
//  * @retval None
//  */
//void appcan_prcs(void)
//{
//}
/**
  * @brief  appcan_hbpacket_send  心跳包封装
  * @param  None
  * @retval None
  */
uint8_t appcan_hbpacket_send(void)
{
	hb_msg.datalen = zkrt_final_encode(hb_msg.data, &hb_msg.packet);
	CAN1_send_message_fun(hb_msg.data, hb_msg.datalen);
	return 0;
}
/**
  * @brief   
  * @param  None
  * @retval None
  */
u8 zkrt_decode(recv_zkrt_packet_handlest *rh)
{
	uint8_t can_value;
	while(CAN1_rx_check() == 1)
	{
		can_value = CAN1_rx_byte();
		if (zkrt_decode_char_v2(rh, can_value)==1)
		{
			return 1;
		}
	}
	return 0;
}
///**
//  * @brief  app_can_recv_handle 
//  * @param  None
//  * @retval None
//  */
//static void app_can_recv_handle(void)
//{
//	zkrt_packet_t *packet = &msg_handlest.recvpacket;
//	zkrt_packet_t *packet2 = &msg_handlest.sendpacket;
//	u8 ret = NOTNEED_RETRUN;
////receive data	
//	if(zkrt_decode(&msg_handlest.recvpacket)==0)
//		return;
////check cmd
//	if((packet->cmd!=APP_TO_UAV)&&(packet->cmd!=UAV_TO_SUBDEV)&&(packet->cmd!=APP_TO_UAV_SUPER))
//		return;
////check command
//	if(packet->command>ZK_COMMAND_MAX)
//		return;
////check uavid
//  if(packet->UAVID[3]!=DEVICE_TYPE_SELF)
//		return;
////check data control num
//	if((packet->data[CTRLNUM_INDEX] >=CN_MAX)||(packet->data[CTRLNUM_INDEX] <=CN_None))
//		return;
////check data
//	switch(packet->command)
//	{
//		case ZK_COMMAND_NORMAL:
//			
//			break;
//		
//		case ZK_COMMAND_SPECIFIED:
////			printf("ptcol_fun\n");
//			ret = ptcol_fun[packet->data[CTRLNUM_INDEX]](packet->data, packet2->data, packet->length, &packet2->length);
//			break;
//		
//		case ZK_COMMAND_COMMON:
//			break;
//		
//		default:break;
//	}
////respond data
//	if(ret==NEED_RETRUN)
//	{
//		packet2->cmd = packet->cmd+1;
//		packet2->command = packet->command;
//		msg_handlest.datalen = zkrt_final_encode(msg_handlest.data, packet2);
//#ifdef UART_TEST_PROTOCOL
//		t_osscomm_sendMessage(msg_handlest.data, msg_handlest.datalen, USART_TEST_NUM);
//#else
//		ret = CAN1_send_message_fun(msg_handlest.data, msg_handlest.datalen);
//#endif
//	}
////clear recv packet
//  memset(packet, 0, sizeof(zkrt_packet_t));
//}

