/******************** (C) COPYRIGHT 2015 ****************************************
 * 文件名  ：zigbee.c
 * 描述    ：         
 * 实验平台：ipc9学习板
 * 硬件连接：---------------------
 *          |  PB.10---TXD(蓝牙)  |
 *          |  PB.11---RXD(蓝牙)  |
 * 库版本  ：ST3.5.0
 *
**********************************************************************************/
#include "zigbee.h"
/*
 * 函数名：void Zigbee_usart3(void)
 * 描述  ：配置USART3,配制成波特率为115200，接受中断
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void Zigbee_usart3(void)
{  
	GPIO_InitTypeDef GPIO_InitStructure;	  //串口端口配置结构体变量
	USART_InitTypeDef USART_InitStructure;	  //串口参数配置结构体变量
	NVIC_InitTypeDef    NVIC_InitStructure; 					   //定义用于配置中断的结构体变量

	//使能 USART3 时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//打开串口复用时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   //打开PB端口时钟
  	
	//将USART3 Tx（发送脚）的GPIO配置为推挽复用模式   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				  //选定哪个IO口 现选定PB10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //设定IO口的输出速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);    				  //初始化GPIOB
	
	//将USART3 Rx（接收脚）的GPIO配置为浮空输入模式														  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				  //选定哪个IO口 现选定PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);                    //初始化GPIOA
	  
	//配置USART3参数
	USART_InitStructure.USART_BaudRate = 9600;	                    //波特率设置：115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	    //数据位数设置：8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	        //停止位设置：1位
	USART_InitStructure.USART_Parity = USART_Parity_No ;            //是否奇偶校验：无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //接收与发送都使能
	USART_Init(USART3, &USART_InitStructure);                       //初始化USART3
	
	// USART3中断优先级配置
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                //中断优先级分组  抢占式优先级别设置为2位；响应优先级占2位
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;			   //指定中断源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	   //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;             //指定响应优先级别1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	               //打开中断
	NVIC_Init(&NVIC_InitStructure);							       //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

    //打开发送中断和接收中断(如果需要中断)
	//USART_ITConfig(USART3, USART_IT_TXE, ENABLE);  // 使能指定的USART3发送中断
  	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // 使能指定的USART3接收中断

	//使能 USART3， 配置完毕
	USART_Cmd(USART3, ENABLE);                             // USART3使能

	//如下语句解决第1个字节无法正确发送出去的问题
   USART_ClearFlag(USART3, USART_FLAG_TC);                //清串口3发送标志
}

/*
 * 函数名：void usart3_SendByte(unsigned char SendData)
 * 描述  ：串口3发送字符子程序
 * 输入  ：SendData为输入数据
 * 输出  ：无
 * 调用  ：外、内部调用
 */
void usart3_SendByte(unsigned char SendData)	  //用串口3发送一个字节数据
{	   
	USART_SendData(USART3,SendData);   //用串口3发送一个字节
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);  //等待串口3发送完毕
}  
