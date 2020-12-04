/******************** (C) COPYRIGHT 2015 ****************************************
 * �ļ���  ��steppermotor.c
 * ����    ��         
 * ʵ��ƽ̨��Ұ��STM32������
 * Ӳ�����ӣ�----------------------------
 *          |   PA.09: �������������    |
 *          |   PB.01: ��ȡ������������  |
 *      	|                            | 
 *    		|                            |
 *           ----------------------------    			
 * ��汾  ��ST3.0.0
 *
 * ����    �����  QQ: 116****038
**********************************************************************************/
#include "steppermotor.h"
#include "math.h"
//int TIM3_Period=180;//TIM3����Ԥ��Ƶ��
//int TIM3_waveNumber=1800;//��������ֵ�������������������ֵ�ĸ�����ʽʱ����ƽ��������
//int TIM1_Period=72;//TIM1����Ԥ��Ƶ��
//int TIM1_waveNumber=1000;//��������ֵ�������������������ֵ�ĸ�����ʽʱ����ƽ��������
#define PWM_Period 18000//������ݿ���ʹ����������Ƶ��
int waveNumber=10;
int TIM1_Period=1;
long int overflow=0;
int accel_flag=0;
BAND BAND_C;//�˽ṹ����Ϊ�������Ҫ�������˶���׼���Ľṹ��
/*
 * ��������void Stepper_Motor_Init(void)
 * ����  �������������ź���ȡ�������ŵ����ú�TIM1��TIM3������
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void Stepper_Motor_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	//PCLK1����2��Ƶ����ΪTIM3��ʱ��Դ����36MHz
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	//PCLK1����1��Ƶ����ΪTIM1��ʱ��Դ����72MHz
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	/*�����������������ŵ�����*/
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
	
	/*TIM3������*/
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period = waveNumber*2;	 //����ʱ����0������waveNumber*2����ΪwaveNumber*2�Σ�Ϊһ����ʱ����					
	TIM_TimeBaseStructure.TIM_Prescaler = PWM_Period/2-1;//����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ36MHz	//���㿪ʼ��������Ҫ��1
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//����ʱ�ӷ�Ƶϵ��������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, & TIM_TimeBaseStructure);
	
	/* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;		//����ΪPWMģʽ1			 
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//����ʱ������ֵС��CCR1_ValʱΪ�͵�ƽ
	TIM_OCInitStructure.TIM_Pulse =waveNumber*2-1;//TIM3_waveNumber-1;//;//��������ֵ�������������������ֵʱ����ƽ��������
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	
	TIM_CtrlPWMOutputs(TIM3,ENABLE); 
	TIM_SelectOnePulseMode(TIM3,TIM_OPMode_Single);//����TM3�ڵ�����ģʽ�����ǵ�һ�����壬����һ�������¼�ֹͣ
	
	/*TIM1������*/
	TIM_DeInit(TIM1);
	TIM_TimeBaseStructure.TIM_Period = PWM_Period;		//���㿪ʼ��������Ҫ��1
	TIM_TimeBaseStructure.TIM_Prescaler = 0;	  //���㿪ʼ��������Ҫ��1
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
    TIM_SelectMasterSlaveMode(TIM3,TIM_MasterSlaveMode_Enable);//ͬ������ʱ���ʹӶ�ʱ��
    TIM_SelectMasterSlaveMode(TIM1,TIM_MasterSlaveMode_Enable);//ͬ������ʱ���ʹӶ�ʱ��
    /* timer3's channel 4 is used as the control signal */
    TIM_SelectOutputTrigger(TIM3,TIM_TRGOSource_OC4Ref );//��TIM3��ͨ��4�и���Ч����ʱ��CC1IF flag ������λ��
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
 * ��������void stepper_low_speed_Move(signed int step,unsigned max_speed)
 * ����  ����������������Ծ�ȷ�Ŀ��Ƶ����ת���ٲ����Լ����ת�����Ҫ������ת��
 * ����  ��step������Ҫ�������ת���ٲ���max_speedΪ�����ת�٣�500Hz~4000Hz���������Ƶ�ʿ������Ϊ�������ת�٣�
 * ���  ����
 * ����  ���ⲿ����
 */
