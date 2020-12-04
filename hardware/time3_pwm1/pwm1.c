/******************** (C) COPYRIGHT 2015 ****************************************
 * �ļ���  ��pwm1.c---������ӳ��
 * ����    ��         
 * ʵ��ƽ̨��Ұ��STM32������
 * Ӳ�����ӣ�-
TIM2:
PA.00: (TIM2_CH1)//PA0û����
PA.01: (TIM2_CH2)
PA.02: (TIM2_CH3)//�ʹ���2��ͻ
PA.03: (TIM2_CH4)//�ʹ���2��ͻ

TIM3:
PB.04: (TIM3_CH1)
PB.05: (TIM3_CH2)
PB.00: (TIM3_CH3)
PB.01: (TIM3_CH4)
��汾  ��ST3.5.0
**********************************************************************************/
#include "pwm1.h"

/*
 * ��������TIM3_GPIO1_Config
 * ����  ������TIM3�������PWMʱ�õ���I/O
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
 static void TIM3_GPIO1_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* TIM3 clock enable */
	//PCLK1����2��Ƶ����ΪTIM3��ʱ��Դ����36MHz
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

  /* GPIOA and GPIOB clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
  
	/*TIM3������ӳ��Ŀ⺯��ʵ�ַ���*/
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); 
	
	//PB4 �����ؿڳ�ͻ�ˣ��������Բ��ò�����ӳ����
	//	//��Ҫ��ӳ�����swj��PB4���ܹ��������Ǽ�����仰֮�󣬾�û����sw�����ˣ���Ҫ����һ�仰�����׽��JTAG �� SW
	//��ϵͳ��λ��ʱ��Ĭ�Ͽ���JTAG��SWD��������¾仰��������غ�2���ڰ���λ�����ɽ��������ص�stm32
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST, ENABLE);//
	
	
  //���ø�����Ϊ�����������,���TIM3 CH1\CH2\CH3\CH4��PWM���岨��	GPIOB.0\GPIOB.1\GPIOB.4\GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
}

/*
 * ��������TIM3_Mode1_Config
 * ����  ������TIM3�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ�
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
static void TIM3_Mode1_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
  /* PWM�źŵ�ƽ����ֵ */
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
  TIM_TimeBaseStructure.TIM_Period = 999;       //����ʱ����0������4999����Ϊ5000�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Prescaler = 0;	    //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ36MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//����ʱ�ӷ�Ƶϵ��������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;	    //����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ

  TIM_OC1Init(TIM3, &TIM_OCInitStructure);	 //ʹ��ͨ��1

  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  //����ͨ��2�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM

  TIM_OC2Init(TIM3, &TIM_OCInitStructure);	  //ʹ��ͨ��2

  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	//����ͨ��3�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM

  TIM_OC3Init(TIM3, &TIM_OCInitStructure);	 //ʹ��ͨ��3

  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	//����ͨ��4�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM

  TIM_OC4Init(TIM3, &TIM_OCInitStructure);	//ʹ��ͨ��4

  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM3, ENABLE);			 // ʹ��TIM3���ؼĴ���ARR

  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);                   //ʹ�ܶ�ʱ��3	
}

/*
 * ��������void TIM3_PWM1_Init(void)
 * ����  ��TIM3 ���PWM�źų�ʼ����ֻҪ�����������
 *         TIM3���ĸ�ͨ���ͻ���PWM�ź����
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void TIM3_PWM1_Init(void)
{
	TIM3_GPIO1_Config();
	TIM3_Mode1_Config();	
}

/******************* (C) COPYRIGHT 2015 **********************************END OF FILE*****/
