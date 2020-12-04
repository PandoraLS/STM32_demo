/******************** (C) COPYRIGHT 2016 ****************************************
 * �ļ���  ��exti.c
 * ����    �������жϵ�����         
 * ʵ��ƽ̨��IPC9ѧϰ��  			
 * ��汾  ��ST3.0.0
 *
 * ����    ���
**********************************************************************************/
#include "exti.h"

/*
 * ��������void KEY_Configuration()
 * ����  �������жϵ�����
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void KEY_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;	
	EXTI_InitTypeDef EXTI_InitStruct;	
	NVIC_InitTypeDef NVIC_InitStruct;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE ,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO ,ENABLE);	//����ʱ��ʹ��
	
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IPU;	//����Ϊ��������ģʽ
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_4 ;	
	GPIO_Init(GPIOE,&GPIO_InitStruct);	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);	//�����ж�ΪPE5
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line4;	
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;	//����Ϊ�ж�ģʽ
	EXTI_InitStruct.EXTI_Trigger 	= EXTI_Trigger_Falling;	//�����½��ش���

	EXTI_InitStruct.EXTI_LineCmd 	= ENABLE;	//ʹ���ж�
	
	EXTI_Init(&EXTI_InitStruct);
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	//��ռ���ȼ�������ȼ��ķ���
	
	NVIC_InitStruct.NVIC_IRQChannel= EXTI4_IRQn;	
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;	
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStruct);
}




/******************* (C) COPYRIGHT 2016 **********************************END OF FILE*****/
