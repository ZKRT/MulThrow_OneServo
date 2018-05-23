#include "adc.h"

//#define ADC_KIND        7
#define ADC_KIND        2
#define ADC_BUFFER_SIZE (ADC_KIND*10)
volatile uint16_t adc1_rx_buffer[ADC_BUFFER_SIZE];

void ADC1_Init(void) {
	ADC_InitTypeDef     ADC_InitStructure;
	GPIO_InitTypeDef    GPIO_InitStructure;
	DMA_InitTypeDef   DMA_InitStructure;

	ADC_DeInit(ADC1);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);

	ADC_StructInit(&ADC_InitStructure);
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;//从通道0到通道16的方向
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_ChannelConfig(ADC1, ADC_Channel_0 , ADC_SampleTime_239_5Cycles);
	ADC_ChannelConfig(ADC1, ADC_Channel_4 , ADC_SampleTime_239_5Cycles);
//	ADC_ChannelConfig(ADC1, ADC_Channel_5 , ADC_SampleTime_239_5Cycles);
//	ADC_ChannelConfig(ADC1, ADC_Channel_6 , ADC_SampleTime_239_5Cycles);
//	ADC_ChannelConfig(ADC1, ADC_Channel_7 , ADC_SampleTime_239_5Cycles);
//	ADC_ChannelConfig(ADC1, ADC_Channel_8 , ADC_SampleTime_239_5Cycles);
//	ADC_ChannelConfig(ADC1, ADC_Channel_9 , ADC_SampleTime_239_5Cycles);

	/* ADC Calibration */
	ADC_GetCalibrationFactor(ADC1);

	/* ADC DMA request in circular mode */
	ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);

	/* Enable ADC_DMA */
	ADC_DMACmd(ADC1, ENABLE);

	/* Enable the ADC peripheral */
	ADC_Cmd(ADC1, ENABLE);

	/* Wait the ADRDY flag */
	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY));

	/* DMA1 Channel1 Config */
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR));
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)adc1_rx_buffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = ADC_BUFFER_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	/* DMA1 Channel1 enable */
	DMA_Cmd(DMA1_Channel1, ENABLE);
}

uint16_t get_adc_val(uint8_t read_type) {
	uint8_t count = 0;
	uint32_t sum = 0;

	for (count = 0; count < 10; count++) {							//取10次
		sum += adc1_rx_buffer[count * ADC_KIND + read_type];
	}

	sum /= 10;

//	switch (read_type)
//	{
//		case _25V_VOL:
//		sum = (sum*67925)/9216;	//vol = (X*3300/4096)*(247/27) = X * 67925/9216￡???·?êy12
//		break;
//		case _5VA_VOL:
//		case _5VB_VOL:
//		case _5VC_VOL:
//		sum = (sum* 715)/512;	//vol = (X*3300/4096)*(52/30)  = X * 715/512￡?   ??·?êy240
//		break;
//		case _5VA_IS:
//		case _5VB_IS:
//		case _5VC_IS:
//		sum = (sum * 165)/128;	//vol = (X*3300/4096)/0.625    = X * 165/128?ê? ???¤?¨oy96
//		break;
//	}
	switch (read_type) {
	case _5V_VOL:
		sum = (sum * 825) / 512;	//vol = (X*3300/4096)*(20/10) = X * 67925/9216￡???·?êy12
		break;
	case _12V_VOL:
		sum = (sum * 10725) / 3072;	//vol = (X*3300/4096)*(130/30)  = X * 715/512￡?   ??·?êy240
		break;
	default:
		break;
	}
	return sum;
}

//uint16_t adc_25vol;				//0
//uint16_t adc_5A_vol;			//1
//uint16_t adc_5B_vol;			//2
//uint16_t adc_5C_vol;			//3
//uint16_t adc_5A_is;				//4
//uint16_t adc_5B_is;				//5
//uint16_t adc_5C_is;				//6
//uint8_t V25_error_flag = 0;
//uint8_t V5A_error_flag = 0;
//uint8_t V5B_error_flag = 0;
//uint8_t V5C_error_flag = 0;

uint16_t adc_12vol;				//1
uint16_t adc_5vol;			  //0
uint8_t V12_error_flag = 0;
uint8_t V5_error_flag = 0;

//电池读取
void bat_read(void) {
	adc_5vol  = get_adc_val(_5V_VOL);
	adc_12vol = get_adc_val(_12V_VOL);
}

//电池校验
void bat_check(void) {
	if ((adc_12vol > 13000) || (adc_12vol < 11000)) {
		if (V12_error_flag == 0) {
			GPIO_ResetBits(GPIOA, GPIO_Pin_8);
			delay_ms(500);
		}
		V12_error_flag = 1;
	} else {
		V12_error_flag = 0;
	}

	if ((adc_5vol > 5500) || (adc_5vol < 4500)) {
		if (V5_error_flag == 0) {
			GPIO_ResetBits(GPIOB, GPIO_Pin_3);
			delay_ms(500);
		}
		V5_error_flag = 1;
	} else {
		V5_error_flag = 0;
	}
}

////电池读取
//void bat_read(void)
//{
//	adc_25vol  = get_adc_val(_25V_VOL);
//	adc_5A_vol = get_adc_val(_5VA_VOL);
//	adc_5B_vol = get_adc_val(_5VB_VOL);
//	adc_5C_vol = get_adc_val(_5VC_VOL);
//	adc_5A_is  = get_adc_val(_5VA_IS);
//	adc_5B_is  = get_adc_val(_5VB_IS);
//	adc_5C_is  = get_adc_val(_5VC_IS);
//}

////电池校验
//void bat_check(void)
//{
//	if ((adc_25vol > 26000) || (adc_25vol < 20000))
//	{
//		V25_error_flag = 1;
//	}
//	else
//	{
//		V25_error_flag = 0;
//	}
//
////	if ((adc_5A_vol > 5565) || (adc_5A_vol < 4800) || (adc_5A_is > 3000))  //modify for 单舵机_多抛
////	{
////		if (V5A_error_flag == 0)
////		{
////			GPIO_ResetBits(GPIOA,GPIO_Pin_8);
////			delay_ms(500);
////		}
////		V5A_error_flag = 1;
////	}
////	else
////	{
////		V5A_error_flag = 0;
////	}
////
////	if ((adc_5B_vol > 5565) || (adc_5B_vol < 4800) || (adc_5B_is > 3000))
////	{
////		if (V5B_error_flag == 0)
////		{
////			GPIO_ResetBits(GPIOA,GPIO_Pin_9);
////			delay_ms(500);
////		}
////		V5B_error_flag = 1;
////	}
////	else
////	{
////		V5B_error_flag = 0;
////	}
//
////	if ((adc_5C_vol > 5565) || (adc_5C_vol < 4800) || (adc_5C_is > 3000))
//	if ((adc_5C_vol > 5800) || (adc_5C_vol < 4800) || (adc_5C_is > 3000))   //modify by yanly 电压留有余地
//	{
//		if (V5C_error_flag == 0)
//		{
//			GPIO_ResetBits(GPIOA,GPIO_Pin_10);
//			delay_ms(500);
//		}
//		V5C_error_flag = 1;
//	}
//	else
//	{
//		V5C_error_flag = 0;
//	}
//}



