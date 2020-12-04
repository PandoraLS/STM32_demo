/******************** (C) COPYRIGHT 2015 ****************************************
 * �ļ���  ��zigbee.c
 * ����    ��         
 * ʵ��ƽ̨��ipc9ѧϰ��
 * Ӳ�����ӣ�---------------------
 *          |  PB.10---TXD(����)  |
 *          |  PB.11---RXD(����)  |
 * ��汾  ��ST3.5.0
 *
**********************************************************************************/
#include "zigbee.h"
/*
 * ��������void Zigbee_usart3(void)
 * ����  ������USART3,���Ƴɲ�����Ϊ115200�������ж�
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void Zigbee_usart3(void)
{  
	GPIO_InitTypeDef GPIO_InitStructure;	  //���ڶ˿����ýṹ�����
	USART_InitTypeDef USART_InitStructure;	  //���ڲ������ýṹ�����
	NVIC_InitTypeDef    NVIC_InitStructure; 					   //�������������жϵĽṹ�����

	//ʹ�� USART3 ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//�򿪴��ڸ���ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   //��PB�˿�ʱ��
  	
	//��USART3 Tx�����ͽţ���GPIO����Ϊ���츴��ģʽ   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				  //ѡ���ĸ�IO�� ��ѡ��PB10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //�趨IO�ڵ�����ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);    				  //��ʼ��GPIOB
	
	//��USART3 Rx�����սţ���GPIO����Ϊ��������ģʽ														  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				  //ѡ���ĸ�IO�� ��ѡ��PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);                    //��ʼ��GPIOA
	  
	//����USART3����
	USART_InitStructure.USART_BaudRate = 9600;	                    //���������ã�115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	    //����λ�����ã�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	        //ֹͣλ���ã�1λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;            //�Ƿ���żУ�飺��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�����뷢�Ͷ�ʹ��
	USART_Init(USART3, &USART_InitStructure);                       //��ʼ��USART3
	
	// USART3�ж����ȼ�����
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                //�ж����ȼ�����  ��ռʽ���ȼ�������Ϊ2λ����Ӧ���ȼ�ռ2λ
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;			   //ָ���ж�Դ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	   //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;             //ָ����Ӧ���ȼ���1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	               //���ж�
	NVIC_Init(&NVIC_InitStructure);							       //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

    //�򿪷����жϺͽ����ж�(�����Ҫ�ж�)
	//USART_ITConfig(USART3, USART_IT_TXE, ENABLE);  // ʹ��ָ����USART3�����ж�
  	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // ʹ��ָ����USART3�����ж�

	//ʹ�� USART3�� �������
	USART_Cmd(USART3, ENABLE);                             // USART3ʹ��

	//�����������1���ֽ��޷���ȷ���ͳ�ȥ������
   USART_ClearFlag(USART3, USART_FLAG_TC);                //�崮��3���ͱ�־
}

/*
 * ��������void usart3_SendByte(unsigned char SendData)
 * ����  ������3�����ַ��ӳ���
 * ����  ��SendDataΪ��������
 * ���  ����
 * ����  ���⡢�ڲ�����
 */
void usart3_SendByte(unsigned char SendData)	  //�ô���3����һ���ֽ�����
{	   
	USART_SendData(USART3,SendData);   //�ô���3����һ���ֽ�
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);  //�ȴ�����3�������
}  
