#ifndef _THROW_H
#define _THROW_H

#include "sys.h"

////��һ��
//#define _1_THROW_UNLOCK 1950
//#define _1_THROW_LOCK   1500
////�ڶ���
//#define _2_THROW_UNLOCK	1000
//#define _2_THROW_LOCK   1500
////������
//#define _3_THROW_UNLOCK 2000
//#define _3_THROW_LOCK   1500

//#define THROW_PWM1(value)       TIM_SetCompare2(TIM2, value)
//#define THROW_PWM2(value)       TIM_SetCompare3(TIM2, value)
//#define THROW_PWM3(value)       TIM_SetCompare4(TIM2, value)
//
//#define GET_PWM1								TIM_GetCapture2(TIM2)
//#define GET_PWM2								TIM_GetCapture3(TIM2)
//#define GET_PWM3								TIM_GetCapture4(TIM2)

/////////////////////////////////////////////modify for ���׵����
////ȫ��
//#define _ALL_THROW_LOCK 1290
////��һ��
//#define _1_THROW_UNLOCK 1420
//#define _1_THROW_LOCK   _ALL_THROW_LOCK
////�ڶ���
//#define _2_THROW_UNLOCK	1530
//#define _2_THROW_LOCK   _ALL_THROW_LOCK
////������
//#define _3_THROW_UNLOCK 1600
//#define _3_THROW_LOCK   _ALL_THROW_LOCK

//���ݽṹ��ƺ͵��Ծ��飬unlockֵ��lockֵС��all_lock<1unlock<2unlock<3unlock
//ȫ��
#define _ALL_THROW_LOCK 1330   //ֵԽС������Խ��
//��һ��
#define _1_THROW_UNLOCK 1505   //��ֵ175
#define _1_THROW_LOCK   _ALL_THROW_LOCK
//�ڶ���
#define _2_THROW_UNLOCK	1625   //��ֵ120
#define _2_THROW_LOCK   _ALL_THROW_LOCK
//������
#define _3_THROW_UNLOCK 1720   //��ֵ95
#define _3_THROW_LOCK   _ALL_THROW_LOCK


#define THROW_PWM1(value)       TIM_SetCompare2(TIM2, value)
#define THROW_PWM2(value)       THROW_PWM1(value)
#define THROW_PWM3(value)       THROW_PWM1(value)

#define GET_PWM1								TIM_GetCapture2(TIM2)
#define GET_PWM2								GET_PWM1
#define GET_PWM3								GET_PWM1

extern uint8_t throw_ack_flag;
void sub_throw_zkrt_recv_decode_and_zkrt_encode_ack(void);

#endif

