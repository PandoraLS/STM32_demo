/******************** (C) COPYRIGHT 2015 ****************************************
�ļ���  ��init_key.c
����    ���ʺ�������������
ʵ��ƽ̨��IPC9ѧϰ��
Ӳ�����ӣ�����init_key�н�������
#define KEY_6 		PB5//���ؼ�ֵ
#define KEY_5	    PB4//���ؼ�ֵ
#define KEY_4 		PB3//���ؼ�ֵ
#define KEY_3	    PA15//���ؼ�ֵ
#define KEY_2    	PA12//���ؼ�ֵ
#define KEY_1    	PA11//���ؼ�ֵ
��汾  ��ST3.5.0
���ߣ�Pandora
**********************************************************************************/
#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "init_key.h"
#include "OLED.h"
//#include "oled_1.h"

/*
 * ��������void KEY_Init(void)
 * ����  ������������ʼ��
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
 	RCC_APB2PeriphClockCmd(KYE_GPOOX_RCC,ENABLE);//ʹ��PORTA,PORTEʱ��
	RCC_APB2PeriphClockCmd(KYE_GPOOY_RCC,ENABLE);//ʹ��PORTB,PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = KYE_GPOOX_Pin1|KYE_GPOOX_Pin2|KYE_GPOOX_Pin3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(KYE_GPOOX, &GPIO_InitStructure);//��ʼ��
	
	
	//��ӳ������IO��PA13 PA14 ������           PA15 PB3 PB4����
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin  = KYE_GPOOY_Pin1|KYE_GPOOY_Pin2|KYE_GPOOY_Pin3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(KYE_GPOOY, &GPIO_InitStructure);//��ʼ��GPIOB 3,4,5
	

}

/*
 * ��������u8 KEY_Scan(u8 mode)
 * ����  ��������������(ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3!!)
 * ����  ��mode:0,��֧��������;1,֧��������;
 * ���  ��0��û���κΰ�������
           1��KEY0����
           2��KEY1����
           3��KEY2���� 
           4��KEY3���� 
 * ����  ���ⲿ����
 */
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)	
		key_up=1;  //֧������		  
	if(key_up&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0||KEY5==0||KEY6==0))
	{
		delay_ms(10);	//ȥ���� 
		if(key_up&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0||KEY5==0||KEY6==0))
		{
			key_up=0;
			if(KEY1==0)return 1;
			else if(KEY2==0)return 2;
			else if(KEY3==0)return 3;
			else if(KEY4==0)return 4;
			else if(KEY5==0)return 5;
			else if(KEY6==0)return 6;
		}
	} 
	else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1&&KEY5==1&&KEY6==1)
		key_up=1; 	    
 	return 0;// �ް�������
}

/*
 * ��������void key_show()
 * ����  ������������ʾ��ֵ
 * ����  ����
 * ���  ���� 
 * ����  ���ⲿ����
 */
void key_show()
{
	int KEY_Back;
	KEY_Back=KEY_Scan(1);
		if(KEY_Back!=0)
		{
//			LCD_8x16var_int(0,1,KEY_Back);//LDC��ʾ
		}
}




/******************* (C) COPYRIGHT 2016 **********************************END OF FILE*****/
