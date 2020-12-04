/******************** (C) COPYRIGHT 2016 ****************************************
 * 文件名  ：exti.c
 * 描述    ：按键中断的配置         
 * 实验平台：IPC9学习板  			
 * 库版本  ：ST3.0.0
 *
 * 作者    ：李俊
**********************************************************************************/
#include "exti.h"

/*
 * 函数名：void KEY_Configuration()
 * 描述  ：按键中断的配置
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void KEY_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;	
	EXTI_InitTypeDef EXTI_InitStruct;	
	NVIC_InitTypeDef NVIC_InitStruct;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE ,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO ,ENABLE);	//复用时钟使能
	
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IPU;	//设置为上拉输入模式
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_4 ;	
	GPIO_Init(GPIOE,&GPIO_InitStruct);	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);	//设置中断为PE5
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line4;	
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;	//设置为中断模式
	EXTI_InitStruct.EXTI_Trigger 	= EXTI_Trigger_Falling;	//设置下降沿触发

	EXTI_InitStruct.EXTI_LineCmd 	= ENABLE;	//使能中断
	
	EXTI_Init(&EXTI_InitStruct);
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	//先占优先级与从优先级的分配
	
	NVIC_InitStruct.NVIC_IRQChannel= EXTI4_IRQn;	
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;	
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStruct);
}




/******************* (C) COPYRIGHT 2016 **********************************END OF FILE*****/
