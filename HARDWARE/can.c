#include "can.h"
#include "zkrt.h"	//��ҪZKRT���THROW

volatile uint8_t can1_rx_buff[CAN_BUFFER_SIZE];
volatile uint16_t can1_rx_buff_store = 0;
uint16_t can1_rx_buff_get = 0;

uint8_t CAN_Mode_Init(uint8_t mode) {
	GPIO_InitTypeDef       GPIO_InitStructure;
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN, ENABLE);

//��PA11��PA12��ΪCAN��������ʹ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����CANĬ�ϸߵ�ƽ���ź���ʼλ�͵�ƽ��������������Ϊ�ߵ�ƽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

//����֪AF4��PA11��PA12���õ�CAN
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_4);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_4);

	CAN_DeInit(CAN);
//����CAN����ģʽ
	CAN_InitStructure.CAN_TTCM = DISABLE; //��ʱ�䴥��ͨ��ģʽ
	CAN_InitStructure.CAN_ABOM = DISABLE; //����Զ����߹���
	CAN_InitStructure.CAN_AWUM = DISABLE; //˯��ģʽͨ��������ѣ����CAN1->MCR��SLEEPλ��
	CAN_InitStructure.CAN_NART = DISABLE; //�����Զ����ͷ���ʧ�ܺ�����·���
	CAN_InitStructure.CAN_RFLM = DISABLE; //���Ĳ�����,�µĸ��Ǿɵ�
	CAN_InitStructure.CAN_TXFP = DISABLE; //���ȼ��ɱ��ı�ʶ�����������ó�1�ͱ�����ȼ�����������˳����������

//���ò�����
//������ = fpclk1/((tsjw+tbs1+tbs2)*brp);
//������ = fpclk1/(TS1[3:0]+TS2[2:0]+3)*(BRP[9:0]+1)
//Ҳ����ÿ��ֵ��д��Ĵ�����ʱ�򣬶�Ҫ����ֵ-1д��
//tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:1~4; ����궨��CAN_SJW_1tq=0�����Ĵ�����д��0
//tbs1:ʱ���1��ʱ�䵥Ԫ.��Χ:1~16;	  ����궨��CAN_BS1_7tq=6�����Ĵ�����д��6
//tbs2:ʱ���2��ʱ�䵥Ԫ.��Χ:1~8;		����궨��CAN_BS2_8tq=7�����Ĵ�����д��7
//brp :�����ʷ�Ƶ��.��Χ:1~1024;			����û�к궨�壬���������üĴ�����ʱ��Ҫ����-1��д��
//Fpclk1��ʱ���ڳ�ʼ����ʱ������Ϊ48M����ô����48M/((15+8+1)*2)=1M
	CAN_InitStructure.CAN_Mode = mode; //��ͨģʽ������ģʽ����Ĭģʽ�����ؾ�Ĭģʽ
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_8tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
	CAN_InitStructure.CAN_Prescaler = 8;

//������11�������󣬽���CAN��ʼ��
	CAN_Init(CAN, &CAN_InitStructure);// ��ʼ��CAN1

//���ù�����0�������κ����ݣ�ֻ����ID=1������
	CAN_FilterInitStructure.CAN_FilterNumber = 0;	 //������0
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; //32λ
	CAN_FilterInitStructure.CAN_FilterIdHigh = (DEVICE_TYPE_SELF << 5);
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000; //32λMASK������0000������㲥
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0; //������0������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE; //���������0
	CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ��

	NVIC_InitStructure.NVIC_IRQChannel = CEC_CAN_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	CAN_ITConfig(CAN, CAN_IT_FMP0, ENABLE); //FIFO0��Ϣ�Һ��ж�����

	return 0;
}

void CEC_CAN_IRQHandler(void) {
	uint8_t i;
	CanRxMsg RxMessage;

	CAN_Receive(CAN, CAN_FIFO0, &RxMessage);//CAN�豸���FIFO0�����ݣ���ȡ�����浽RxMessage�ṹ����

	for (i = 0; i < RxMessage.DLC; i++) { //�������ݳ�������������洢����
		can1_rx_buff[can1_rx_buff_store] = RxMessage.Data[i];

		can1_rx_buff_store++;
		if (can1_rx_buff_store == CAN_BUFFER_SIZE) {
			can1_rx_buff_store = 0;
		}
	}

	GPIO_ResetBits(GPIOB, GPIO_Pin_7);
	led_rx_count = TimingDelay;
}

//ͨ���ж�get��store��λ�ù�ϵ�������Ƿ�����ֵ
uint8_t CAN1_rx_check(void) {
	if (can1_rx_buff_store == can1_rx_buff_get)//��û���µ�ֵ����ʱ���洢ֵλ�õ���ȡ��ֵλ��
		return 0;
	else
		return 1;//����ֵ����ʱ������1
}

