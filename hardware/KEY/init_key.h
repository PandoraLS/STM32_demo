#ifndef __INIT_KEY_H
#define __INIT_KEY_H	 
#include "stm32f10X.h"

/*独立按键相关端口配置*/
#define KYE_GPOOX GPIOA//按键端口设置
//PA9,PA10为串口1的引脚，PA11为TIM1_CH4接口
//PA11，PA12不能使用
#define KYE_GPOOX_Pin1 GPIO_Pin_11//按键引脚设置
#define KYE_GPOOX_Pin2 GPIO_Pin_12//按键引脚设置
#define KYE_GPOOX_Pin3 GPIO_Pin_15//按键引脚设置
#define KYE_GPOOX_RCC RCC_APB2Periph_GPIOA//按键时钟初始化

//PB3,PB5不能使用，PB13，PB14,PB15能用
#define KYE_GPOOY GPIOB//按键端口设置
#define KYE_GPOOY_Pin1 GPIO_Pin_3//按键引脚设置
#define KYE_GPOOY_Pin2 GPIO_Pin_4//按键引脚设置
#define KYE_GPOOY_Pin3 GPIO_Pin_5//按键引脚设置
#define KYE_GPOOY_RCC RCC_APB2Periph_GPIOB//按键时钟初始化

#define KEY1  GPIO_ReadInputDataBit(KYE_GPOOX,KYE_GPOOX_Pin1)//读取按键0
#define KEY2  GPIO_ReadInputDataBit(KYE_GPOOX,KYE_GPOOX_Pin2)//读取按键1
#define KEY3  GPIO_ReadInputDataBit(KYE_GPOOX,KYE_GPOOX_Pin3)//读取按键2 

#define KEY4  GPIO_ReadInputDataBit(KYE_GPOOY,KYE_GPOOY_Pin1)//读取按键1
#define KEY5  GPIO_ReadInputDataBit(KYE_GPOOY,KYE_GPOOY_Pin2)//读取按键2 
#define KEY6  GPIO_ReadInputDataBit(KYE_GPOOY,KYE_GPOOY_Pin3)//读取按键3(WK_UP) 


#define KEY_6 		6//返回键值
#define KEY_5	    5//返回键值
#define KEY_4 		4//返回键值
#define KEY_3	    3//返回键值
#define KEY_2    	2//返回键值
#define KEY_1    	1//返回键值

/*独立按键*/
void KEY_Init(void);//IO初始化
u8 KEY_Scan(u8);  	//按键扫描函数(传入非零值进行连续按键)		
void key_show(void);		//显示按键键值

#endif
