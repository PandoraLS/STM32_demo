/******************** (C) COPYRIGHT 2016 ****************************************
 * 文件名  ：time.c
 * 描述    ：定时器和定时器中断的配置 ，默认的定时都是1s        
 * 实验平台：IPC9学习板  			
 * 库版本  ：ST3.0.0
 *
 * 作者    ：李俊
**********************************************************************************/
 /*
 * TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--71 
 * 中断周期为 = 1/(72MHZ /72) * 1000 = 1ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> 中断 且TIMxCNT重置为0重新计数 
 */

/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 *typedef struct
 *{  TIM_Prescaler         都有
 *	TIM_CounterMode			 TIMx,x[6,7]没有，其他都有
 *  TIM_Period            都有
 *  TIM_ClockDivision     TIMx,x[6,7]没有，其他都有
 *  TIM_RepetitionCounter TIMx,x[1,8,15,16,17]才有
 *}TIM_TimeBaseInitTypeDef; 
 *-----------------------------------------------------------------------------
 */
#include "time.h"
#include "sys.h"

/*
 * 函数名：TIMER1_INIT(void)
 * 描述  ：1s中断一次
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void TIMER1_INIT(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	//定义结构体变量    
		NVIC_InitTypeDef NVIC_InitStructure; 
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 	//定时器1的时钟使能,使用RCC_APB2,系统时间频率为36Mhz
    TIM_TimeBaseStructure.TIM_Prescaler = 7199;	//psc设置用来作为TIM4时钟频率除数的预分频值 36M/7200=5K //时间计算公式：（TIM_Prescaler+1）/36M * (TIM_Period+1)
    TIM_TimeBaseStructure.TIM_Period = 999;	//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	  arr  5K/50=100Hz  10ms
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	//TIM向上计数模式
    TIM_TimeBaseStructure.TIM_ClockDivision= TIM_CKD_DIV1;	//设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);	//根据指定的参数初始化TIM4的时间基数单位     
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);	//清除TIM4的中断TIM_IT标志位，TIM4的溢出中断发生后，我们要清除中断标志位
    TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);	//使能指定的TIM4中断,允许更新中断
    TIM_ARRPreloadConfig(TIM1, DISABLE);	//禁用TIM4外围ARR预装寄存器
    TIM_Cmd(TIM1,ENABLE); 
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn; 		//中断源TIM4      
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 1; //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级           
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;  //IRQ通道被使能                  
	NVIC_Init(&NVIC_InitStructure); //初始化NVIC寄存器
}

/*
 * 函数名：TIMER2_INIT(void)
 * 描述  ：1s中断一次
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void TIMER2_INIT(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	//定义结构体变量    
		NVIC_InitTypeDef NVIC_InitStructure; 
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 	//定时器4的时钟使能
    TIM_TimeBaseStructure.TIM_Prescaler = 7199;	//psc设置用来作为TIM4时钟频率除数的预分频值 72M/7200=10K //时间计算公式：（TIM_Prescaler+1）/72M * (TIM_Period+1)
    TIM_TimeBaseStructure.TIM_Period = 4999;	//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	  arr  10K/10000=1Hz  1s
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	//TIM向上计数模式
    TIM_TimeBaseStructure.TIM_ClockDivision= TIM_CKD_DIV1;	//设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	//根据指定的参数初始化TIM4的时间基数单位     
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);	//清除TIM4的中断TIM_IT标志位，TIM4的溢出中断发生后，我们要清除中断标志位
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);	//使能指定的TIM4中断,允许更新中断
    TIM_ARRPreloadConfig(TIM2, DISABLE);	//禁用TIM4外围ARR预装寄存器
    TIM_Cmd(TIM2,ENABLE); 
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 		//中断源TIM4      
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 1; //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级           
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;  //IRQ通道被使能                  
	NVIC_Init(&NVIC_InitStructure); //初始化NVIC寄存器
}
/*
 * 函数名：TIMER3_INIT(void)
 * 描述  ：1s中断一次
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void TIMER3_INIT(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	//定义结构体变量    
		NVIC_InitTypeDef NVIC_InitStructure; 
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 	//定时器4的时钟使能
    TIM_TimeBaseStructure.TIM_Prescaler = 7199;	//psc设置用来作为TIM4时钟频率除数的预分频值 72M/7200=10K //时间计算公式：（TIM_Prescaler+1）/72M * (TIM_Period+1)
    TIM_TimeBaseStructure.TIM_Period = 4999;	//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	  arr  10K/10000=1Hz  1s
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	//TIM向上计数模式
    TIM_TimeBaseStructure.TIM_ClockDivision= TIM_CKD_DIV1;	//设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);	//根据指定的参数初始化TIM4的时间基数单位     
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);	//清除TIM4的中断TIM_IT标志位，TIM4的溢出中断发生后，我们要清除中断标志位
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);	//使能指定的TIM4中断,允许更新中断
    TIM_ARRPreloadConfig(TIM3, DISABLE);	//禁用TIM4外围ARR预装寄存器
    TIM_Cmd(TIM3,ENABLE); 
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 		//中断源TIM4      
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 1; //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级           
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;  //IRQ通道被使能                  
	NVIC_Init(&NVIC_InitStructure); //初始化NVIC寄存器
}

/*
 * 函数名：TIMER4_INIT(void)
 * 描述  ：1s中断一次
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void TIMER4_INIT(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	//定义结构体变量    
		NVIC_InitTypeDef NVIC_InitStructure; 
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 	//定时器4的时钟使能
    TIM_TimeBaseStructure.TIM_Prescaler = 7199;	//psc设置用来作为TIM4时钟频率除数的预分频值 72M/7200=10K //时间计算公式：（TIM_Prescaler+1）/72M * (TIM_Period+1)
    TIM_TimeBaseStructure.TIM_Period = 4999;	//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	  arr  10K/10000=1Hz  1s
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	//TIM向上计数模式
    TIM_TimeBaseStructure.TIM_ClockDivision= TIM_CKD_DIV1;	//设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);	//根据指定的参数初始化TIM4的时间基数单位     
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);	//清除TIM4的中断TIM_IT标志位，TIM4的溢出中断发生后，我们要清除中断标志位
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);	//使能指定的TIM4中断,允许更新中断
    TIM_ARRPreloadConfig(TIM4, DISABLE);	//禁用TIM4外围ARR预装寄存器
    TIM_Cmd(TIM4,ENABLE); 
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; 		//中断源TIM4      
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 1; //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级           
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;  //IRQ通道被使能                  
	NVIC_Init(&NVIC_InitStructure); //初始化NVIC寄存器
}


//TIMER5_INIT是在大容量的才有
/*
 * 函数名：TIMER5_INIT(void)
 * 描述  ：1s中断一次
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void TIMER5_INIT(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	//定义结构体变量    
		NVIC_InitTypeDef NVIC_InitStructure; 
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); 	//定时器4的时钟使能
    TIM_TimeBaseStructure.TIM_Prescaler = 7199;	//psc设置用来作为TIM4时钟频率除数的预分频值 72M/7200=10K //时间计算公式：（TIM_Prescaler+1）/72M * (TIM_Period+1)
    TIM_TimeBaseStructure.TIM_Period = 4999;	//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	  arr  10K/10000=1Hz  1s
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	//TIM向上计数模式
    TIM_TimeBaseStructure.TIM_ClockDivision= TIM_CKD_DIV1;	//设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);	//根据指定的参数初始化TIM4的时间基数单位     
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);	//清除TIM4的中断TIM_IT标志位，TIM4的溢出中断发生后，我们要清除中断标志位
    TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);	//使能指定的TIM4中断,允许更新中断
    TIM_ARRPreloadConfig(TIM5, DISABLE);	//禁用TIM4外围ARR预装寄存器
    TIM_Cmd(TIM5,ENABLE); 
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn; 		//中断源TIM5     
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 1; //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级           
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;  //IRQ通道被使能                  
	NVIC_Init(&NVIC_InitStructure); //初始化NVIC寄存器
}



//TIMER6_INIT是在大容量的才有
/*
 * 函数名：TIMER6_INIT(void)
 * 描述  ：1s中断一次
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void TIMER6_INIT(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	//定义结构体变量    
		NVIC_InitTypeDef NVIC_InitStructure; 
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); 	//定时器4的时钟使能
    TIM_TimeBaseStructure.TIM_Prescaler = 719;	//psc设置用来作为TIM4时钟频率除数的预分频值 72M/7200=10K //时间计算公式：（TIM_Prescaler+1）/72M * (TIM_Period+1)
    TIM_TimeBaseStructure.TIM_Period = 199;	//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	  arr  10K/10000=1Hz  1s
		
//		TIM_TimeBaseStructure.TIM_Prescaler = 7199;	//psc设置用来作为TIM4时钟频率除数的预分频值 72M/7200=10K //时间计算公式：（TIM_Prescaler+1）/72M * (TIM_Period+1)
//    TIM_TimeBaseStructure.TIM_Period = 29999;	//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	  arr  10K/10000=1Hz  1s
	
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	//TIM向上计数模式
    TIM_TimeBaseStructure.TIM_ClockDivision= TIM_CKD_DIV1;	//设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);	//根据指定的参数初始化TIM4的时间基数单位     
    TIM_ClearITPendingBit(TIM6, TIM_IT_Update);	//清除TIM4的中断TIM_IT标志位，TIM4的溢出中断发生后，我们要清除中断标志位
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);	//使能指定的TIM4中断,允许更新中断
    TIM_ARRPreloadConfig(TIM6, DISABLE);	//禁用TIM4外围ARR预装寄存器
    TIM_Cmd(TIM6,ENABLE); 
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn; 		//中断源TIM5     
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 2; //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级           
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;  //IRQ通道被使能                  
	NVIC_Init(&NVIC_InitStructure); //初始化NVIC寄存器
}


//TIMER7_INIT是在大容量的才有
/*
 * 函数名：TIMER7_INIT(void)
 * 描述  ：1s中断一次
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void TIMER7_INIT(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	//定义结构体变量    
		NVIC_InitTypeDef NVIC_InitStructure; 
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); 	//定时器4的时钟使能
    TIM_TimeBaseStructure.TIM_Prescaler = 7199;	//psc设置用来作为TIM4时钟频率除数的预分频值 72M/7200=10K //时间计算公式：（TIM_Prescaler+1）/72M * (TIM_Period+1)
    TIM_TimeBaseStructure.TIM_Period = 999;	//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	  arr  10K/10000=1Hz  1s
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	//TIM向上计数模式
    TIM_TimeBaseStructure.TIM_ClockDivision= TIM_CKD_DIV1;	//设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);	//根据指定的参数初始化TIM4的时间基数单位     
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);	//清除TIM4的中断TIM_IT标志位，TIM4的溢出中断发生后，我们要清除中断标志位
    TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);	//使能指定的TIM4中断,允许更新中断
    TIM_ARRPreloadConfig(TIM7, DISABLE);	//禁用TIM4外围ARR预装寄存器
    TIM_Cmd(TIM7,ENABLE); 
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn; 		//中断源TIM5     
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 3; //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级           
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;  //IRQ通道被使能                  
	NVIC_Init(&NVIC_InitStructure); //初始化NVIC寄存器
}


/******************* (C) COPYRIGHT 2016 **********************************END OF FILE*****/
