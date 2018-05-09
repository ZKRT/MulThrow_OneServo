#include "flash.h"
#include "throw.h"

/*
�������洢���⣬����3����ģ��洢����֯������3������ϵͳ�洢����30K����OTP����528�֣���ѡ���ֽڣ�16�֣�
ϵͳ�洢����ų���ʱ��BOOTLOADER
OTP��һ����д������������32��1�飬һ����16�飬����512�֣��������һ����д�������Ҳ��ɲ���������16������������Ӧ�顣ע��OTP�Ĳ�������ͨFLASH�Ķ�д����һ��
�û�ѡ���ֽڣ��������ö�������BOR�������/Ӳ�����Ź���λ�Լ��������ڴ�����ֹͣ״̬�µĸ�λ

׼ȷ������FLASH�ĵȴ���������LATENCY������ȡ����CPU��SYSCLK���ѹ��Χ���������3.3V���粢����168MHZ������֪6��CPU���ڣ�LATENCY=5

FLASH��̲��裺���BSYλ�����æ�ȴ�����PG=1��������PSIZE����д�롪���ȴ�BSYλ���㡪���˳�

�����������裺���LOCK�����û���������������BSYλ�����æ�ȴ�����SER��һ����������������SNBѡ���ĸ���������STRT��һ����ʼ���������ȴ�BSYλ���㡪���˳�
�����������裺�������SER��һ��SNBѡ�������滻��MER��һ�����಻��

�ⲿVPP��ʲô��
���洢������Ϊʲô�ճ���ô��һƬ��ַ�ռ�δ֪��
����������������Ͳ���Ҫ�����β鿴�ˣ�ֱ��������һ��������
����������ʹ���ǲ���Ӧ���ڱ��֮ǰ��Ҫ�����ˣ�
IAP����ͨ�����ⷽʽ����Ӳ�������硢GPRSԶ�������ȣ�Ч�ʺܸߡ����硢GPRS�������Ҫ�ع鵽ĳ������Ľӿڣ�������ʵһ����Ч����
*/

//��0��T1
//��1��T2

flash_type flash_buffer;
uint32_t throw_init_value;

//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ
//����ֵ:��Ӧ����.
unsigned int STMFLASH_ReadWord(unsigned int faddr) {
	uint32_t value;
	value = *(uint32_t*)faddr;
	return value;
}

//������7����д��
void STMFLASH_Write(void) {
	uint8_t i = 0;
	FLASH_Status status = FLASH_COMPLETE;
	uint32_t WriteAddr = MY_SETTING_PAGE_ADDR;
	uint32_t *flash_addr = (uint32_t *)(&flash_buffer._start_cod);

	FLASH_Unlock();																				//����
//  FLASH_DataCacheCmd(DISABLE);													//FLASH������д��ʱ���κγ��Զ�ȡ�Ĳ����ᵼ�����ߴ���������Ҫ��ȡ�����ݣ����Ļ��棬��д��Ĳ����������ֹ���ݻ��档����ûʲô�ã��ŵ����ﱸע��

	status = FLASH_ErasePage(MY_SETTING_PAGE_ADDR);

	if (status == FLASH_COMPLETE) {														//��������ɹ����ſ�ʼд��
		for (i = 0; i < FLASH_USE_NUM; i++) {
			if (FLASH_ProgramWord(WriteAddr,  *flash_addr) != FLASH_COMPLETE) {
				break;	//д���쳣
			}
			WriteAddr += 4;
			flash_addr++;
		}
	}

//  FLASH_DataCacheCmd(ENABLE);	//FLASH��������,�������ݻ���
	FLASH_Lock();//����
}

//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToRead:��(4λ)��
//void STMFLASH_Read(void)
//{
//	uint32_t i;
//	uint32_t ReadAddr = MY_SETTING_PAGE_ADDR;
//	uint32_t *flash_addr = (uint32_t *)(&flash_buffer._start_cod);
//
//	for(i = 0;i < FLASH_USE_NUM; i++)
//	{
//		*flash_addr = STMFLASH_ReadWord(ReadAddr);
//		ReadAddr += 4;
//		flash_addr++;
//	}
//}
uint32_t dddddvalue[15];
void STMFLASH_Read(void) {
	uint32_t i;
	uint32_t ReadAddr = MY_SETTING_PAGE_ADDR;
	uint32_t *flash_addr = (uint32_t *)(&flash_buffer._start_cod);


	for (i = 0; i < 1; i++) {
		dddddvalue[i] = STMFLASH_ReadWord(ReadAddr);
		ReadAddr += 4;
	}
	ReadAddr = MY_SETTING_PAGE_ADDR;
	for (i = 0; i < FLASH_USE_NUM; i++) {
		*flash_addr = STMFLASH_ReadWord(ReadAddr);
		ReadAddr += 4;
		flash_addr++;
	}
}
//��flash���ֵ�洢���������������bsp����������������������ֵ
void STMFLASH_Init(void) {
	STMFLASH_Read();

	if ((flash_buffer._start_cod == 0XFEDCBA98) && (flash_buffer._end_cod == 0X76543210)) {
	} else {
		flash_buffer._start_cod = 0XFEDCBA98;
		flash_buffer._end_cod = 0X76543210;
		//tempture data
		flash_buffer.throw_init_value  = _ALL_THROW_LOCK;

		STMFLASH_Write();
	}
	////read flash value to global param struct
	throw_init_value = flash_buffer.throw_init_value;
}
//����ĳ������
void STMFLASH_Erase(short sector) {
	FLASH_Status status = FLASH_COMPLETE;

	FLASH_Unlock();																				//����
//  FLASH_DataCacheCmd(DISABLE);													//FLASH������д��ʱ���κγ��Զ�ȡ�Ĳ����ᵼ�����ߴ���������Ҫ��ȡ�����ݣ����Ļ��棬��д��Ĳ����������ֹ���ݻ��档����ûʲô�ã��ŵ����ﱸע��

	status = FLASH_ErasePage(MY_SETTING_PAGE_ADDR);

	if (status == FLASH_COMPLETE) {														//��������ɹ����ſ�ʼд��
	}

//  FLASH_DataCacheCmd(ENABLE);	//FLASH��������,�������ݻ���
	FLASH_Lock();//����
}
/**
  * @brief  stmflash_process �����û����ô洢��
  * @param  None
  * @retval None
  */
void stmflash_process(void) {
	if (flash_buffer.throw_init_value != throw_init_value) {
		flash_buffer.throw_init_value  = throw_init_value;				//�����������ñ�����������
		STMFLASH_Write();											//����������дflash
	}
}
