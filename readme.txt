程序基于SMD交付的多抛程序。
修改功能：
1. 将多抛改成单舵机控制3个抛投开关，结构上做优化了。
2. 使用第一个PWM,即PWM1(#define THROW_PWM1(value)       TIM_SetCompare2(TIM2, value))
3. 按键和软件控制抛投：
抛投1 --- PWM值》
抛投2 --- PWM值》
抛投3 --- PWM值》
全关  --- PWM值》
4. 屏蔽ADC检测5VA，5VB；
5. 加大ADC 5VC 检测值，5800最大值（因为力量一大就过压，需要查看舵机资料再看）；
20170214:
修改抛投PWM值，适配结构；
修改多抛反馈值错误的bug；
