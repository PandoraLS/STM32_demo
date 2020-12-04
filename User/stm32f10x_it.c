/**
  ******************************************************************************
  * @file    main.c
  * @author  LJ
  * @version V1.0
  * @date    2016-7-13
  * @brief   用3.5.0版本库建的工程模板
  ******************************************************************************
  * 实验平台:IPC9开发板 
  ******************************************************************************
  * @attention  定时器1~8的中断
  *             按键中断
  *
  *
  * Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "usart.h"
#include "OLED.h"
#include "encoder.h"
#include "led.h"
#include "beep.h"
#include "adc.h"
#include "ultrasonic.h"


/*定时器1中断*/
void TIM1_UP_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM1 , TIM_IT_Update) != RESET ) 
	{
		LED0 = !LED0;
	}
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);		//清除中断标志位
	
	
	
//	if(TIM1->SR&0X0001) //5ms定时中断
//	{   
//		Encoder_Left=Read_Encoder(2); 
//		printf("%d    ",Encoder_Left);
//		TIM1->SR&=~(1<<0);  //清除中断标志位                                     //===清除定时器1中断标志位		 
//					
//	} 		
}

/*定时器2中断*/
void TIM2_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
	{
		printf("  2222 \r\n  ");
		LED0 = !LED0;
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);		//清除中断标志位
}

/*定时器3中断*/
void TIM3_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM3 , TIM_IT_Update) != RESET ) 
	{
//		printf("  333 \r\n  ");
		LED1 = !LED1;
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);		//清除中断标志位
}

/*定时器4中断*/
void TIM4_IRQHandler(void)
{
//	if ( TIM_GetITStatus(TIM4 , TIM_IT_Update) != RESET ) 
//	{
////		printf("1   ");
//				LED0 = !LED0;
//	}
//	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);		//清除中断标志位
}

/*定时器5中断*/
void TIM5_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM5 , TIM_IT_Update) != RESET ) 
	{
//		printf("1   ");
//				LED1 = !LED1;
//		printf("Y_zuobiao =%f  \r\n ",Y_zuobiao/6);
		
	}
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);		//清除中断标志位
}
u8 adc_i;
static int add_i = 0;
float temp[10];
static  float filter_buf[200],filter_buf1[200],filter_buf2[200],filter_buf3[200],filter_buf4[200];
static int init_flag = 1;
static float init_adc = 0;
static float xishu = 1.0;
static int init_i = 0;
static float guangmin_xiuzheng;
float Y_zuobiao;
/*定时器6中断*/
void TIM6_IRQHandler(void)
{
	int i, j;
  float filter_temp, filter_sum = 0,filter_sum1 = 0,filter_sum2=0,filter_sum3=0,filter_sum4=0;
	float adc_filter,adc_filter1,adc_filter2,adc_filter3,adc_filter4;
	float sum_init = 0;
	if ( TIM_GetITStatus(TIM6,TIM_IT_Update) != RESET) 
	{
			ADC_ConvertedValueLocal[0] =(float) 10000*ADC_ConvertedValue[0]/4096*3.3;
			ADC_ConvertedValueLocal[1] =(float) 10000*ADC_ConvertedValue[1]/4096*3.3;
			ADC_ConvertedValueLocal[2] =(float) 10000*ADC_ConvertedValue[2]/4096*3.3;
			ADC_ConvertedValueLocal[3] =(float) 10000*ADC_ConvertedValue[3]/4096*3.3;
			ADC_ConvertedValueLocal[4] =(float) 10000*ADC_ConvertedValue[4]/4096*3.3;
			//均值滤波
			filter_buf[add_i] = ADC_ConvertedValueLocal[0];
			filter_buf1[add_i] = ADC_ConvertedValueLocal[1];
			filter_buf2[add_i] = ADC_ConvertedValueLocal[2];
			filter_buf3[add_i] = ADC_ConvertedValueLocal[3];
			filter_buf4[add_i] = ADC_ConvertedValueLocal[4];
			add_i++;
			if(add_i >= 200)
			{
				LED0 = !LED0;
				for(i = 0; i < 200; i++) 
					filter_sum += filter_buf[i];
				adc_filter = filter_sum / 200;
				for(i = 0; i < 200; i++) 
					filter_sum1 += filter_buf1[i];
				adc_filter1 = filter_sum1 / 200;
				for(i = 0; i < 200; i++) 
					filter_sum2 += filter_buf2[i];
				adc_filter2 = filter_sum2 / 200;
				for(i = 0; i < 200; i++) 
					filter_sum3 += filter_buf3[i];
				adc_filter3 = filter_sum3 / 200;
				for(i = 0; i < 200; i++) 
					filter_sum4 += filter_buf4[i];
				adc_filter4 = filter_sum4 / 200;
				
				add_i = 0;
				printf("\r\n              CH0 = %5.0f  \r\n",adc_filter);
				printf("CH1 = %5.0f   ",adc_filter1);
				printf("CH2 = %5.0f   ",adc_filter2);
				printf("CH3 = %5.0f   \r\n",adc_filter3);
				printf("              CH4 = %5.0f   \r\n",adc_filter4);
			}
	}
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);		//清除中断标志位
}


