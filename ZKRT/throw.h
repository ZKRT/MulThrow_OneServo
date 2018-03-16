#ifndef _THROW_H
#define _THROW_H

#include "sys.h"

//根据结构设计和调试经验，unlock值大，lock值小，all_lock<1unlock<2unlock<3unlock
//全关

//zkrt_debug 170322
#define _ALL_THROW_LOCK 0   //值越小，锁的越多     num2  old:1340
//第一个
#define _1_THROW_UNLOCK 20000   //差值275										old:1480
#define _1_THROW_LOCK   _ALL_THROW_LOCK
//第二个
#define _2_THROW_UNLOCK	20000   //差值												old:1575
#define _2_THROW_LOCK   _ALL_THROW_LOCK
//第三个
#define _3_THROW_UNLOCK 20000   //差值												old:1665
#define _3_THROW_LOCK   _ALL_THROW_LOCK


#define THROW_PWM1(value)       TIM_SetCompare2(TIM2, value)
#define THROW_PWM2(value)       TIM_SetCompare3(TIM2, value)
#define THROW_PWM3(value)       THROW_PWM1(value)

#define GET_PWM1								TIM_GetCapture2(TIM2)
#define GET_PWM2								GET_PWM1
#define GET_PWM3								GET_PWM1

extern uint8_t throw_ack_flag;
void sub_throw_zkrt_recv_decode_and_zkrt_encode_ack(void);
void check_throw_value(void);
#endif

