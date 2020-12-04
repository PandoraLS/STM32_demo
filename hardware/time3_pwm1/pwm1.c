/******************** (C) COPYRIGHT 2015 ****************************************
 * 文件名  ：pwm1.c---部分重映像
 * 描述    ：         
 * 实验平台：野火STM32开发板
 * 硬件连接：-
TIM2:
PA.00: (TIM2_CH1)//PA0没法用
PA.01: (TIM2_CH2)
PA.02: (TIM2_CH3)//和串口2冲突
PA.03: (TIM2_CH4)//和串口2冲突

TIM3:
PB.04: (TIM3_CH1)
PB.05: (TIM3_CH2)
PB.00: (TIM3_CH3)
PB.01: (TIM3_CH4)
库版本  ：ST3.5.0
**********************************************************************************/
#include "pwm1.h"

/*
 * 函数名：TIM3_GPIO1_Config
 * 描述  ：配置TIM3复用输出PWM时用到的I/O
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
 static void TIM3_GPIO1_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* TIM3 clock enable */
	//PCLK1经过2倍频后作为TIM3的时钟源等于36MHz
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

  /* GPIOA and GPIOB clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
  
	/*TIM3部分重映射的库函数实现方法*/
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); 
	
	//PB4 和下载口冲突了，这里索性不用部分重映射了
	//	//需要重映像这个swj，PB4才能工作，但是加上这句话之后，就没法用sw下载了，需要下面一句话来彻底解放JTAG 和 SW
	//在系统复位的时候，默认开启JTAG和SWD，如果用下句话，点击下载后2秒内按复位键即可将程序下载到stm32
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST, ENABLE);//
	
	
  //设置该引脚为复用输出功能,输出TIM3 CH1\CH2\CH3\CH4的PWM脉冲波形	GPIOB.0\GPIOB.1\GPIOB.4\GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
}

/*
 * 函数名：TIM3_Mode1_Config
 * 描述  ：配置TIM3输出的PWM信号的模式，如周期、极性、占空比
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void TIM3_Mode1_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
  /* PWM信号电平跳变值 */
	u16 CCR1_Val = 125;        
	u16 CCR2_Val = 375;
	u16 CCR3_Val = 500;
	u16 CCR4_Val = 600;
/* -----------------------------------------------------------------------
    TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles:
    TIM3CLK = 72 MHz, Prescaler = 0x0, TIM3 counter clock = 72 MHz
    TIM3 ARR Register = 999 => TIM3 Frequency = TIM3 counter clock/(ARR + 1)
    TIM3 Frequency = 72 KHz.
    TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
    TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
    TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR)* 100 = 25%
    TIM3 Channel4 duty cycle = (TIM3_CCR4/ TIM3_ARR)* 100 = 12.5%
  ----------------------------------------------------------------------- */

  /* Time base configuration */		 
  TIM_TimeBaseStructure.TIM_Period = 999;       //当定时器从0计数到4999，即为5000次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Prescaler = 0;	    //设置预分频：不预分频，即为36MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//设置时钟分频系数：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;	    //配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;  //当定时器计数值小于CCR1_Val时为高电平

  TIM_OC1Init(TIM3, &TIM_OCInitStructure);	 //使能通道1

  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  //设置通道2的电平跳变值，输出另外一个占空比的PWM

  TIM_OC2Init(TIM3, &TIM_OCInitStructure);	  //使能通道2

  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	//设置通道3的电平跳变值，输出另外一个占空比的PWM

  TIM_OC3Init(TIM3, &TIM_OCInitStructure);	 //使能通道3

  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	//设置通道4的电平跳变值，输出另外一个占空比的PWM

  TIM_OC4Init(TIM3, &TIM_OCInitStructure);	//使能通道4

  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM3, ENABLE);			 // 使能TIM3重载寄存器ARR

  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);                   //使能定时器3	
}

/*
 * 函数名：void TIM3_PWM1_Init(void)
 * 描述  ：TIM3 输出PWM信号初始化，只要调用这个函数
 *         TIM3的四个通道就会有PWM信号输出
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void TIM3_PWM1_Init(void)
{
	TIM3_GPIO1_Config();
	TIM3_Mode1_Config();	
}

/******************* (C) COPYRIGHT 2015 **********************************END OF FILE*****/
