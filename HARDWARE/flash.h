#ifndef __FLASH_H__
#define __FLASH_H__
 
 #include "sys.h"
 
//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 								//STM32 FLASH����ʼ��ַ������0����ʼ��ַ
#define STM32_FLASH_END  0x08007FFF									//STM32 FLASH�Ľ�����ַ������7�Ľ�����ַ


//FLASH ��������ʼ��ַ
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) 	//����0��ʼ��ַ, 4 Kbytes  
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08001000) 	//����1��ʼ��ַ, 4 Kbytes
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08002000) 	//����2��ʼ��ַ, 4 Kbytes
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x08003000) 	//����3��ʼ��ַ, 4 Kbytes
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08004000) 	//����4��ʼ��ַ, 4 Kbytes
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08005000) 	//����5��ʼ��ַ, 4 Kbytes
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08006000) 	//����6��ʼ��ַ, 4 Kbytes
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08007000) 	//����7��ʼ��ַ, 4 Kbytes

#define MY_SETTING_PAGE_ADDR    ADDR_FLASH_SECTOR_7
//#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) 	//����8��ʼ��ַ, 128 Kbytes  
//#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) 	//����9��ʼ��ַ, 128 Kbytes  
//#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) 	//����10��ʼ��ַ,128 Kbytes  
//#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) 	//����11��ʼ��ַ,128 Kbytes  

//flash sector size
#define SIZE_FLASH_SECTOR_0     ((uint32_t)(ADDR_FLASH_SECTOR_1-ADDR_FLASH_SECTOR_0)
#define SIZE_FLASH_SECTOR_1     ((uint32_t)ADDR_FLASH_SECTOR_2-ADDR_FLASH_SECTOR_1)
#define SIZE_FLASH_SECTOR_2     ((uint32_t)ADDR_FLASH_SECTOR_3-ADDR_FLASH_SECTOR_2)
#define SIZE_FLASH_SECTOR_3     ((uint32_t)ADDR_FLASH_SECTOR_4-ADDR_FLASH_SECTOR_3)
#define SIZE_FLASH_SECTOR_4     ((uint32_t)ADDR_FLASH_SECTOR_5-ADDR_FLASH_SECTOR_4)
#define SIZE_FLASH_SECTOR_5     ((uint32_t)ADDR_FLASH_SECTOR_6-ADDR_FLASH_SECTOR_5)
#define SIZE_FLASH_SECTOR_6     ((uint32_t)ADDR_FLASH_SECTOR_7-ADDR_FLASH_SECTOR_6)
#define SIZE_FLASH_SECTOR_7     ((uint32_t)ADDR_FLASH_SECTOR_8-ADDR_FLASH_SECTOR_7)
//#define SIZE_FLASH_SECTOR_8     ((uint32_t)ADDR_FLASH_SECTOR_9-ADDR_FLASH_SECTOR_8)
//#define SIZE_FLASH_SECTOR_9     ((uint32_t)ADDR_FLASH_SECTOR_10-ADDR_FLASH_SECTOR_9)
//#define SIZE_FLASH_SECTOR_10    ((uint32_t)ADDR_FLASH_SECTOR_11-ADDR_FLASH_SECTOR_10)
//#define SIZE_FLASH_SECTOR_11    SIZE_FLASH_SECTOR_10

#pragma pack(push, 1)
typedef struct _flash_type	
{
	unsigned int _start_cod;
	unsigned int throw_init_value;  //�����ֵ
	unsigned int _end_cod;
}flash_type;
#pragma pack(pop)

extern flash_type flash_buffer;
extern uint32_t throw_init_value;

#define FLASH_USE_NUM    (sizeof(flash_buffer)/4 + ((sizeof(flash_buffer)%4 == 0)?0:1))

unsigned int STMFLASH_ReadWord(unsigned int faddr);		  	//������  
void STMFLASH_Write(void);		//��ָ����ַ��ʼд��ָ�����ȵ�����
void STMFLASH_Read(void);   		//��ָ����ַ��ʼ����ָ�����ȵ�����
void STMFLASH_Init(void);
void STMFLASH_Erase(short sector);			
void stmflash_process(void);
#endif


