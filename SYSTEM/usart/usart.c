/******************** (C) COPYRIGHT 2016 ****************************************
文件名  ：key.c
描述    ：串口1~5
实验平台：IPC9学习板
硬件连接        
------------------------------------
|   串口1：  RX--PA10   TX--PA09   |
|   串口2：  RX--PA03   TX--PA02   |
|   串口3：  RX--PB11   TX--PB10   |
|   串口4：  RX--PC11   TX--PC10   |
|   串口5：  RX--PD02   TX--PC12   |
------------------------------------			
库版本  ：ST3.5.0
 
**********************************************************************************/
#include "usart.h"
#include "stdio.h"
 

//串口1初始化
void USART1_Config(void)		//初始化 配置USART1
{
	GPIO_InitTypeDef     GPIO_InitStructure;   //串口端口配置结构体变量
	USART_InitTypeDef    USART_InitStructure;  //串口参数配置结构体变量
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//第1步：打开GPIO和USART部件的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);	 //打开GPIOA时钟和GPIOA复用时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//打开串口复用时钟
	USART_DeInit(USART1);  //复位串口1

	//第2步：将USART1 Tx（发送脚）的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			   //串口1发送脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		   //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   //输出速度50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);				   //初始化GPIOA
														  
	//第3步：将USART Rx（接收脚）的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			   //串口1接收脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);				   //初始化GPIOA

	//第4步：配置USART1参数
	USART_InitStructure.USART_BaudRate             = 115200;							 //波特率设置：115200
	USART_InitStructure.USART_WordLength           = USART_WordLength_8b;			 //数据位数设置：8位
	USART_InitStructure.USART_StopBits             = USART_StopBits_1;				 //停止位设置：1位
	USART_InitStructure.USART_Parity               = USART_Parity_No;				 //是否奇偶校验：无
	USART_InitStructure.USART_HardwareFlowControl  = USART_HardwareFlowControl_None; //硬件流控制模式设置：没有使能
	USART_InitStructure.USART_Mode                 = USART_Mode_Rx | USART_Mode_Tx;	 //接收与发送都使能
	USART_Init(USART1, &USART_InitStructure);	

//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);//初始化USART1

    //打开发送中断和接收中断(如果需要中断)
//	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);  // 使能指定的USART1发送中断 ；
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // 使能指定的USART1接收中断 ；

	//第5步：使能 USART1， 配置完毕
	USART_Cmd(USART1, ENABLE);							   //使能 USART1

    //如下语句解决第1个字节无法正确发送出去的问题
    USART_ClearFlag(USART1, USART_FLAG_TC);                //清串口1发送标志
	
}

//串口2初始化
void USART2_Config(void)   //初始化 配置USART2
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	/* config USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* 使能串口1接收中断 */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART2, ENABLE);
}

