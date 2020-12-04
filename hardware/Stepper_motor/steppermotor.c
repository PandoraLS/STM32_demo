/******************** (C) COPYRIGHT 2015 ****************************************
 * 文件名  ：steppermotor.c
 * 描述    ：         
 * 实验平台：野火STM32开发板
 * 硬件连接：----------------------------
 *          |   PA.09: 产生脉冲的引脚    |
 *          |   PB.01: 提取脉冲数的引脚  |
 *      	|                            | 
 *    		|                            |
 *           ----------------------------    			
 * 库版本  ：ST3.0.0
 *
 * 作者    ：逆风  QQ: 116****038
**********************************************************************************/
#include "steppermotor.h"
#include "math.h"
//int TIM3_Period=180;//TIM3设置预分频数
//int TIM3_waveNumber=1800;//设置跳变值，当计数器计数到这个值的给定公式时，电平发生跳变
//int TIM1_Period=72;//TIM1设置预分频数
//int TIM1_waveNumber=1000;//设置跳变值，当计数器计数到这个值的给定公式时，电平发生跳变
#define PWM_Period 18000//这个数据可以使步进电机最高频率
int waveNumber=10;
int TIM1_Period=1;
long int overflow=0;
int accel_flag=0;
BAND BAND_C;//此结构体是为步进电机要做高速运动而准备的结构体
/*
 * 函数名：void Stepper_Motor_Init(void)
 * 描述  ：产生脉冲引脚和提取脉冲引脚的配置和TIM1和TIM3的配置
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void Stepper_Motor_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	//PCLK1经过2倍频后作为TIM3的时钟源等于36MHz
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	//PCLK1经过1倍频后作为TIM1的时钟源等于72MHz
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	/*步进电机脉冲输出引脚的配置*/
	/* GPIOA and GPIOB clock enable */
	RCC_APB2PeriphClockCmd(Stepper_motor_pulse|Stepper_motor_time, ENABLE);			 
	/*Timer1 Channel 2,PA8*/
	GPIO_InitStructure.GPIO_Pin = Stepper_motor_pulse_Out;						  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(Stepper_motor_pulse_G , &GPIO_InitStructure);
	/*Timer3 Channel 4,PB1*/
	GPIO_InitStructure.GPIO_Pin = Stepper_motor_time_Out;						 
	GPIO_Init(Stepper_motor_time_G , &GPIO_InitStructure);
	RCC_APB2PeriphClockCmd( MOTOR_COTR_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin =MOTOR_COTR_DIR;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(MOTOR_COTR_PORT, &GPIO_InitStructure);
	
	/*TIM3的配置*/
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period = waveNumber*2;	 //当定时器从0计数到waveNumber*2，即为waveNumber*2次，为一个定时周期					
	TIM_TimeBaseStructure.TIM_Prescaler = PWM_Period/2-1;//设置预分频：不预分频，即为36MHz	//从零开始算起，所以要减1
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//设置时钟分频系数：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInit(TIM3, & TIM_TimeBaseStructure);
	
	/* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;		//配置为PWM模式1			 
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//当定时器计数值小于CCR1_Val时为低电平
	TIM_OCInitStructure.TIM_Pulse =waveNumber*2-1;//TIM3_waveNumber-1;//;//设置跳变值，当计数器计数到这个值时，电平发生跳变
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	
	TIM_CtrlPWMOutputs(TIM3,ENABLE); 
	TIM_SelectOnePulseMode(TIM3,TIM_OPMode_Single);//设置TM3在单脉冲模式，且是单一的脉冲，在下一个更新事件停止
	
	/*TIM1的配置*/
	TIM_DeInit(TIM1);
	TIM_TimeBaseStructure.TIM_Period = PWM_Period;		//从零开始算起，所以要减1
	TIM_TimeBaseStructure.TIM_Prescaler = 0;	  //从零开始算起，所以要减1
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, & TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse = PWM_Period/2;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	
		 /* Enable timer's master/slave work mode */
    TIM_SelectMasterSlaveMode(TIM3,TIM_MasterSlaveMode_Enable);//同步主定时器和从定时器
    TIM_SelectMasterSlaveMode(TIM1,TIM_MasterSlaveMode_Enable);//同步主定时器和从定时器
    /* timer3's channel 4 is used as the control signal */
    TIM_SelectOutputTrigger(TIM3,TIM_TRGOSource_OC4Ref );//当TIM3的通道4有个有效脉冲时，CC1IF flag 将被置位。
    /* Check the master/slave is valid or not */
    compile_assert((u16)GetInternalTrigger(TIM1,TIM3) != (u16)-1); 
    /* Config timer1's external clock */
    TIM_ITRxExternalClockConfig(TIM1, GetInternalTrigger(TIM1,TIM3));//Trigger source.
    TIM_SelectSlaveMode(TIM1,TIM_SlaveMode_Gated);//Rising edge of the selected trigger signal (TRGI) re-initializes
                                                  //the counter and triggers an update of the registers.
		
		TIM_Cmd(TIM1,ENABLE);
		BAND_C_Init();
}

