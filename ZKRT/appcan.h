/**
  ******************************************************************************
  * @file    appcan.h 
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
#ifndef __APPCAN_H
#define __APPCAN_H 
/* Includes ------------------------------------------------------------------*/
#include "appprotocol.h"
/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void appcan_init(void);
//void appcan_prcs(void);
uint8_t appcan_hbpacket_send(void);
u8 zkrt_decode(recv_zkrt_packet_handlest *rh);

extern throw_hbccplst* throwchb;
#endif /* __APPCAN_H */
/**
  * @}
  */ 

/**
  * @}
  */
	
/************************ (C) COPYRIGHT ZKRT *****END OF FILE****/