/*定时器7中断*/
void TIM7_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM7 , TIM_IT_Update) != RESET ) 
	{
//		printf("1   ");
				LED0 = !LED0;
	}
	TIM_ClearITPendingBit(TIM7,TIM_IT_Update);		//清除中断标志位
}

/*按键PE4中断*/
void EXTI4_IRQHandler(void)
{
//	if (EXTI_GetITStatus(EXTI_Line4) != RESET)
//	{
//		USART1_printf("1  ");
//		LED0 = !LED0;
//	}
//	EXTI_ClearFlag(EXTI_Line4);
}

/*串口1的中断*/
/*                       *
 *   串口1中断未能实现   *
 *                       */
u8 usart1_Rx_Data;

void USART1_IRQHandler(void)		   //串口全局中断服务函数
{
////	if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)  //接收中断有效,若接收数据寄存器满
////    {
////		LCD_8x16var_int(0,0,12);
////	}	
//	 LCD_8x16var_int(0,0,12);	
////	 USART_ClearFlag(USART1,USART_FLAG_RXNE); //清除中断标志位
//	 USART_ClearITPendingBit(USART1,TIM_FLAG_Update);//清除中断标志位
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
	{	
		usart1_Rx_Data = USART_ReceiveData(USART1);
//		USART_SendData(USART1,usart1_Rx_Data);
//		printf("1");
//		USART_SendData(USART2,usart1_Rx_Data);//将从串口助手接收到的数据通过串口2发出去，发到蓝牙接收端
		
	}
	USART_ClearITPendingBit(USART1,USART_IT_RXNE);
}
u16 Bluetooth_Rxdata;
/*串口2的中断*/
void USART2_IRQHandler(void)		   //串口全局中断服务函数,用于蓝牙中断接收
{
//		if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
//		{	
//			Bluetooth_Rxdata = USART_ReceiveData(USART2);
//			USART_SendData(USART1,Bluetooth_Rxdata);
//		}
//		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
}

u16 zigbee_Rxdata;
/*串口3的中断*/
void USART3_IRQHandler(void)		   //串口全局中断服务函数
{
		if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断有效,若接收数据寄存器满
		{
				USART_ClearITPendingBit(USART3,USART_IT_RXNE);
				BEEP = !BEEP;
				LED0 = !LED0;
			zigbee_Rxdata = USART_ReceiveData(USART3);
			USART_SendData(USART1,zigbee_Rxdata);
		}
//		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
}


/*串口4的全局中断*/
void UART4_IRQHandler(void)			   //串口4全局中断服务函数
{
//	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //接收中断有效,若接收数据寄存器满
//  	{
//  	}
		USART_ClearITPendingBit(UART4,USART_IT_RXNE);
}


//void TIM6_IRQHandler(void)
//{
//	int i, j;
//  float filter_temp, filter_sum = 0;
//	float adc_filter;
//	float sum_init = 0;
//	if ( TIM_GetITStatus(TIM6 , TIM_IT_Update) != RESET ) 
//	{
////		if(init_flag == 1)
////		{
////				ADC_ConvertedValueLocal[2] =(float) 10000*ADC_ConvertedValue[2]/4096*3.3;
////				filter_buf[init_i] = ADC_ConvertedValueLocal[2];
////				init_i++;
////				if(init_i >= 200)
////				{
//////					LED1 = !LED1;
////					for(i = 0; i < 200; i++) 
////						sum_init += filter_buf[i];
////					init_adc = sum_init / 200;
////					init_i = 0;
////					xishu = init_adc/11336;
////					printf("CH2 = %5.0f..",init_adc);
////					printf("xishu = %f.......................  \r\n",xishu);
////					init_flag = 0;
////				}
////				
////		}
////		else
////		{
//			ADC_ConvertedValueLocal[2] =(float) 10000*ADC_ConvertedValue[2]/4096*3.3;
//			//均值滤波
//			filter_buf[add_i] = ADC_ConvertedValueLocal[2];
//			add_i++;
//			if(add_i >= 200)
//			{
//				LED0 = !LED0;
//				for(i = 0; i < 200; i++) 
//					filter_sum += filter_buf[i];
//				adc_filter = filter_sum / 200;
//				add_i = 0;
//				printf("CH2 = %5.0f ",adc_filter);
//				
////				guangmin_xiuzheng = adc_filter/xishu/1000;
//////				printf("修正 = %f       \r\n",guangmin_xiuzheng);
////				Y_zuobiao = (-10.802 * guangmin_xiuzheng * guangmin_xiuzheng +375.46*guangmin_xiuzheng - 2501.3);
////				printf("Y_zuobiao =%f  \r\n ",Y_zuobiao);
////				OLED_6x8Str(0,0,"Topic I. SWUST");//横坐标 纵坐标 要显示的字符	
////				OLED_6x8Str(0,1,"X:");//横坐标 纵坐标 要显示的字符	
////				OLED_6x8var_ch(15,1,distance_display/10-40);OLED_6x8Str(40,1,".");OLED_6x8var_ch(50,1,distance_display%10);
////				OLED_6x8Str(0,2,"Y:");//横坐标 纵坐标 要显示的字符
////				OLED_6x8Str(0,3,"Position:");//横坐标 纵坐标 要显示的字符
////				OLED_6x8Str(0,4,"Key:");//横坐标 纵坐标 要显示的字符
//			}
////		}
//	}
//	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);		//清除中断标志位
//}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