/*
 * 函数名：void stepper_low_speed_Move(signed int step,unsigned max_speed)
 * 描述  ：调用这个函数可以精确的控制电机的转多少步，以及电机转到这个要求步数的转速
 * 输入  ：step我们需要步进电机转多少步，max_speed为电机的转速（500Hz~4000Hz这里的脉冲频率可以理解为步进电机转速）
 * 输出  ：无
 * 调用  ：外部调用
 */
/*step为需要电机转多少步，pulse_frequency为脉冲频率其范围为：500Hz~4000Hz,max_speed为设定步进电机正常运行的最大转速*/
void stepper_low_speed_Move(signed int step,unsigned max_speed)
{
	TIM1->CNT = 0;//定时器1清零
	if(step <0)//控制电机运动方向
	{
		GPIO_ResetBits(MOTOR_COTR_PORT, MOTOR_COTR_DIR);//顺时针
	}
	else
	{
		GPIO_SetBits(MOTOR_COTR_PORT, MOTOR_COTR_DIR);//逆时针 
	}
	step=abs(step);//求step的绝对值，方便下面计算
	TIM1_Period=(int)(4000/max_speed);//调节脉冲频率
	
	if(step<200)//如果小于200步就没有必要做平滑转速处理
	{
		if(max_speed<2000)//给
		{
			TIM1->PSC =TIM1_Period-1;//直接给我们输入的脉冲频率，即步进电机的转速
		}
		else
		{
			TIM1->PSC =1;//如果我们要求的步数小于200，但是有要求其转速超过2000，这时程序里面强制设置为2000
			TIM1_Period=2;
		}
	TIM3->ARR = TIM1_Period*step*2;  /* Reload wave number*/
	TIM3->CCR4 =TIM1_Period*step*2-1;//更新TIM3的数据，更新脉冲输出个数
	TIM3->CR1|=1; /* Re-enable the timer */
	}
	else
	{
		if(max_speed<2000)//如果给定步数大于200步，当我们要求的输入脉冲频率小于2000，即我们要求好多的频率，他就给好多的频率
		 {
			TIM1->PSC =TIM1_Period-1;
		 }
		 else//这里面的数据需要平滑处理应用于要求电机转速要求较高或扭矩比较大的时候
		 {
			 TIM1->PSC =1;
			 TIM1_Period=2;
		}
		 TIM3->ARR = TIM1_Period*step*2;  /* Reload wave number*/
		 TIM3->CCR4 =TIM1_Period*step*2-1;//更新TIM3的数据，更新脉冲输出个数
		 TIM3->CR1|=1; /* Re-enable the timer */
	}    
	while((TIM3->CR1 & 1));
	TIM3->CCR4 =0;
}

