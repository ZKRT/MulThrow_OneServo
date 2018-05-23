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
#include "throw.h"
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
msg_handle_st hb_msg;
common_data_plst* commondata;
common_hbd_plst* commonhb;
throw_hbccplst* throwchb;
//receive handle
recv_zkrt_packet_handlest recv_handle = {0};
zkrt_packet_t *recv_pcket             = &recv_handle.packet;
msg_handle_st recv_respond_msg;
/* Private functions ---------------------------------------------------------*/
static void app_can_recv_handle(void);
static u8 common_data_handle(const zkrt_packet_t *spacket, zkrt_packet_t *respond_packet);
static u8 normal_data_handle(const zkrt_packet_t *spacket);
/**
  * @brief  appcan_init
  * @param  None
  * @retval None
  */
void appcan_init(void) {
//heartbeat fixed pack init
	commondata                             = (common_data_plst*)hb_msg.packet.data;
	commonhb                               = (common_hbd_plst*)commondata->type_data;
	throwchb                               = (throw_hbccplst*)commonhb->hb_data;

	zkrt_init_packet(&hb_msg.packet);
	hb_msg.packet.cmd                      = SUBDEV_TO_UAV;
	hb_msg.packet.command                  = ZK_COMMAND_COMMON;
	hb_msg.packet.UAVID[ZK_DINDEX_DEVTYPE] = DEVICE_TYPE_SELF;
	commondata->type_num                   = TN_HEARTBEAT;
	commonhb->hb_flag                      = TNHB_FLAG;
	hb_msg.packet.length                   = THHB_FIXED_LEN + sizeof(throw_hbccplst);
}
/**
  * @brief
  * @param  None
  * @retval None
  */
void appcan_prcs(void) {
	app_can_recv_handle();
}
/**
  * @brief  appcan_hbpacket_send  心跳包封装
  * @param  None
  * @retval None
  */
uint8_t appcan_hbpacket_send(void) {
	hb_msg.datalen = zkrt_final_encode(hb_msg.data, &hb_msg.packet);
	CAN1_send_message_fun(hb_msg.data, hb_msg.datalen);
	return 0;
}
/**
  * @brief
  * @param  None
  * @retval None
  */
u8 zkrt_decode(recv_zkrt_packet_handlest *rh) {
	uint8_t can_value;
	while (CAN1_rx_check() == 1) {
		can_value = CAN1_rx_byte();
		if (zkrt_decode_char_v2(rh, can_value) == 1) {
			return 1;
		}
	}
	return 0;
}
/**
  * @brief  app_can_recv_handle
  * @param  None
  * @retval None
  */
static void app_can_recv_handle(void) {
	u8 ret = 0;
//receive data
	if (zkrt_decode(&recv_handle) == 0)
		return;
//check cmd
	if ((recv_pcket->cmd != APP_TO_UAV) && (recv_pcket->cmd != UAV_TO_SUBDEV) && (recv_pcket->cmd != APP_TO_UAV_SUPER))
		return;
//check command
	if (recv_pcket->command > ZK_COMMAND_MAX)
		return;
//check uavid
	if (recv_pcket->UAVID[3] != DEVICE_TYPE_SELF)
		return;
////check data control num
//	if((packet->data[CTRLNUM_INDEX] >=CN_MAX)||(packet->data[CTRLNUM_INDEX] <=CN_None))
//		return;
//check data
	switch (recv_pcket->command) {
	case ZK_COMMAND_NORMAL:
		normal_data_handle(recv_pcket);
		break;
	case ZK_COMMAND_SPECIFIED:
		break;

	case ZK_COMMAND_COMMON:
		memset(&recv_respond_msg.packet, 0, sizeof(zkrt_packet_t));
		ret = common_data_handle(recv_pcket, &recv_respond_msg.packet);
		break;
	default: break;
	}
//respond data
	if (ret == 1) {
		recv_respond_msg.datalen = zkrt_final_encode(recv_respond_msg.data, &recv_respond_msg.packet);
		ret = CAN1_send_message_fun(recv_respond_msg.data, recv_respond_msg.datalen);
	}
//clear recv packet
	memset(recv_pcket, 0, sizeof(zkrt_packet_t));
}
/**
  * @brief
  * @param  None
  * @retval None
  */
static u8 common_data_handle(const zkrt_packet_t *spacket, zkrt_packet_t *rpacket) {
	u8 res = 1;
	common_data_plst *scommon = (common_data_plst *)(spacket->data);
	common_data_plst *rcommon = (common_data_plst*)(rpacket->data);
	common_get_devinfo_plst *rother;

	memcpy(rpacket, spacket, ZK_HEADER_LEN);
	//differnet msg handle by type num
	switch (scommon->type_num) {
	case TN_GETDEVINFO:
		rother = (common_get_devinfo_plst *)(rpacket->data + 1);
		rother->status = 0;
		memcpy(rother->model, DEV_MODEL, sizeof(DEV_MODEL));
		memcpy(rother->hw_version, DEV_HW, 6);
		memcpy(rother->sw_version, DEV_SW, 6);
		rpacket->length = sizeof(common_get_devinfo_plst) + 1;
		break;
	default:
		res = 0;
		break;
	}

	//packet common handle
	if (res == 1) {
		rcommon->type_num = scommon->type_num;
		rpacket->cmd      = UAV_TO_APP;
		rpacket->end_code = _END_CODE;
	}
	return 1;
}
/**
  * @brief
  * @param  None
  * @retval None
  */
static u8 normal_data_handle(const zkrt_packet_t *spacket) {
	if ((spacket->data[1] == 0XEE) && (spacket->data[2] == 0XEE)) {
		if (spacket->data[0] == 1) {
			THROW_PWM1(_1_THROW_UNLOCK);
			THROW_PWM2(_2_THROW_LOCK);
			throw_ack_flag = 1;  //表示抛投1开
		} else if (spacket->data[0] == 0) {
			THROW_PWM1(_1_THROW_LOCK);
			THROW_PWM2(_2_THROW_UNLOCK);
			throw_ack_flag = 0;

		}
	} else if ((spacket->data[0] == 0XEE) && (spacket->data[2] == 0XEE)) {
		if (spacket->data[1] == 1) {
			THROW_PWM1(_1_THROW_UNLOCK);
			THROW_PWM2(_2_THROW_LOCK);
			throw_ack_flag = 3; //表示抛投1、2开
		} else if (spacket->data[1] == 0) {
			THROW_PWM1(_1_THROW_LOCK);
			THROW_PWM2(_2_THROW_UNLOCK);
			throw_ack_flag = 0;
		}
	} else if ((spacket->data[0] == 0XEE) && (spacket->data[1] == 0XEE)) {
		if (spacket->data[2] == 1) {
			THROW_PWM1(_1_THROW_UNLOCK);
			THROW_PWM2(_2_THROW_LOCK);
			throw_ack_flag = 7; //表示抛投1、2、3开
		} else if (spacket->data[2] == 0) {
			THROW_PWM1(_1_THROW_LOCK);
			THROW_PWM2(_2_THROW_UNLOCK);
			throw_ack_flag = 0;

		}
	}
	return 1;
}
