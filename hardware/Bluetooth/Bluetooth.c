/******************** (C) COPYRIGHT 2015 ****************************************
 * �ļ���  ��Bluetooth.c
 * ����    ��         
 * ʵ��ƽ̨��ipc8ѧϰ��
 * Ӳ�����ӣ�---------------------
 *          |  PA.2---RXD(����)  |
 *          |  PA.3---TXD(����)  |
 * ��汾  ��ST3.5.0
 *
**********************************************************************************/
#include "Bluetooth.h"

/*
 * ��������void Bluetooth_uart5(void)
 * ����  ������USART2,���Ƴɲ�����Ϊ9600�������ж�
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void Bluetooth_usart2(void)
{  
	//����4��ʼ��
	GPIO_InitTypeDef GPIO_InitStructure;	   //���ڶ˿����ýṹ�����
	USART_InitTypeDef USART_InitStructure;	   //���ڲ������ýṹ�����
  NVIC_InitTypeDef NVIC_InitStructure; 
	//ʹ�� USART2 ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//�򿪴��ڸ���ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //��PA�˿�ʱ��
	
	//��USART2 Tx�����ͽţ���GPIO����Ϊ���츴��ģʽ   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				  //ѡ���ĸ�IO�� ��ѡ��PA2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //�趨IO�ڵ�����ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);    				  //��ʼ��GPIOA

	//��UART4 Rx�����սţ���GPIO����Ϊ��������ģʽ														  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				  //ѡ���ĸ�IO�� ��ѡ��PA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);                    //��ʼ��GPIOA
	  
	//����UART4����
	USART_InitStructure.USART_BaudRate = 9600;	                    //���������ã�9600
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	    //����λ�����ã�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	        //ֹͣλ���ã�1λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;            //�Ƿ���żУ�飺��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�����뷢�Ͷ�ʹ��
	USART_Init(USART2, &USART_InitStructure);                       //��ʼ��USART2
	
	//�򿪷����жϺͽ����ж�(�����Ҫ�ж�)
	//USART_ITConfig(UART4, USART_IT_TXE, ENABLE);  // ʹ��ָ����USART2�����ж�
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // ʹ��ָ����USART2�����ж�
	/* Configure the NVIC Preemption Priority Bits */  
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);//ע�����ǰ��������������������һ��Ҫ����
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//ע�⴮��1~3�õ���USART1_IRQn\USART2_IRQn\USART3_IRQn\����4~5�õ���UART4_IRQn��UART5_IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//ʹ�� UART4�� �������
	USART_Cmd(USART2, ENABLE);                             // USART2ʹ��
 
	//�����������1���ֽ��޷���ȷ���ͳ�ȥ������
  USART_ClearFlag(USART2, USART_FLAG_TC);                //�崮��4���ͱ�־
}

/*
 * ��������void usart4_SendByte(unsigned char SendData)
 * ����  ������4�����ַ��ӳ���
 * ����  ��SendDataΪ��������
 * ���  ����
 * ����  ���⡢�ڲ�����
 */
void usart2_SendByte(unsigned char SendData)	  //�ô���2����һ���ֽ�����
{	   
	USART_SendData(USART2,SendData);   //�ô���3����һ���ֽ�
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);  //�ȴ�����2�������
}  
