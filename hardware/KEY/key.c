/******************** (C) COPYRIGHT 2016 ****************************************
 * �ļ���  ��key.c
 * ����    �����󰴼�4*2
 * ʵ��ƽ̨��IPC9ѧϰ��
 * Ӳ������                ------------------
 *                        |   ���󰴼�       |
 *                        |  PE1--   PE5--   |
 *                        |  PE0--   PE6--   |
 *      	                 |  PE3--   PE7--   |
 *    		              |  PE4--   PE8--   |
 *                         ------------------ 			
 * ��汾  ��ST3.0.0
 *
 * ����    ���
**********************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "OLED.h"
#include "key.h"								    

/*
 * ��������void KEY_Matrix_Init(void)
 * ����  �����󰴼���ʼ��
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void KEY_Matrix_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);//ʹ��PORTA,PORTEʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4; //����ߵ�ƽ��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOE, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8; //��ƽ������������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	GPIO_Init(GPIOE, &GPIO_InitStructure);  
	GPIO_SetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4); //��Ϊ�ߵ�ƽ
	GPIO_ResetBits(GPIOE, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8); //��Ϊ�͵�ƽ
}

/*
 * ��������u8 Read_KeyValue_Matrix(void) 
 * ����  �����󰴼�����
 * ����  ����
 * ���  ������0~15��ֵ���ֱ����һ�����µİ���
 * ����  ���ⲿ����
 */

u8 Read_KeyValue_Matrix(void) 
{  
	u8 KeyValue=0;  
	if((GPIO_ReadInputData(GPIOE)&0x01FB)!=0x001B) 	//��鰴���Ƿ���
	{  
		delay_ms(4*dey);  //����
		if((GPIO_ReadInputData(GPIOE)&0x01FB)!=0x001B) 	//�������
		{  
			
			GPIO_SetBits(GPIOE, GPIO_Pin_0);  	//ʹ��PE0
			GPIO_ResetBits(GPIOE, GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4); 	//ʧ��PE1��PE3��PE4
			if(GPIO_ReadInputData(GPIOE)&0x01FB)
			{
				delay_ms(dey);		//����
				if(GPIO_ReadInputData(GPIOE)&0x01FB)
				{
					switch(GPIO_ReadInputData(GPIOE)&0x01FB) 	//ɨ��PE5��PE6����ȡ��ֵ
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
				delay_ms(dey);		//����
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
				delay_ms(dey);		//����
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
				delay_ms(dey);		//����
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