/*stepΪ��Ҫ���ת���ٲ���pulse_frequencyΪ����Ƶ���䷶ΧΪ��500Hz~4000Hz,max_speedΪ�趨��������������е����ת��*/
void stepper_low_speed_Move(signed int step,unsigned max_speed)
{
	TIM1->CNT = 0;//��ʱ��1����
	if(step <0)//���Ƶ���˶�����
	{
		GPIO_ResetBits(MOTOR_COTR_PORT, MOTOR_COTR_DIR);//˳ʱ��
	}
	else
	{
		GPIO_SetBits(MOTOR_COTR_PORT, MOTOR_COTR_DIR);//��ʱ�� 
	}
	step=abs(step);//��step�ľ���ֵ�������������
	TIM1_Period=(int)(4000/max_speed);//��������Ƶ��
	
	if(step<200)//���С��200����û�б�Ҫ��ƽ��ת�ٴ���
	{
		if(max_speed<2000)//��
		{
			TIM1->PSC =TIM1_Period-1;//ֱ�Ӹ��������������Ƶ�ʣ������������ת��
		}
		else
		{
			TIM1->PSC =1;//�������Ҫ��Ĳ���С��200��������Ҫ����ת�ٳ���2000����ʱ��������ǿ������Ϊ2000
			TIM1_Period=2;
		}
	TIM3->ARR = TIM1_Period*step*2;  /* Reload wave number*/
	TIM3->CCR4 =TIM1_Period*step*2-1;//����TIM3�����ݣ����������������
	TIM3->CR1|=1; /* Re-enable the timer */
	}
	else
	{
		if(max_speed<2000)//���������������200����������Ҫ�����������Ƶ��С��2000��������Ҫ��ö��Ƶ�ʣ����͸��ö��Ƶ��
		 {
			TIM1->PSC =TIM1_Period-1;
		 }
		 else//�������������Ҫƽ������Ӧ����Ҫ����ת��Ҫ��ϸ߻�Ť�رȽϴ��ʱ��
		 {
			 TIM1->PSC =1;
			 TIM1_Period=2;
		}
		 TIM3->ARR = TIM1_Period*step*2;  /* Reload wave number*/
		 TIM3->CCR4 =TIM1_Period*step*2-1;//����TIM3�����ݣ����������������
		 TIM3->CR1|=1; /* Re-enable the timer */
	}    
	while((TIM3->CR1 & 1));
	TIM3->CCR4 =0;
}

/*
 * ��������void stop_tim3()
 * ����  ��ʱʱ��������������жϵ���Ƿ�ִ�������Ǹ�����������û��һ����while(1)������ã�  
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void stop_tim3(void)//ʱʱ��������������жϵ���Ƿ�ִ�������Ǹ�����������û��             
{               //���ִ�������Ǹ�����������������ֱ�ӽ�tim3��pwm���Ϊ0,�����������Ϊ0
   if(!(TIM3->CR1 & 1))
	 {
			TIM3->CCR4 =0;
	 }
}

/*
 * ��������void BAND_C_Init(void)
 * ����  ���˽ṹ����Ϊ�������Ҫ�������˶���׼���Ľṹ��
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
void BAND_C_Init(void)//�˽ṹ����Ϊ�������Ҫ�������˶���׼���Ľṹ��
{
	BAND_C.BandF=0;//Ƶ��Ƶ��
	BAND_C.BandP=0;//Ƶ������
	BAND_C.TotalP=0;//�ۼ�����
}

/*
 * ��������void BandTabCreate_accelerate(long StaF,long MovF,signed stepper_direction)
 * ����  ���˺���Ϊ���ٶȹ��̺������õ�������Ƶ�ʼ��ٵ��ϸ�Ƶ�ʡ�������һ����ٴ�400HZ~2000HZ��Χ�ڡ�������Ӽ��ٹ��̶���S���ߵģ�
 * ����  ��StaFΪ��ʹƵ��һ��ȡΪ��400Hz-500Hz����MovFΪĿ��Ƶ�ʣ����2000Hz����step���Ƶ���˶�����
 * ���  ����
 * ����  ���ⲿ����
 */
