#ifndef __ADC_H
#define __ADC_H
#include "sys.h" 

//#define _25V_VOL 0
//#define _5VA_VOL 1
//#define _5VB_VOL 2
//#define _5VC_VOL 3
//#define _5VA_IS  4
//#define _5VB_IS  5
//#define _5VC_IS  6

#define _5V_VOL   0
#define _12V_VOL  1


void ADC1_Init(void); 				//ADC通道初始化
uint16_t get_adc_val(uint8_t read_type);
void bat_read(void);
void bat_check(void);

#endif 

