#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 

#define KEY_DEFAULT 0
#define KEY1_PRES  1
#define KEY2_PRES	 2
#define KEY3_PRES	 3

void KEY_Init(void);
uint8_t KEY_Scan(void);
void KEY_Rock(void);

#endif