/*
 * 函数名：void stop_tim3()
 * 描述  ：时时调用这个函数，判断电机是否执行完我们给定的脉冲输没（一般在while(1)里面调用）  
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void stop_tim3(void)//时时调用这个函数，判断电机是否执行完我们给定的脉冲输没，             
{               //如果执行完我们给定的脉冲数，我们直接将tim3的pwm输出为0,这是脉冲输出为0
   if(!(TIM3->CR1 & 1))
	 {
			TIM3->CCR4 =0;
	 }
}

/*
 * 函数名：void BAND_C_Init(void)
 * 描述  ：此结构体是为步进电机要做高速运动而准备的结构体
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
void BAND_C_Init(void)//此结构体是为步进电机要做高速运动而准备的结构体
{
	BAND_C.BandF=0;//频段频率
	BAND_C.BandP=0;//频段脉冲
	BAND_C.TotalP=0;//累计脉冲
}

/*
 * 函数名：void BandTabCreate_accelerate(long StaF,long MovF,signed stepper_direction)
 * 描述  ：此函数为加速度过程函数，让电机从最低频率加速到较高频率。经测试一般加速从400HZ~2000HZ范围内。（这个加减速过程都是S曲线的）
 * 输入  ：StaF为起使频率一般取为（400Hz-500Hz），MovF为目标频率（最大2000Hz），step控制电机运动方向
 * 输出  ：无
 * 调用  ：外部调用
 */
void BandTabCreate_accelerate(long StaF,long MovF,signed char stepper_direction)//StaF为起使频率，MovF为目标频率,否则表示减速过程,stepper_direction控制电机运动方向
{
	 int    stepper_chc=3000;//stepper_c为常数，决定了步进电机加减速过程的快慢。
	 long   InfF=(StaF+MovF)/2;//拐点频率
	 double InfT=sqrt(2.0*(InfF-StaF)/stepper_chc);//拐点时间
	 double St=2.0/StaF;//频段时间跨度
	 double T=0;//频段开始时刻
	 if(stepper_direction <=0)//控制电机运动方向
	 {
		 GPIO_ResetBits(MOTOR_COTR_PORT, MOTOR_COTR_DIR);//顺时针
	 }
	 else
	 {
		 GPIO_SetBits(MOTOR_COTR_PORT, MOTOR_COTR_DIR);//逆时针 
	 }
	 while(T<InfT)//计算0-t1时间段内T时刻频段频率
	{
	   TIM3->CCR4 =0;//避免误差
	   BAND_C.BandF=StaF+(stepper_chc*T*T/2.0);//(2)式曲线方程实现
	   T=T+St;//下一频段时刻
	   BAND_C.BandP=(int)(St*BAND_C.BandF);//此段频率的脉冲个数
	   BAND_C.TotalP+=BAND_C.BandP;//累计脉冲数
	   stepper_high_speed_Move(BAND_C.BandP,BAND_C.BandF);//控制步进电机的运行的距离
	   while((TIM3->CR1 & 1)); //等待步进电机运动完指定的距离
	}
	 TIM3->CCR4 =0;
	 while(BAND_C.BandF<(MovF))//判断步进电机是否达到最大速度
	 {
		TIM3->CCR4 =0;//避免误差
		BAND_C.BandF=(int)(3*StaF+2.0*stepper_chc*sqrt(2.0*(InfF-StaF)/stepper_chc)*T-2.0*InfF-stepper_chc*T*T/2.0)+1;
		//(5)式曲线方程的实现
		T=T+St;//下一频段时刻
		BAND_C.BandP=(int)(St*BAND_C.BandF);//此段频率的脉冲个数
		BAND_C.TotalP+=BAND_C.BandP;//累计脉冲数
		stepper_high_speed_Move(BAND_C.BandP,BAND_C.BandF);//控制步进电机的运行的距离
		while((TIM3->CR1 & 1));   //等待步进电机运动完指定的距离
   } 
}

/*
 * 函数名：void BandTabCreate_decelerate(long StaF,long MovF)
 * 描述  ：此函数为减速度过程函数，让电机从最高频率减速到最低频率后停止。经测试一般减速到400Hz左右就直接让步进电机停下来（这个加减速过程都是S曲线的）
 * 输入  ：StaF为起使频率一般取为（400Hz-500Hz），MovF为目标频率（最大2000Hz）
 * 输出  ：无
 * 调用  ：外部调用
 */