//串口3初始化
void USART3_Config(void)   //初始化 配置USART3
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
void UART4_Config(void)   //初始化 配置UART4 串口4的uart比usart少一个s
{
	GPIO_InitTypeDef GPIO_InitStructure;	  //串口端口配置结构体变量
	USART_InitTypeDef USART_InitStructure;	  //串口参数配置结构体变量
	NVIC_InitTypeDef    NVIC_InitStructure; 					   //定义用于配置中断的结构体变量

	//使能 USART3 时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	//打开串口复用时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);   //打开PB端口时钟
  	
	//将USART3 Tx（发送脚）的GPIO配置为推挽复用模式   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				  //选定哪个IO口 现选定PB10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //设定IO口的输出速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);    				  //初始化GPIOB
	
	//将USART3 Rx（接收脚）的GPIO配置为浮空输入模式														  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				  //选定哪个IO口 现选定PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //浮空输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);                    //初始化GPIOA
	  
	//配置USART3参数
	USART_InitStructure.USART_BaudRate = 9600;	                    //波特率设置：115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	    //数据位数设置：8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	        //停止位设置：1位
	USART_InitStructure.USART_Parity = USART_Parity_No ;            //是否奇偶校验：无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //接收与发送都使能
	USART_Init(UART4, &USART_InitStructure);                       //初始化USART3
	
	// USART3中断优先级配置
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                //中断优先级分组  抢占式优先级别设置为2位；响应优先级占2位
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;			   //指定中断源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	   //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;             //指定响应优先级别1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	               //打开中断
	NVIC_Init(&NVIC_InitStructure);							       //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

    //打开发送中断和接收中断(如果需要中断)
	//USART_ITConfig(USART3, USART_IT_TXE, ENABLE);  // 使能指定的USART3发送中断
  	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE); // 使能指定的USART3接收中断

	//使能 USART3， 配置完毕
	USART_Cmd(UART4, ENABLE);                             // USART3使能

	//如下语句解决第1个字节无法正确发送出去的问题
    USART_ClearFlag(UART4, USART_FLAG_TC);                //清串口3发送标志
}
void UART5_Config(void)   //初始化 配置UART4 串口4的uart比usart少一个s
{
	GPIO_InitTypeDef GPIO_InitStructure;	  //串口端口配置结构体变量
	USART_InitTypeDef USART_InitStructure;	  //串口参数配置结构体变量
	NVIC_InitTypeDef    NVIC_InitStructure; 					   //定义用于配置中断的结构体变量

	//使能 USART3 时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);	//打开串口复用时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);   //打开PB端口时钟
  	
	//将USART3 Tx（发送脚）的GPIO配置为推挽复用模式   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				  //选定哪个IO口 现选定PB10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //设定IO口的输出速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);    				  //初始化GPIOB
	
	//将USART3 Rx（接收脚）的GPIO配置为浮空输入模式														  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				  //选定哪个IO口 现选定PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //浮空输入
	GPIO_Init(GPIOD, &GPIO_InitStructure);                    //初始化GPIOA
	  
	//配置USART3参数
	USART_InitStructure.USART_BaudRate = 9600;	                    //波特率设置：115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	    //数据位数设置：8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	        //停止位设置：1位
	USART_InitStructure.USART_Parity = USART_Parity_No ;            //是否奇偶校验：无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //接收与发送都使能
	USART_Init(UART5, &USART_InitStructure);                       //初始化USART3
	
	// USART3中断优先级配置
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                //中断优先级分组  抢占式优先级别设置为2位；响应优先级占2位
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;			   //指定中断源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	   //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;             //指定响应优先级别1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	               //打开中断
	NVIC_Init(&NVIC_InitStructure);							       //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

    //打开发送中断和接收中断(如果需要中断)
	//USART_ITConfig(USART3, USART_IT_TXE, ENABLE);  // 使能指定的USART3发送中断
  	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE); // 使能指定的USART3接收中断

	//使能 USART3， 配置完毕
	USART_Cmd(UART5, ENABLE);                             // USART3使能

	//如下语句解决第1个字节无法正确发送出去的问题
    USART_ClearFlag(UART5, USART_FLAG_TC);                //清串口3发送标志
}
 //重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
		USART_SendData(USART1, (uint8_t) ch);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
		return (ch);
}
/// 重定向c库函数scanf到USART1
int fgetc(FILE *f)
{
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}


