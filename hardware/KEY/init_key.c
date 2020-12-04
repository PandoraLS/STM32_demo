/******************** (C) COPYRIGHT 2015 ****************************************
文件名  ：init_key.c
描述    ：适合六个独立按键
实验平台：IPC9学习板
硬件连接：可在init_key中进行设置
#define KEY_6 		PB5//返回键值
#define KEY_5	    PB4//返回键值
#define KEY_4 		PB3//返回键值
#define KEY_3	    PA15//返回键值
#define KEY_2    	PA12//返回键值
#define KEY_1    	PA11//返回键值
库版本  ：ST3.5.0
作者：Pandora
**********************************************************************************/
#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "init_key.h"
#include "OLED.h"
//#include "oled_1.h"

/*
 * 函数名：void KEY_Init(void)
 * 描述  ：独立按键初始化
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
 	RCC_APB2PeriphClockCmd(KYE_GPOOX_RCC,ENABLE);//使能PORTA,PORTE时钟
	RCC_APB2PeriphClockCmd(KYE_GPOOY_RCC,ENABLE);//使能PORTB,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = KYE_GPOOX_Pin1|KYE_GPOOX_Pin2|KYE_GPOOX_Pin3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(KYE_GPOOX, &GPIO_InitStructure);//初始化
	
	
	//重映射结果是IO：PA13 PA14 不能用           PA15 PB3 PB4能用
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin  = KYE_GPOOY_Pin1|KYE_GPOOY_Pin2|KYE_GPOOY_Pin3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(KYE_GPOOY, &GPIO_InitStructure);//初始化GPIOB 3,4,5
	

}

/*
 * 函数名：u8 KEY_Scan(u8 mode)
 * 描述  ：独立按键处理(注意此函数有响应优先级,KEY0>KEY1>KEY2>KEY3!!)
 * 输入  ：mode:0,不支持连续按;1,支持连续按;
 * 输出  ：0，没有任何按键按下
           1，KEY0按下
           2，KEY1按下
           3，KEY2按下 
           4，KEY3按下 
 * 调用  ：外部调用
 */
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)	
		key_up=1;  //支持连按		  
	if(key_up&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0||KEY5==0||KEY6==0))
	{
		delay_ms(10);	//去抖动 
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
 	return 0;// 无按键按下
}

/*
 * 函数名：void key_show()
 * 描述  ：独立按键显示键值
 * 输入  ：无
 * 输出  ：无 
 * 调用  ：外部调用
 */
void key_show()
{
	int KEY_Back;
	KEY_Back=KEY_Scan(1);
		if(KEY_Back!=0)
		{
//			LCD_8x16var_int(0,1,KEY_Back);//LDC显示
		}
}




/******************* (C) COPYRIGHT 2016 **********************************END OF FILE*****/
