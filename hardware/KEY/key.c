/******************** (C) COPYRIGHT 2016 ****************************************
 * 文件名  ：key.c
 * 描述    ：矩阵按键4*2
 * 实验平台：IPC9学习板
 * 硬件连接                ------------------
 *                        |   矩阵按键       |
 *                        |  PE1--   PE5--   |
 *                        |  PE0--   PE6--   |
 *      	                 |  PE3--   PE7--   |
 *    		              |  PE4--   PE8--   |
 *                         ------------------ 			
 * 库版本  ：ST3.0.0
 *
 * 作者    ：李俊
**********************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "OLED.h"
#include "key.h"								    

/*
 * 函数名：void KEY_Matrix_Init(void)
 * 描述  ：矩阵按键初始化
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void KEY_Matrix_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);//使能PORTA,PORTE时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4; //输出高电平引脚配置
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOE, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8; //电平输入引脚配置
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	GPIO_Init(GPIOE, &GPIO_InitStructure);  
	GPIO_SetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4); //置为高电平
	GPIO_ResetBits(GPIOE, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8); //置为低电平
}

/*
 * 函数名：u8 Read_KeyValue_Matrix(void) 
 * 描述  ：矩阵按键处理
 * 输入  ：无
 * 输出  ：返回0~15的值，分别代表一个按下的按键
 * 调用  ：外部调用
 */

u8 Read_KeyValue_Matrix(void) 
{  
	u8 KeyValue=0;  
	if((GPIO_ReadInputData(GPIOE)&0x01FB)!=0x001B) 	//检查按键是否按下
	{  
		delay_ms(4*dey);  //消抖
		if((GPIO_ReadInputData(GPIOE)&0x01FB)!=0x001B) 	//消抖检查
		{  
			
			GPIO_SetBits(GPIOE, GPIO_Pin_0);  	//使能PE0
			GPIO_ResetBits(GPIOE, GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4); 	//失能PE1，PE3，PE4
			if(GPIO_ReadInputData(GPIOE)&0x01FB)
			{
				delay_ms(dey);		//消抖
				if(GPIO_ReadInputData(GPIOE)&0x01FB)
				{
					switch(GPIO_ReadInputData(GPIOE)&0x01FB) 	//扫描PE5，PE6，读取键值
					{  
						case 0x0021: KeyValue = 2; break; 
						case 0x0041: KeyValue = 6; break; 
						case 0x0081: KeyValue = 10;break; 
						case 0x0101: KeyValue = 14;break; 
						default : break;
					}
				}
			}
			
			GPIO_SetBits(GPIOE, GPIO_Pin_1);  
			GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_4); 
			if(GPIO_ReadInputData(GPIOE)&0x01FB)
			{
				delay_ms(dey);		//消抖
				if(GPIO_ReadInputData(GPIOE)&0x01FB)
				{
					switch(GPIO_ReadInputData(GPIOE)&0x01FB) 
					{  
						case 0x0022: KeyValue = 1; break; 
						case 0x0042: KeyValue = 5; break; 
						case 0x0082: KeyValue = 9; break; 
						case 0x0102: KeyValue = 13;break; 
						default : break;
					}					
				}
			}			 
			
			GPIO_SetBits(GPIOE, GPIO_Pin_3);  
			GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4); 
			if(GPIO_ReadInputData(GPIOE)&0x01FB)
			{
				delay_ms(dey);		//消抖
				if(GPIO_ReadInputData(GPIOE)&0x01FB)
				{
					switch(GPIO_ReadInputData(GPIOE)&0x01FB) 
					{  
						case 0x0028: KeyValue = 3; break; 
						case 0x0048: KeyValue = 7; break; 
						case 0x0088: KeyValue = 11;break; 
						case 0x0108: KeyValue = 15;break; 
						default : break;
					}  
				}
			}			
			
			GPIO_SetBits(GPIOE, GPIO_Pin_4);  
			GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3); 
			if(GPIO_ReadInputData(GPIOE)&0x01FB)
			{
				delay_ms(dey);		//消抖
				if(GPIO_ReadInputData(GPIOE)&0x01FB)
				{
					switch(GPIO_ReadInputData(GPIOE)&0x01FB) 
					{  
						case 0x0030: KeyValue = 4; break; 
						case 0x0050: KeyValue = 8; break; 
						case 0x0090: KeyValue = 12;break; 
						case 0x0110: KeyValue = 16;break;
						default : break;				
					}
				}
			}			  
			
			GPIO_SetBits(GPIOE, GPIO_Pin_1 | GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_4); 
			GPIO_ResetBits(GPIOE, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8);  
			while((GPIO_ReadInputData(GPIOE)&0x01FB)!=0x001B)
			{
				return KeyValue;;
			}
		} 
	} 
	if(KeyValue==0)
	{
		return 0;
	}
	else
	{
		return KeyValue;
	}
}

void Key_Value_Show()
{
	int a;
	a=Read_KeyValue_Matrix();
		if(a!=0)
		{
			OLED_6x8var_ch(0,0,a);
		}
}


/******************* (C) COPYRIGHT 2016 **********************************END OF FILE*****/