//������ֵ��ʱ��ȡ��һ��ֵ
uint8_t CAN1_rx_byte(void) {
	uint8_t ch;

	ch = can1_rx_buff[can1_rx_buff_get];//��ch��¼�����յ���һ������

	can1_rx_buff_get++;
	if (can1_rx_buff_get == CAN_BUFFER_SIZE) {
		can1_rx_buff_get = 0;
	}

	return ch;
}

////can����һ������(�̶���ʽ:IDΪ0X01,��׼֡,����֡)
////len:���ݳ���(���Ϊ8)
////msg:����ָ��,���Ϊ8���ֽ�.
////����ֵ:0,�ɹ�;����,ʧ��;
//uint8_t Can_Send_Msg(uint8_t* msg,uint8_t len)
//{
//  uint8_t mbox;
//  uint16_t i=0;
//  CanTxMsg TxMessage;						           //�ýṹ�嶨��������֡��������Ϣ
//
//	TxMessage.StdId=(DEVICE_TYPE_THROW<<4);  //��׼��ʶ���������Ǹ����豸ID����4λ�õ��ģ���������Ͷ���õ�����0X60
//  TxMessage.ExtId=0x00;				             //������չ��ʾ��
//  TxMessage.IDE=CAN_Id_Standard;           //��׼֡��ע�⣬һ��ѡ���˱�׼֡����չ��ʶ���Ͳ���������
//  TxMessage.RTR=CAN_RTR_Data;		           //����֡
//  TxMessage.DLC=len;						           //Ҫ���͵����ݳ���
//  for(i=0;i<len;i++)
//	TxMessage.Data[i]=msg[i];                //����Ҫд������
//
//	mbox= CAN_Transmit(CAN, &TxMessage);     //����ֵ������ţ����û���ͳɹ��򷵻�4
//
//  i=0;
//  while((CAN_TransmitStatus(CAN, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//����ʧ�ܵ�ʱ���ٸ����ɴλ��ᣬֱ������ľ�Ҳ�ͳ�����
//
//  if(i>=0XFFF)
//	{
//		CAN_Mode_Init(CAN_Mode_Normal);
//		return 1;
//  }
//
//	GPIO_ResetBits(GPIOB, GPIO_Pin_6);
//	led_tx_count = TimingDelay;
//
//	return 0;		//�ɹ�����

//}
uint8_t Can_Send_Msg(uint8_t* msg, uint8_t len) {
	uint8_t status;
	uint8_t mbox;
	uint16_t i = 0;
	CanTxMsg TxMessage;

//pack data
	TxMessage.StdId = (DEVICE_TYPE_SELF << 4);
	TxMessage.ExtId = 0x00;
	TxMessage.IDE = CAN_Id_Standard;
	TxMessage.RTR = CAN_RTR_Data;
	TxMessage.DLC = len;
	for (i = 0; i < len; i++)
		TxMessage.Data[i] = msg[i];

	GPIO_ResetBits(GPIOB, GPIO_Pin_6);
	led_tx_count = TimingDelay;

//transmit
	i = 0;
	mbox = CAN_Transmit(CAN, &TxMessage);
	while ((mbox == CAN_TxStatus_NoMailBox) && (i < 0x4FFF)) {
		i++;
		mbox = CAN_Transmit(CAN, &TxMessage);
	}
	if (mbox == CAN_TxStatus_NoMailBox) {
		return 1;
	}
//check transmitstatus
	i = 0;
	status = CAN_TransmitStatus(CAN, mbox);
	while ((status != CAN_TxStatus_Ok) && (i < 0x4FFF)) {
		i++;
		status = CAN_TransmitStatus(CAN, mbox);
	}
	if (i >= 0x4FFF) {
		if (status == CAN_TxStatus_Pending) {
//			printf("can mbox[%d] CAN_TxStatus_Pending\n", mbox);
		} else if (status == CAN_TxStatus_Failed) {
			CAN_Mode_Init(CAN_Mode_Normal);
//			printf("can mbox[%d] CAN_TxStatus_Failed\n", mbox);
			return 1;
		}
	}

	return 0;		//�ɹ�����
}
//����ģ������ݷ��ظ�����ģ��
uint8_t CAN1_send_message_fun(uint8_t *message, uint8_t len) {
	//����һ��50����
	uint8_t count;
	uint8_t time;
	uint8_t ret;

	time = len / 8;

	for (count = 0; count < time; count++) {
		ret = Can_Send_Msg(message, 8);
		if (ret)
			return ret;
		message += 8;
	}
	if (len % 8) {
		ret = Can_Send_Msg(message, len % 8);
	}
	return ret;
}