void USART1_SendChar(uint16_t ch)
{
	/* Check the parameters */
	//assert_param(IS_USART_ALL_PERIPH(USARTx));
	//assert_param(IS_USART_DATA(Data)); 

	/* Transmit Data */
	USART1->DR = (ch & (uint16_t)0x01FF);
	
	/* 等待发送完毕 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	
}

void USART2_SendChar(uint16_t ch)
{
	/* Check the parameters */
	//assert_param(IS_USART_ALL_PERIPH(USARTx));
	//assert_param(IS_USART_DATA(Data)); 

	/* Transmit Data */
	USART1->DR = (ch & (uint16_t)0x01FF);
	
	/* 等待发送完毕 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	
}

void UART0_Oscilloscope(u32 Data , u8 channel)	//串口示波器
{	
	unsigned char ch_Temp = 0;
	unsigned char Temp_A_L;									
	signed char Temp_A_H;
	signed char Temp_B_L;
	signed char Temp_B_H;
	
	Temp_A_L = Data;
	Temp_A_H = Data >> 8;
	Temp_B_L = Data >> 16;
	Temp_B_H = Data >> 24;
	
	USART2_SendChar(0xaa);
	USART2_SendChar(0x09);
	/**/
	USART2_SendChar(channel);	

	ch_Temp ^= 0x09;
	ch_Temp ^= channel;
	ch_Temp ^= Temp_B_H;
	ch_Temp ^= Temp_B_L;
	ch_Temp ^= Temp_A_H;
	ch_Temp ^= Temp_A_L;//


	USART2_SendChar(Temp_A_L);
	USART2_SendChar(Temp_A_H);
	USART2_SendChar(Temp_B_L);
	USART2_SendChar(Temp_B_H);

	USART2_SendChar(ch_Temp);
	USART2_SendChar(0x55);
}

void USART1_printf (char *fmt, ...) 
{ 
	char buffer[CMD_BUFFER_LEN+1];  // CMD_BUFFER_LEN长度 
	u8 i = 0; 

	va_list arg_ptr; 
	va_start(arg_ptr, fmt);   
	vsnprintf(buffer, CMD_BUFFER_LEN+1, fmt, arg_ptr); 
	
	while ((i < CMD_BUFFER_LEN) && buffer[i]) 
	{ 
		USART_SendData(USART1, (u8) buffer[i++]); 
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);  
	} 
	va_end(arg_ptr); 
} 
void USART2_printf (char *fmt, ...) 
{ 
	char buffer[CMD_BUFFER_LEN+1];  // CMD_BUFFER_LEN长度 
	u8 i = 0; 

	va_list arg_ptr; 
	va_start(arg_ptr, fmt);   
	vsnprintf(buffer, CMD_BUFFER_LEN+1, fmt, arg_ptr); 
	
	while ((i < CMD_BUFFER_LEN) && buffer[i]) 
	{ 
		USART_SendData(USART2, (u8) buffer[i++]); 
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);  
	} 
	va_end(arg_ptr); 
} 
void USART3_printf (char *fmt, ...) 
{ 
	char buffer[CMD_BUFFER_LEN+1];  // CMD_BUFFER_LEN长度 
	u8 i = 0; 

	va_list arg_ptr; 
	va_start(arg_ptr, fmt);   
	vsnprintf(buffer, CMD_BUFFER_LEN+1, fmt, arg_ptr); 
	
	while ((i < CMD_BUFFER_LEN) && buffer[i]) 
	{ 
		USART_SendData(USART3, (u8) buffer[i++]); 
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);  
	} 
	va_end(arg_ptr); 
} 
void UART4_printf (char *fmt, ...) 
{ 
	char buffer[CMD_BUFFER_LEN+1];  // CMD_BUFFER_LEN长度 
	u8 i = 0; 

	va_list arg_ptr; 
	va_start(arg_ptr, fmt);   
	vsnprintf(buffer, CMD_BUFFER_LEN+1, fmt, arg_ptr); 
	
	while ((i < CMD_BUFFER_LEN) && buffer[i]) 
	{ 
		USART_SendData(UART4, (u8) buffer[i++]); 
		while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);  
	} 
	va_end(arg_ptr); 
} 
void UART5_printf (char *fmt, ...) 
{ 
	char buffer[CMD_BUFFER_LEN+1];  // CMD_BUFFER_LEN长度 
	u8 i = 0; 

	va_list arg_ptr; 
	va_start(arg_ptr, fmt);   
	vsnprintf(buffer, CMD_BUFFER_LEN+1, fmt, arg_ptr); 
	
	while ((i < CMD_BUFFER_LEN) && buffer[i]) 
	{ 
		USART_SendData(UART5, (u8) buffer[i++]); 
		while (USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET);  
	} 
	va_end(arg_ptr); 
} 
