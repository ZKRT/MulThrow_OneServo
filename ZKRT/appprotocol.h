/**
  ******************************************************************************
  * @file    appprotocol.h
  * @author  ZKRT
  * @version V1.0
  * @date    9-May-2017
  * @brief
	*					 + (1) init
  ******************************************************************************
  * @attention
  *
  * ...
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APPPROTOCOL_H
#define __APPPROTOCOL_H
/* Includes ------------------------------------------------------------------*/
#include "zkrt.h"
/* Exported macro ------------------------------------------------------------*/

//version and model string //zkrt_notice
#define DEV_MODEL              "M-PT"
#define DEV_HW                 "000400"
#define DEV_SW                 "000303"

/* Exported constants --------------------------------------------------------*/
//type num enum
typedef enum {
	TN_None = 0,
	TN_HEARTBEAT,
	TN_GETDEVINFO,
	TN_MAX
} TypeNumEnum;
//hb flag
#define TNHB_FLAG                     0xaabbccdd
#define THHB_FIXED_LEN                5
/* Exported types ------------------------------------------------------------*/
#pragma pack(1)
///////////////////////////////subdev to uav zkrt data struct
//////common command
typedef struct {
	u8 type_num;
	u8 type_data[ZK_DATA_MAX_LEN - 1];
} common_data_plst;
//heartbeat data
typedef struct {
	u32 hb_flag;
	u8 hb_data[ZK_DATA_MAX_LEN - THHB_FIXED_LEN];
} common_hbd_plst;
//dev info data
typedef struct {
	u8 status;
	u8 model[10];
	u8 hw_version[6];
	u8 sw_version[6];
} common_get_devinfo_plst;
//throw heartbeat
typedef struct {
//	hb_header_ccplst hbh;
	u8 v1;
	u8 v2;
	u8 v3;
} throw_hbccplst;
////irradiate heartbeat
//typedef struct
//{
////	hb_header_ccplst hbh;
//	u8 value;
//}irradiate_hbccplst;
////3Dmodeling heartbeat
//typedef irradiate_hbccplst threemodel_hbccplst;
////gas heartbeat
//typedef struct
//{
////	hb_header_ccplst hbh;
//	u8 num;
//	u8 status;
//	u8 gas1;
//	u16 gas1v;
//	u8 gas2;
//	u16 gas2v;
//	u8 gas3;
//	u16 gas3v;
//	u8 gas4;
//	u16 gas4v;
//	u8 gas5;
//	u16 gas5v;
//	u8 gas6;
//	u16 gas6v;
//	u8 gas7;
//	u16 gas7v;
//	u8 gas8;
//	u16 gas8v;
//}gas_hbccplst;
///////////////////////////////
#pragma pack()
/* Exported functions ------------------------------------------------------- */
#endif /* __APPCAN_H */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT ZKRT *****END OF FILE****/

