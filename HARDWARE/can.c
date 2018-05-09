#include "can.h"
#include "zkrt.h"	//需要ZKRT里的THROW

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

//让PA11、PA12作为CAN的两根线使用
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//由于CAN默认高电平，信号起始位低电平，所以这里配置为高电平
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

//查表得知AF4是PA11、PA12复用到CAN
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_4);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_4);

	CAN_DeInit(CAN);
//设置CAN工作模式
	CAN_InitStructure.CAN_TTCM = DISABLE; //非时间触发通信模式
	CAN_InitStructure.CAN_ABOM = DISABLE; //软件自动离线管理
	CAN_InitStructure.CAN_AWUM = DISABLE; //睡眠模式通过软件唤醒（清除CAN1->MCR的SLEEP位）
	CAN_InitStructure.CAN_NART = DISABLE; //报文自动传送发送失败后会重新发送
	CAN_InitStructure.CAN_RFLM = DISABLE; //报文不锁定,新的覆盖旧的
	CAN_InitStructure.CAN_TXFP = DISABLE; //优先级由报文标识符决定（设置成1就变成优先级按发送请求顺序来决定）

//设置波特率
//波特率 = fpclk1/((tsjw+tbs1+tbs2)*brp);
//波特率 = fpclk1/(TS1[3:0]+TS2[2:0]+3)*(BRP[9:0]+1)
//也就是每个值在写入寄存器的时候，都要将其值-1写入
//tsjw:重新同步跳跃时间单元.范围:1~4; 这里宏定义CAN_SJW_1tq=0，往寄存器里写入0
//tbs1:时间段1的时间单元.范围:1~16;	  这里宏定义CAN_BS1_7tq=6，往寄存器里写入6
//tbs2:时间段2的时间单元.范围:1~8;		这里宏定义CAN_BS2_8tq=7，往寄存器里写入7
//brp :波特率分频器.范围:1~1024;			这里没有宏定义，所以在配置寄存器的时候要将其-1再写入
//Fpclk1的时钟在初始化的时候设置为48M，那么就是48M/((15+8+1)*2)=1M
	CAN_InitStructure.CAN_Mode = mode; //普通模式、环回模式、静默模式、环回静默模式
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_8tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
	CAN_InitStructure.CAN_Prescaler = 8;

//设置完11个参数后，进行CAN初始化
	CAN_Init(CAN, &CAN_InitStructure);// 初始化CAN1

//设置过滤器0，接收任何数据，只接受ID=1的数据
	CAN_FilterInitStructure.CAN_FilterNumber = 0;	 //过滤器0
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; //32位
	CAN_FilterInitStructure.CAN_FilterIdHigh = (DEVICE_TYPE_SELF << 5);
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000; //32位MASK，保存0000，代表广播
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0; //过滤器0关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE; //激活过滤器0
	CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化

	NVIC_InitStructure.NVIC_IRQChannel = CEC_CAN_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	CAN_ITConfig(CAN, CAN_IT_FMP0, ENABLE); //FIFO0消息挂号中断允许

	return 0;
}

void CEC_CAN_IRQHandler(void) {
	uint8_t i;
	CanRxMsg RxMessage;

	CAN_Receive(CAN, CAN_FIFO0, &RxMessage);//CAN设备里的FIFO0的数据，读取出来存到RxMessage结构体里

	for (i = 0; i < RxMessage.DLC; i++) { //按照数据长度往缓冲区里存储数据
		can1_rx_buff[can1_rx_buff_store] = RxMessage.Data[i];

		can1_rx_buff_store++;
		if (can1_rx_buff_store == CAN_BUFFER_SIZE) {
			can1_rx_buff_store = 0;
		}
	}

	GPIO_ResetBits(GPIOB, GPIO_Pin_7);
	led_rx_count = TimingDelay;
}

//通过判断get和store的位置关系来决定是否有新值
uint8_t CAN1_rx_check(void) {
	if (can1_rx_buff_store == can1_rx_buff_get)//当没有新的值到达时，存储值位置等于取出值位置
		return 0;
	else
		return 1;//当有值到达时，返回1
}

//当有新值的时候，取出一个值
uint8_t CAN1_rx_byte(void) {
	uint8_t ch;

	ch = can1_rx_buff[can1_rx_buff_get];//用ch记录下来收到的一个数据

	can1_rx_buff_get++;
	if (can1_rx_buff_get == CAN_BUFFER_SIZE) {
		can1_rx_buff_get = 0;
	}

	return ch;
}

////can发送一组数据(固定格式:ID为0X01,标准帧,数据帧)
////len:数据长度(最大为8)
////msg:数据指针,最大为8个字节.
////返回值:0,成功;其他,失败;
//uint8_t Can_Send_Msg(uint8_t* msg,uint8_t len)
//{
//  uint8_t mbox;
//  uint16_t i=0;
//  CanTxMsg TxMessage;						           //该结构体定义了数据帧的所有信息
//
//	TxMessage.StdId=(DEVICE_TYPE_THROW<<4);  //标准标识符，这里是根据设备ID左移4位得到的，这里是抛投，得到的是0X60
//  TxMessage.ExtId=0x00;				             //设置扩展标示符
//  TxMessage.IDE=CAN_Id_Standard;           //标准帧。注意，一旦选择了标准帧，扩展标识符就不再有意义
//  TxMessage.RTR=CAN_RTR_Data;		           //数据帧
//  TxMessage.DLC=len;						           //要发送的数据长度
//  for(i=0;i<len;i++)
//	TxMessage.Data[i]=msg[i];                //填上要写的数据
//
//	mbox= CAN_Transmit(CAN, &TxMessage);     //返回值是邮箱号，如果没发送成功则返回4
//
//  i=0;
//  while((CAN_TransmitStatus(CAN, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//发送失败的时候，再给若干次机会，直到机会耗尽也就出错返回
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
//	return 0;		//成功返回

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

	return 0;		//成功返回
}
//将子模块的数据返回给管理模块
uint8_t CAN1_send_message_fun(uint8_t *message, uint8_t len) {
	//假设一共50个字
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

