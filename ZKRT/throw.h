#ifndef _THROW_H
#define _THROW_H

#include "sys.h"

//���ݽṹ��ƺ͵��Ծ��飬unlockֵ��lockֵС��all_lock<1unlock<2unlock<3unlock
//ȫ��

//zkrt_debug 170322
#define _ALL_THROW_LOCK 1000   //ֵԽС������Խ��     num2  old:1340
//��һ��
#define _1_THROW_UNLOCK 1180   //��ֵ275										old:1480
#define _1_THROW_LOCK   _ALL_THROW_LOCK
//�ڶ���
#define _2_THROW_UNLOCK	1350   //��ֵ												old:1575
#define _2_THROW_LOCK   _ALL_THROW_LOCK
//������
#define _3_THROW_UNLOCK 1520   //��ֵ												old:1665
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

