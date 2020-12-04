/******************** (C) COPYRIGHT 2015 ****************************************
 * 文件名  ：Bluetooth.c
 * 描述    ：         
 * 实验平台：ipc8学习板
 * 硬件连接：---------------------
 *          |  PA.2---RXD(蓝牙)  |
 *          |  PA.3---TXD(蓝牙)  |
 * 库版本  ：ST3.5.0
 *
**********************************************************************************/
#include "Bluetooth.h"

/*
 * 函数名：void Bluetooth_uart5(void)
 * 描述  ：配置USART2,配制成波特率为9600，接受中断
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void Bluetooth_usart2(void)
{  
	//串口4初始化
	GPIO_InitTypeDef GPIO_InitStructure;	   //串口端口配置结构体变量
	USART_InitTypeDef USART_InitStructure;	   //串口参数配置结构体变量
  NVIC_InitTypeDef NVIC_InitStructure; 
	//使能 USART2 时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//打开串口复用时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //打开PA端口时钟
	
	//将USART2 Tx（发送脚）的GPIO配置为推挽复用模式   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				  //选定哪个IO口 现选定PA2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //设定IO口的输出速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);    				  //初始化GPIOA

	//将UART4 Rx（接收脚）的GPIO配置为浮空输入模式														  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				  //选定哪个IO口 现选定PA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);                    //初始化GPIOA
	  
	//配置UART4参数
	USART_InitStructure.USART_BaudRate = 9600;	                    //波特率设置：9600
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	    //数据位数设置：8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	        //停止位设置：1位
	USART_InitStructure.USART_Parity = USART_Parity_No ;            //是否奇偶校验：无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //接收与发送都使能
	USART_Init(USART2, &USART_InitStructure);                       //初始化USART2
	
	//打开发送中断和接收中断(如果需要中断)
	//USART_ITConfig(UART4, USART_IT_TXE, ENABLE);  // 使能指定的USART2发送中断
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // 使能指定的USART2接收中断
	/* Configure the NVIC Preemption Priority Bits */  
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);//注意如果前面有了这个声明，这个就一定要屏蔽
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//注意串口1~3用的是USART1_IRQn\USART2_IRQn\USART3_IRQn\串口4~5用的是UART4_IRQn、UART5_IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//使能 UART4， 配置完毕
	USART_Cmd(USART2, ENABLE);                             // USART2使能
 
	//如下语句解决第1个字节无法正确发送出去的问题
  USART_ClearFlag(USART2, USART_FLAG_TC);                //清串口4发送标志
}

/*
 * 函数名：void usart4_SendByte(unsigned char SendData)
 * 描述  ：串口4发送字符子程序
 * 输入  ：SendData为输入数据
 * 输出  ：无
 * 调用  ：外、内部调用
 */
void usart2_SendByte(unsigned char SendData)	  //用串口2发送一个字节数据
{	   
	USART_SendData(USART2,SendData);   //用串口3发送一个字节
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);  //等待串口2发送完毕
}  
