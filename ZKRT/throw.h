#ifndef _THROW_H
#define _THROW_H

#include "sys.h"

////第一个
//#define _1_THROW_UNLOCK 1950
//#define _1_THROW_LOCK   1500
////第二个
//#define _2_THROW_UNLOCK	1000
//#define _2_THROW_LOCK   1500
////第三个
//#define _3_THROW_UNLOCK 2000
//#define _3_THROW_LOCK   1500

//#define THROW_PWM1(value)       TIM_SetCompare2(TIM2, value)
//#define THROW_PWM2(value)       TIM_SetCompare3(TIM2, value)
//#define THROW_PWM3(value)       TIM_SetCompare4(TIM2, value)
//
//#define GET_PWM1								TIM_GetCapture2(TIM2)
//#define GET_PWM2								TIM_GetCapture3(TIM2)
//#define GET_PWM3								TIM_GetCapture4(TIM2)

/////////////////////////////////////////////modify for 多抛单舵机
////全关
//#define _ALL_THROW_LOCK 1290
////第一个
//#define _1_THROW_UNLOCK 1420
//#define _1_THROW_LOCK   _ALL_THROW_LOCK
////第二个
//#define _2_THROW_UNLOCK	1530
//#define _2_THROW_LOCK   _ALL_THROW_LOCK
////第三个
//#define _3_THROW_UNLOCK 1600
//#define _3_THROW_LOCK   _ALL_THROW_LOCK

//根据结构设计和调试经验，unlock值大，lock值小，all_lock<1unlock<2unlock<3unlock
//全关
#define _ALL_THROW_LOCK 1330   //值越小，锁的越多
//第一个
#define _1_THROW_UNLOCK 1505   //差值175
#define _1_THROW_LOCK   _ALL_THROW_LOCK
//第二个
#define _2_THROW_UNLOCK	1625   //差值120
#define _2_THROW_LOCK   _ALL_THROW_LOCK
//第三个
#define _3_THROW_UNLOCK 1720   //差值95
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

