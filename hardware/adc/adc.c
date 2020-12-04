#include "adc.h"

__IO uint16_t ADC_ConvertedValue[NOFCHANEL]={0,0,0,0,0,0,0,0,0,0};
float ADC_ConvertedValueLocal[NOFCHANEL];// 局部变量，用于保存转换计算后的电压值 
/**
  * @brief  ADC GPIO 初始化
  * @param  无
  * @retval 无
  */
static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// 打开 ADC IO端口时钟
	ADC_GPIO_APBxClock_FUN ( ADC_GPIO_CLK, ENABLE );//PC端口	
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能GPIOB端口时钟
	
	// 配置 ADC IO 引脚模式
	GPIO_InitStructure.GPIO_Pin = 	ADC_PIN1|
																	ADC_PIN2|
																	ADC_PIN3|
																	ADC_PIN4|
																	ADC_PIN5|
																	ADC_PIN6;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	
	// 初始化 ADC IO
	GPIO_Init(ADC_PORT, &GPIO_InitStructure);	
	
	
//	// 配置 ADC IO 引脚模式
//	GPIO_InitStructure.GPIO_Pin = 	ADC_PIN7|
//																	ADC_PIN8;

////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//	
//	// 初始化 ADC IO
//	GPIO_Init(GPIOB, &GPIO_InitStructure);		



//	// 配置 ADC IO 引脚模式
//	GPIO_InitStructure.GPIO_Pin = 	ADC_PIN9 | ADC_PIN10;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//	
//	// 初始化 ADC IO
//	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
}

/**
  * @brief  配置ADC工作模式
  * @param  无
  * @retval 无
  */
static void ADCx_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	// 打开DMA时钟
	RCC_AHBPeriphClockCmd(ADC_DMA_CLK, ENABLE);
	// 打开ADC时钟
	ADC_APBxClock_FUN ( ADC_CLK, ENABLE );
	
	// 复位DMA控制器
	DMA_DeInit(ADC_DMA_CHANNEL);
	
	// 配置 DMA 初始化结构体
	// 外设基址为：ADC 数据寄存器地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( u32 ) ( & ( ADC_x->DR ) );
	
	// 存储器地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue;
	
	// 数据源来自外设
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	
	// 缓冲区大小，应该等于数据目的地的大小
	DMA_InitStructure.DMA_BufferSize = NOFCHANEL;
	
	// 外设寄存器只有一个，地址不用递增
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	// 存储器地址递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	
	// 外设数据大小为半字，即两个字节
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	
	// 内存数据大小也为半字，跟外设数据大小相同
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	
	// 循环传输模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	

	// DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
	// 禁止存储器到存储器模式，因为是从外设到存储器
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	// 初始化DMA
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);
	
	// 使能 DMA 通道
	DMA_Cmd(ADC_DMA_CHANNEL , ENABLE);
	
	// ADC 模式配置
	// 只使用一个ADC，属于单模式
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	
	// 扫描模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 

	// 连续转换模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;

	// 不用外部触发转换，软件开启即可
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;

	// 转换结果右对齐
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	// 转换通道个数
	ADC_InitStructure.ADC_NbrOfChannel = NOFCHANEL;	
		
	// 初始化ADC
	ADC_Init(ADC_x, &ADC_InitStructure);
	
	// 配置ADC时钟Ｎ狿CLK2的8分频，即9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	
	// 配置ADC 通道的转换顺序和采样时间
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL1, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL2, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL3, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL4, 4, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL5, 5, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL6, 6, ADC_SampleTime_55Cycles5);
//	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL7, 7, ADC_SampleTime_55Cycles5);
//	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL8, 8, ADC_SampleTime_55Cycles5);
//	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL9, 9, ADC_SampleTime_55Cycles5);
//	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL10, 10, ADC_SampleTime_55Cycles5);
	
	// 使能ADC DMA 请求
	ADC_DMACmd(ADC_x, ENABLE);
	
	// 开启ADC ，并开始转换
	ADC_Cmd(ADC_x, ENABLE);
	
	// 初始化ADC 校准寄存器  
	ADC_ResetCalibration(ADC_x);
	// 等待校准寄存器初始化完成
	while(ADC_GetResetCalibrationStatus(ADC_x));
	
	// ADC开始校准
	ADC_StartCalibration(ADC_x);
	// 等待校准完成
	while(ADC_GetCalibrationStatus(ADC_x));
	
	// 由于没有采用外部触发，所以使用软件触发ADC转换 
	ADC_SoftwareStartConvCmd(ADC_x, ENABLE);
}

