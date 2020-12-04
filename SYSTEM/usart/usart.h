#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h" 
#include "stdio.h"	 
#include <stdarg.h>
 
 #define CMD_BUFFER_LEN 50		//所有串口输出的长度
	  	
void USART1_Config(void);   //初始化 配置USART1
void USART2_Config(void);   //初始化 配置USART2
void USART3_Config(void);	//初始化 配置USART3
void UART4_Config(void);	//初始化 配置UART4
void UART5_Config(void);	//初始化 配置UART4


void UART0_Oscilloscope(u32 Data , u8 channel);	//串口示波器

//串口输出时最好将RX、TX都接上否则容易卡顿
//串口输出程序
void USART1_printf (char *fmt, ...);
/*IPC9开发板上的OLED模块不能和串口2同时使用（PA2，PA3引脚冲突）
若要同时使用则重新定义OLED的引脚*/
void USART2_printf (char *fmt, ...);	
void USART3_printf (char *fmt, ...);
void UART4_printf (char *fmt, ...);
void UART5_printf (char *fmt, ...);

void USART2_SendChar(uint16_t ch);
void USART1_SendChar(uint16_t ch);
#endif	   
