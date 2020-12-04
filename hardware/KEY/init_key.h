#ifndef __INIT_KEY_H
#define __INIT_KEY_H	 
#include "stm32f10X.h"

/*����������ض˿�����*/
#define KYE_GPOOX GPIOA//�����˿�����
//PA9,PA10Ϊ����1�����ţ�PA11ΪTIM1_CH4�ӿ�
//PA11��PA12����ʹ��
#define KYE_GPOOX_Pin1 GPIO_Pin_11//������������
#define KYE_GPOOX_Pin2 GPIO_Pin_12//������������
#define KYE_GPOOX_Pin3 GPIO_Pin_15//������������
#define KYE_GPOOX_RCC RCC_APB2Periph_GPIOA//����ʱ�ӳ�ʼ��

//PB3,PB5����ʹ�ã�PB13��PB14,PB15����
#define KYE_GPOOY GPIOB//�����˿�����
#define KYE_GPOOY_Pin1 GPIO_Pin_3//������������
#define KYE_GPOOY_Pin2 GPIO_Pin_4//������������
#define KYE_GPOOY_Pin3 GPIO_Pin_5//������������
#define KYE_GPOOY_RCC RCC_APB2Periph_GPIOB//����ʱ�ӳ�ʼ��

#define KEY1  GPIO_ReadInputDataBit(KYE_GPOOX,KYE_GPOOX_Pin1)//��ȡ����0
#define KEY2  GPIO_ReadInputDataBit(KYE_GPOOX,KYE_GPOOX_Pin2)//��ȡ����1
#define KEY3  GPIO_ReadInputDataBit(KYE_GPOOX,KYE_GPOOX_Pin3)//��ȡ����2 

#define KEY4  GPIO_ReadInputDataBit(KYE_GPOOY,KYE_GPOOY_Pin1)//��ȡ����1
#define KEY5  GPIO_ReadInputDataBit(KYE_GPOOY,KYE_GPOOY_Pin2)//��ȡ����2 
#define KEY6  GPIO_ReadInputDataBit(KYE_GPOOY,KYE_GPOOY_Pin3)//��ȡ����3(WK_UP) 


#define KEY_6 		6//���ؼ�ֵ
#define KEY_5	    5//���ؼ�ֵ
#define KEY_4 		4//���ؼ�ֵ
#define KEY_3	    3//���ؼ�ֵ
#define KEY_2    	2//���ؼ�ֵ
#define KEY_1    	1//���ؼ�ֵ

/*��������*/
void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(u8);  	//����ɨ�躯��(�������ֵ������������)		
void key_show(void);		//��ʾ������ֵ

#endif