/**
  * @brief  ADC初始化
  * @param  无
  * @retval 无
  */
void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_Mode_Config();
}


//// 中值滤波,中位值平均滤波，放脉冲干扰滤波法
//float Filter() {
//  int i, j;
//  float filter_temp, filter_sum = 0;
//  float filter_buf[FILTER_N];
//  for(i = 0; i < FILTER_N; i++) {
//    filter_buf[i] = Get_AD();
//    delay_us(5);
//  }
//  //
//  for(j = 0; j < FILTER_N - 1; j++) {
//    for(i = 0; i < FILTER_N - 1 - j; i++) {
//      if(filter_buf[i] > filter_buf[i + 1]) {
//        filter_temp = filter_buf[i];
//        filter_buf[i] = filter_buf[i + 1];
//        filter_buf[i + 1] = filter_temp;
//      }
//    }
//  }
//  // 去除最大值最小值
//  for(i = 1; i < FILTER_N - 1; i++) 
//		filter_sum += filter_buf[i];
//  return filter_sum / (FILTER_N - 2);
//}

//			filter_buf[add_i] = ADC_ConvertedValueLocal[2];
//			add_i++;
//			if(add_i >= 10)
//			{
//					for(j = 0; j < 10 - 1; j++) {
//						for(i = 0; i < 10 - 1 - j; i++) {
//							if(filter_buf[i] > filter_buf[i + 1]) {
//								filter_temp = filter_buf[i];
//								filter_buf[i] = filter_buf[i + 1];
//								filter_buf[i + 1] = filter_temp;
//								}
//							}
//						}
//					add_i = 0;
//						
//					for(i = 2; i < 10 - 2; i++) 
//						filter_sum += filter_buf[i];
//					adc_filter = filter_sum / 6;
//					printf("CH2 = %5.0f  \r\n",adc_filter);
//			}




//				printf("CH2 = %5.0f  \r\n",ADC_ConvertedValueLocal[2]);
//				OLED_6x8Str(0,0,"Topic I. SWUST");//横坐标 纵坐标 要显示的字符	
//				OLED_6x8Str(0,1,"X:");//横坐标 纵坐标 要显示的字符	
//				OLED_6x8var_ch(15,1,distance_display/10-40);OLED_6x8Str(40,1,".");OLED_6x8var_ch(50,1,distance_display%10);
//				OLED_6x8Str(0,2,"Y:");//横坐标 纵坐标 要显示的字符
//				OLED_6x8Str(0,3,"Position:");//横坐标 纵坐标 要显示的字符
//				OLED_6x8Str(0,4,"Key:");//横坐标 纵坐标 要显示的字符
				
//			printf("\r\n");
//			printf("             CH0 = %5.0f\r\n",ADC_ConvertedValueLocal[0]);
//			printf("CH1 = %5.0f  ",ADC_ConvertedValueLocal[1]);
//			printf("CH2 = %5.0f  ",ADC_ConvertedValueLocal[2]);
//			printf("CH3 = %5.0f\r\n",ADC_ConvertedValueLocal[3]);
//			printf("             CH4 = %5.0f",ADC_ConvertedValueLocal[4]);
//			printf("\r\n");
			
//			Ultra_Ranging_use(&distance);
//			distance_display = (int)distance;
/*********************************************END OF FILE**********************/

