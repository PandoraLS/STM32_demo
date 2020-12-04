/******************** (C) COPYRIGHT 2016 ****************************************
 * �ļ���  ��time.c
 * ����    ����ʱ���Ͷ�ʱ���жϵ����� ��Ĭ�ϵĶ�ʱ����1s        
 * ʵ��ƽ̨��IPC9ѧϰ��  			
 * ��汾  ��ST3.0.0
 *
 * ����    ���
**********************************************************************************/
 /*
 * TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--71 
 * �ж�����Ϊ = 1/(72MHZ /72) * 1000 = 1ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> �ж� ��TIMxCNT����Ϊ0���¼��� 
 */

/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 *typedef struct
 *{  TIM_Prescaler         ����
 *	TIM_CounterMode			 TIMx,x[6,7]û�У���������
 *  TIM_Period            ����
 *  TIM_ClockDivision     TIMx,x[6,7]û�У���������
 *  TIM_RepetitionCounter TIMx,x[1,8,15,16,17]����
 *}TIM_TimeBaseInitTypeDef; 
 *-----------------------------------------------------------------------------
 */
#include "time.h"
#include "sys.h"

/*
 * ��������TIMER1_INIT(void)
 * ����  ��1s�ж�һ��
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void TIMER1_INIT(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	//����ṹ�����    
		NVIC_InitTypeDef NVIC_InitStructure; 
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 	//��ʱ��1��ʱ��ʹ��,ʹ��RCC_APB2,ϵͳʱ��Ƶ��Ϊ36Mhz
    TIM_TimeBaseStructure.TIM_Prescaler = 7199;	//psc����������ΪTIM4ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 36M/7200=5K //ʱ����㹫ʽ����TIM_Prescaler+1��/36M * (TIM_Period+1)
    TIM_TimeBaseStructure.TIM_Period = 999;	//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	  arr  5K/50=100Hz  10ms
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	//TIM���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_ClockDivision= TIM_CKD_DIV1;	//����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);	//����ָ���Ĳ�����ʼ��TIM4��ʱ�������λ     
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);	//���TIM4���ж�TIM_IT��־λ��TIM4������жϷ���������Ҫ����жϱ�־λ
    TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);	//ʹ��ָ����TIM4�ж�,��������ж�
    TIM_ARRPreloadConfig(TIM1, DISABLE);	//����TIM4��ΧARRԤװ�Ĵ���
    TIM_Cmd(TIM1,ENABLE); 
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn; 		//�ж�ԴTIM4      
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 1; //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�           
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;  //IRQͨ����ʹ��                  
	NVIC_Init(&NVIC_InitStructure); //��ʼ��NVIC�Ĵ���
}

/*
 * ��������TIMER2_INIT(void)
 * ����  ��1s�ж�һ��
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void TIMER2_INIT(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	//����ṹ�����    
		NVIC_InitTypeDef NVIC_InitStructure; 
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 	//��ʱ��4��ʱ��ʹ��
    TIM_TimeBaseStructure.TIM_Prescaler = 7199;	//psc����������ΪTIM4ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 72M/7200=10K //ʱ����㹫ʽ����TIM_Prescaler+1��/72M * (TIM_Period+1)
    TIM_TimeBaseStructure.TIM_Period = 4999;	//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	  arr  10K/10000=1Hz  1s
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	//TIM���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_ClockDivision= TIM_CKD_DIV1;	//����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	//����ָ���Ĳ�����ʼ��TIM4��ʱ�������λ     
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);	//���TIM4���ж�TIM_IT��־λ��TIM4������жϷ���������Ҫ����жϱ�־λ
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);	//ʹ��ָ����TIM4�ж�,��������ж�
    TIM_ARRPreloadConfig(TIM2, DISABLE);	//����TIM4��ΧARRԤװ�Ĵ���
    TIM_Cmd(TIM2,ENABLE); 
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 		//�ж�ԴTIM4      
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 1; //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�           
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;  //IRQͨ����ʹ��                  
	NVIC_Init(&NVIC_InitStructure); //��ʼ��NVIC�Ĵ���
}
/*
 * ��������TIMER3_INIT(void)
 * ����  ��1s�ж�һ��
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void TIMER3_INIT(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	//����ṹ�����    
		NVIC_InitTypeDef NVIC_InitStructure; 
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 	//��ʱ��4��ʱ��ʹ��
    TIM_TimeBaseStructure.TIM_Prescaler = 7199;	//psc����������ΪTIM4ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 72M/7200=10K //ʱ����㹫ʽ����TIM_Prescaler+1��/72M * (TIM_Period+1)
    TIM_TimeBaseStructure.TIM_Period = 4999;	//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	  arr  10K/10000=1Hz  1s
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	//TIM���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_ClockDivision= TIM_CKD_DIV1;	//����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);	//����ָ���Ĳ�����ʼ��TIM4��ʱ�������λ     
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);	//���TIM4���ж�TIM_IT��־λ��TIM4������жϷ���������Ҫ����жϱ�־λ
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);	//ʹ��ָ����TIM4�ж�,��������ж�
    TIM_ARRPreloadConfig(TIM3, DISABLE);	//����TIM4��ΧARRԤװ�Ĵ���
    TIM_Cmd(TIM3,ENABLE); 
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 		//�ж�ԴTIM4      
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 1; //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�           
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;  //IRQͨ����ʹ��                  
	NVIC_Init(&NVIC_InitStructure); //��ʼ��NVIC�Ĵ���
}

/*
 * ��������TIMER4_INIT(void)
 * ����  ��1s�ж�һ��
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void TIMER4_INIT(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	//����ṹ�����    
		NVIC_InitTypeDef NVIC_InitStructure; 
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 	//��ʱ��4��ʱ��ʹ��
    TIM_TimeBaseStructure.TIM_Prescaler = 7199;	//psc����������ΪTIM4ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 72M/7200=10K //ʱ����㹫ʽ����TIM_Prescaler+1��/72M * (TIM_Period+1)
    TIM_TimeBaseStructure.TIM_Period = 4999;	//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	  arr  10K/10000=1Hz  1s
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	//TIM���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_ClockDivision= TIM_CKD_DIV1;	//����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);	//����ָ���Ĳ�����ʼ��TIM4��ʱ�������λ     
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);	//���TIM4���ж�TIM_IT��־λ��TIM4������жϷ���������Ҫ����жϱ�־λ
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);	//ʹ��ָ����TIM4�ж�,��������ж�
    TIM_ARRPreloadConfig(TIM4, DISABLE);	//����TIM4��ΧARRԤװ�Ĵ���
    TIM_Cmd(TIM4,ENABLE); 
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; 		//�ж�ԴTIM4      
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 1; //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�           
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;  //IRQͨ����ʹ��                  
	NVIC_Init(&NVIC_InitStructure); //��ʼ��NVIC�Ĵ���
}


//TIMER5_INIT���ڴ������Ĳ���
/*
 * ��������TIMER5_INIT(void)
 * ����  ��1s�ж�һ��
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void TIMER5_INIT(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	//����ṹ�����    
		NVIC_InitTypeDef NVIC_InitStructure; 
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); 	//��ʱ��4��ʱ��ʹ��
    TIM_TimeBaseStructure.TIM_Prescaler = 7199;	//psc����������ΪTIM4ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 72M/7200=10K //ʱ����㹫ʽ����TIM_Prescaler+1��/72M * (TIM_Period+1)
    TIM_TimeBaseStructure.TIM_Period = 4999;	//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	  arr  10K/10000=1Hz  1s
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	//TIM���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_ClockDivision= TIM_CKD_DIV1;	//����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);	//����ָ���Ĳ�����ʼ��TIM4��ʱ�������λ     
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);	//���TIM4���ж�TIM_IT��־λ��TIM4������жϷ���������Ҫ����жϱ�־λ
    TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);	//ʹ��ָ����TIM4�ж�,��������ж�
    TIM_ARRPreloadConfig(TIM5, DISABLE);	//����TIM4��ΧARRԤװ�Ĵ���
    TIM_Cmd(TIM5,ENABLE); 
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn; 		//�ж�ԴTIM5     
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 1; //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�           
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;  //IRQͨ����ʹ��                  
	NVIC_Init(&NVIC_InitStructure); //��ʼ��NVIC�Ĵ���
}



//TIMER6_INIT���ڴ������Ĳ���
/*
 * ��������TIMER6_INIT(void)
 * ����  ��1s�ж�һ��
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void TIMER6_INIT(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	//����ṹ�����    
		NVIC_InitTypeDef NVIC_InitStructure; 
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); 	//��ʱ��4��ʱ��ʹ��
    TIM_TimeBaseStructure.TIM_Prescaler = 719;	//psc����������ΪTIM4ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 72M/7200=10K //ʱ����㹫ʽ����TIM_Prescaler+1��/72M * (TIM_Period+1)
    TIM_TimeBaseStructure.TIM_Period = 199;	//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	  arr  10K/10000=1Hz  1s
		
//		TIM_TimeBaseStructure.TIM_Prescaler = 7199;	//psc����������ΪTIM4ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 72M/7200=10K //ʱ����㹫ʽ����TIM_Prescaler+1��/72M * (TIM_Period+1)
//    TIM_TimeBaseStructure.TIM_Period = 29999;	//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	  arr  10K/10000=1Hz  1s
	
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	//TIM���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_ClockDivision= TIM_CKD_DIV1;	//����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);	//����ָ���Ĳ�����ʼ��TIM4��ʱ�������λ     
    TIM_ClearITPendingBit(TIM6, TIM_IT_Update);	//���TIM4���ж�TIM_IT��־λ��TIM4������жϷ���������Ҫ����жϱ�־λ
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);	//ʹ��ָ����TIM4�ж�,��������ж�
    TIM_ARRPreloadConfig(TIM6, DISABLE);	//����TIM4��ΧARRԤװ�Ĵ���
    TIM_Cmd(TIM6,ENABLE); 
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn; 		//�ж�ԴTIM5     
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 2; //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�           
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;  //IRQͨ����ʹ��                  
	NVIC_Init(&NVIC_InitStructure); //��ʼ��NVIC�Ĵ���
}


//TIMER7_INIT���ڴ������Ĳ���
/*
 * ��������TIMER7_INIT(void)
 * ����  ��1s�ж�һ��
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void TIMER7_INIT(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	//����ṹ�����    
		NVIC_InitTypeDef NVIC_InitStructure; 
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); 	//��ʱ��4��ʱ��ʹ��
    TIM_TimeBaseStructure.TIM_Prescaler = 7199;	//psc����������ΪTIM4ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 72M/7200=10K //ʱ����㹫ʽ����TIM_Prescaler+1��/72M * (TIM_Period+1)
    TIM_TimeBaseStructure.TIM_Period = 999;	//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	  arr  10K/10000=1Hz  1s
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	//TIM���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_ClockDivision= TIM_CKD_DIV1;	//����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);	//����ָ���Ĳ�����ʼ��TIM4��ʱ�������λ     
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);	//���TIM4���ж�TIM_IT��־λ��TIM4������жϷ���������Ҫ����жϱ�־λ
    TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);	//ʹ��ָ����TIM4�ж�,��������ж�
    TIM_ARRPreloadConfig(TIM7, DISABLE);	//����TIM4��ΧARRԤװ�Ĵ���
    TIM_Cmd(TIM7,ENABLE); 
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn; 		//�ж�ԴTIM5     
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 3; //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�           
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;  //IRQͨ����ʹ��                  
	NVIC_Init(&NVIC_InitStructure); //��ʼ��NVIC�Ĵ���
}


/******************* (C) COPYRIGHT 2016 **********************************END OF FILE*****/