void BandTabCreate_decelerate(long StaF,long MovF)//StaF为起使频率，MovF为目标频率
{
	 int    stepper_chc=-3000;//stepper_c为常数，决定了步进电机加减速过程的快慢。
   long   InfF1=(StaF+MovF)/2;//拐点频率
	 double InfT1=sqrt(2.0*(MovF-InfF1)/(-stepper_chc));//拐点时间
	 double St1=2.0/StaF;//频段时间跨度
	 double T1=0;//频段开始时刻
	 while(T1<InfT1)//计算0-t1时间段内T时刻频段频率
	 {
		   TIM3->CCR4 =0;//避免误差
       BAND_C.BandF=MovF+(int)(stepper_chc*T1*T1/2.0);//(2)式曲线方程实现
		   T1=T1+St1;//下一频段时刻
		   BAND_C.BandP=(int)(St1*BAND_C.BandF);//此段频率的脉冲个数
		   BAND_C.TotalP+=BAND_C.BandP;//累计脉冲数
		   stepper_high_speed_Move(BAND_C.BandP,BAND_C.BandF);//控制步进电机的运行的距离
		   while((TIM3->CR1&1)); //等待步进电机运动完指定的距离
   }
   
	 while(BAND_C.BandF>StaF)//计算t1-t2时间段内T时刻频段频率
	 {  
		  TIM3->CCR4 =0;//避免误差
      BAND_C.BandF=3*MovF-2*InfF1+2.0*stepper_chc*sqrt(2.0*(MovF-InfF1)/-stepper_chc)*T1-stepper_chc*T1*T1/2.0;//9000-3500-2000*1.58*T1+1000*T1*T1/2;
		  //(5)式曲线方程的实现
		  T1=T1+St1;//下一频段时刻
		  BAND_C.BandP=(int)(St1*BAND_C.BandF);//此段频率的脉冲个数
		  BAND_C.TotalP+=BAND_C.BandP;//累计脉冲数
		  stepper_high_speed_Move(BAND_C.BandP,BAND_C.BandF);//控制步进电机的运行的距离
		  while((TIM3->CR1 & 1));  //等待步进电机运动完指定的距离
   }
	 TIM3->CCR4 =0;//目的是让步进电机停止
	 BAND_C_Init();//再次初始化BAND_C_Init();比面其相关变量溢出
}

/*
 * 函数名：void stepper_high_speed_Move(int step,unsigned max_speed)
 * 描述  ：这个函数可以让我们给人以脉冲频率，不会收到限制，适合步进电机高速调节是用
 * 输入  ：step为需要电机转多少步，pulse_frequency为脉冲频率其范围为：500Hz~2000Hz,max_speed为设定步进电机正常运行的最大转速
 * 输出  ：无
 * 调用  ：外部调用
 */
void stepper_high_speed_Move(int step,unsigned max_speed)
{
		  TIM1->CNT = 0;//定时器1清零
		  step=abs(step);//求step的绝对值，方便下面计算
			TIM1_Period=(int)(4000/max_speed);//调节脉冲频率
      TIM1->PSC =TIM1_Period-1;//直接给我们输入的脉冲频率，即步进电机的转速
			TIM3->ARR = TIM1_Period*step*2;  /* Reload wave number*/
			TIM3->CCR4 =TIM1_Period*step*2-1;//更新TIM3的数据，更新脉冲输出个数
			TIM3->CR1|=1; /* Re-enable the timer */
}
/*放入主函数测试程序*/
//////  while(1)
//////  {  
////// 	
//////		 KEY_Back=KEY_Scan(0);//独立按键扫描
//////	
//////		 switch(KEY_Back)
//////		 {
//////			 case 1:BandTabCreate_accelerate(500,2000,100);break;//步进电机从500HZ的频率加速的2000HZ
//////			 case 2:BandTabCreate_decelerate(500,2000);;break;//步进电机从2000HZ的频率减速的500HZ
//////			 case 3:stepper_low_speed_Move(400,1000);break;//以1000HZ的频率转400步
//////			 case 4:stepper_low_speed_Move(800,2000);;break;//以2000HZ的频率转800步
//////			 default:break;
//////		 }	
//////		 stop_tim3();			
//////	}

/*
 * 函数名：void Stepper_Init(void)
 * 描述  ：小步进电机的引脚初始化，驱动使用（ULN2003）
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void Stepper_Init(void)
{
	GPIO_InitTypeDef     GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_2|GPIO_Pin_3;			   //串口1发送脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		   //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   //输出速度50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