void BandTabCreate_accelerate(long StaF,long MovF,signed char stepper_direction)//StaFΪ��ʹƵ�ʣ�MovFΪĿ��Ƶ��,�����ʾ���ٹ���,stepper_direction���Ƶ���˶�����
{
	 int    stepper_chc=3000;//stepper_cΪ�����������˲�������Ӽ��ٹ��̵Ŀ�����
	 long   InfF=(StaF+MovF)/2;//�յ�Ƶ��
	 double InfT=sqrt(2.0*(InfF-StaF)/stepper_chc);//�յ�ʱ��
	 double St=2.0/StaF;//Ƶ��ʱ����
	 double T=0;//Ƶ�ο�ʼʱ��
	 if(stepper_direction <=0)//���Ƶ���˶�����
	 {
		 GPIO_ResetBits(MOTOR_COTR_PORT, MOTOR_COTR_DIR);//˳ʱ��
	 }
	 else
	 {
		 GPIO_SetBits(MOTOR_COTR_PORT, MOTOR_COTR_DIR);//��ʱ�� 
	 }
	 while(T<InfT)//����0-t1ʱ�����Tʱ��Ƶ��Ƶ��
	{
	   TIM3->CCR4 =0;//�������
	   BAND_C.BandF=StaF+(stepper_chc*T*T/2.0);//(2)ʽ���߷���ʵ��
	   T=T+St;//��һƵ��ʱ��
	   BAND_C.BandP=(int)(St*BAND_C.BandF);//�˶�Ƶ�ʵ��������
	   BAND_C.TotalP+=BAND_C.BandP;//�ۼ�������
	   stepper_high_speed_Move(BAND_C.BandP,BAND_C.BandF);//���Ʋ�����������еľ���
	   while((TIM3->CR1 & 1)); //�ȴ���������˶���ָ���ľ���
	}
	 TIM3->CCR4 =0;
	 while(BAND_C.BandF<(MovF))//�жϲ�������Ƿ�ﵽ����ٶ�
	 {
		TIM3->CCR4 =0;//�������
		BAND_C.BandF=(int)(3*StaF+2.0*stepper_chc*sqrt(2.0*(InfF-StaF)/stepper_chc)*T-2.0*InfF-stepper_chc*T*T/2.0)+1;
		//(5)ʽ���߷��̵�ʵ��
		T=T+St;//��һƵ��ʱ��
		BAND_C.BandP=(int)(St*BAND_C.BandF);//�˶�Ƶ�ʵ��������
		BAND_C.TotalP+=BAND_C.BandP;//�ۼ�������
		stepper_high_speed_Move(BAND_C.BandP,BAND_C.BandF);//���Ʋ�����������еľ���
		while((TIM3->CR1 & 1));   //�ȴ���������˶���ָ���ľ���
   } 
}

/*
 * ��������void BandTabCreate_decelerate(long StaF,long MovF)
 * ����  ���˺���Ϊ���ٶȹ��̺������õ�������Ƶ�ʼ��ٵ����Ƶ�ʺ�ֹͣ��������һ����ٵ�400Hz���Ҿ�ֱ���ò������ͣ����������Ӽ��ٹ��̶���S���ߵģ�
 * ����  ��StaFΪ��ʹƵ��һ��ȡΪ��400Hz-500Hz����MovFΪĿ��Ƶ�ʣ����2000Hz��
 * ���  ����
 * ����  ���ⲿ����
 */
