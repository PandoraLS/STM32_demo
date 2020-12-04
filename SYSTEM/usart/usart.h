#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h" 
#include "stdio.h"	 
#include <stdarg.h>
 
 #define CMD_BUFFER_LEN 50		//���д�������ĳ���
	  	
void USART1_Config(void);   //��ʼ�� ����USART1
void USART2_Config(void);   //��ʼ�� ����USART2
void USART3_Config(void);	//��ʼ�� ����USART3
void UART4_Config(void);	//��ʼ�� ����UART4
void UART5_Config(void);	//��ʼ�� ����UART4


void UART0_Oscilloscope(u32 Data , u8 channel);	//����ʾ����

//�������ʱ��ý�RX��TX�����Ϸ������׿���
//�����������
void USART1_printf (char *fmt, ...);
/*IPC9�������ϵ�OLEDģ�鲻�ܺʹ���2ͬʱʹ�ã�PA2��PA3���ų�ͻ��
��Ҫͬʱʹ�������¶���OLED������*/
void USART2_printf (char *fmt, ...);	
void USART3_printf (char *fmt, ...);
void UART4_printf (char *fmt, ...);
void UART5_printf (char *fmt, ...);

void USART2_SendChar(uint16_t ch);
void USART1_SendChar(uint16_t ch);
#endif	   