void BandTabCreate_decelerate(long StaF,long MovF)//StaFΪ��ʹƵ�ʣ�MovFΪĿ��Ƶ��
{
	 int    stepper_chc=-3000;//stepper_cΪ�����������˲�������Ӽ��ٹ��̵Ŀ�����
   long   InfF1=(StaF+MovF)/2;//�յ�Ƶ��
	 double InfT1=sqrt(2.0*(MovF-InfF1)/(-stepper_chc));//�յ�ʱ��
	 double St1=2.0/StaF;//Ƶ��ʱ����
	 double T1=0;//Ƶ�ο�ʼʱ��
	 while(T1<InfT1)//����0-t1ʱ�����Tʱ��Ƶ��Ƶ��
	 {
		   TIM3->CCR4 =0;//�������
       BAND_C.BandF=MovF+(int)(stepper_chc*T1*T1/2.0);//(2)ʽ���߷���ʵ��
		   T1=T1+St1;//��һƵ��ʱ��
		   BAND_C.BandP=(int)(St1*BAND_C.BandF);//�˶�Ƶ�ʵ��������
		   BAND_C.TotalP+=BAND_C.BandP;//�ۼ�������
		   stepper_high_speed_Move(BAND_C.BandP,BAND_C.BandF);//���Ʋ�����������еľ���
		   while((TIM3->CR1&1)); //�ȴ���������˶���ָ���ľ���
   }
   
	 while(BAND_C.BandF>StaF)//����t1-t2ʱ�����Tʱ��Ƶ��Ƶ��
	 {  
		  TIM3->CCR4 =0;//�������
      BAND_C.BandF=3*MovF-2*InfF1+2.0*stepper_chc*sqrt(2.0*(MovF-InfF1)/-stepper_chc)*T1-stepper_chc*T1*T1/2.0;//9000-3500-2000*1.58*T1+1000*T1*T1/2;
		  //(5)ʽ���߷��̵�ʵ��
		  T1=T1+St1;//��һƵ��ʱ��
		  BAND_C.BandP=(int)(St1*BAND_C.BandF);//�˶�Ƶ�ʵ��������
		  BAND_C.TotalP+=BAND_C.BandP;//�ۼ�������
		  stepper_high_speed_Move(BAND_C.BandP,BAND_C.BandF);//���Ʋ�����������еľ���
		  while((TIM3->CR1 & 1));  //�ȴ���������˶���ָ���ľ���
   }
	 TIM3->CCR4 =0;//Ŀ�����ò������ֹͣ
	 BAND_C_Init();//�ٴγ�ʼ��BAND_C_Init();��������ر������
}

/*
 * ��������void stepper_high_speed_Move(int step,unsigned max_speed)
 * ����  ������������������Ǹ���������Ƶ�ʣ������յ����ƣ��ʺϲ���������ٵ�������
 * ����  ��stepΪ��Ҫ���ת���ٲ���pulse_frequencyΪ����Ƶ���䷶ΧΪ��500Hz~2000Hz,max_speedΪ�趨��������������е����ת��
 * ���  ����
 * ����  ���ⲿ����
 */
void stepper_high_speed_Move(int step,unsigned max_speed)
{
		  TIM1->CNT = 0;//��ʱ��1����
		  step=abs(step);//��step�ľ���ֵ�������������
			TIM1_Period=(int)(4000/max_speed);//��������Ƶ��
      TIM1->PSC =TIM1_Period-1;//ֱ�Ӹ��������������Ƶ�ʣ������������ת��
			TIM3->ARR = TIM1_Period*step*2;  /* Reload wave number*/
			TIM3->CCR4 =TIM1_Period*step*2-1;//����TIM3�����ݣ����������������
			TIM3->CR1|=1; /* Re-enable the timer */
}
/*�������������Գ���*/
//////  while(1)
//////  {  
////// 	
//////		 KEY_Back=KEY_Scan(0);//��������ɨ��
//////	
//////		 switch(KEY_Back)
//////		 {
//////			 case 1:BandTabCreate_accelerate(500,2000,100);break;//���������500HZ��Ƶ�ʼ��ٵ�2000HZ
//////			 case 2:BandTabCreate_decelerate(500,2000);;break;//���������2000HZ��Ƶ�ʼ��ٵ�500HZ
//////			 case 3:stepper_low_speed_Move(400,1000);break;//��1000HZ��Ƶ��ת400��
//////			 case 4:stepper_low_speed_Move(800,2000);;break;//��2000HZ��Ƶ��ת800��
//////			 default:break;
//////		 }	
//////		 stop_tim3();			
//////	}

/*
 * ��������void Stepper_Init(void)
 * ����  ��С������������ų�ʼ��������ʹ�ã�ULN2003��
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void Stepper_Init(void)
{
	GPIO_InitTypeDef     GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_2|GPIO_Pin_3;			   //����1���ͽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		   //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   //����ٶ�50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
