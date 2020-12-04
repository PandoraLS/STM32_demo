/******************** (C) COPYRIGHT 2015 ****************************************
 * �ļ���  ��incrementalpid.c
 * ����    ������ʽpid     
 * ʵ��ƽ̨��Ұ��STM32������
 * Ӳ�����ӣ�---------------------
 *          |                     |
 *          |        ��           |
 *      	  |                     | 
 *    		  |                     |
 *           ---------------------    			
 * ��汾  ��ST3.0.0
 *
 * ����    �����  QQ: 116****038
**********************************************************************************/
#include "incrementalpid.h"
#include <math.h>
#include "usart.h"
#include "pwm0.h"
#include "OLED.h"	

INC_PID Inc_Pid;//INC_PIDΪincrementalpid.h�ļ�����Ľṹ�壬Inc_PidΪINC_PID�Ľṹ�����
int INC_Pwm_var1=0;//pid������
int INC_Pwm_var2=0;//pid������
int INC_Pwm_out1=0;//pwm�����
int INC_Pwm_out2=0;//pwm�����
double U0=0;//����ʽpid���õ�һ����ʽ��U0��ʾ�����ʽ�Ⱥŵ����
double U1=0;//����ʽpid���õ�һ����ʽ��U0��ʾ�����ʽ�Ⱥŵ����
/*
 * ��������void INC_Pid_Init(void)
 * ����  ����ʼ��Inc_Pid�ṹ���Ա����
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void INC_Pid_Init(void)
{
  Inc_Pid.SetPoint=0;           //�趨ֵ
//	Inc_Pid.Proportion=0;         // Proportion ����ϵ��
//	Inc_Pid.Integral=0;           // Integral   ����ϵ��
//	Inc_Pid.Derivative=0;         // Derivative  ΢��ϵ��
	Inc_Pid.Now_Error=0;
	Inc_Pid.LastError=0;          // Error[-1]  ǰһ�����
	Inc_Pid.PreError=0;           // Error[-2]  ǰ�������
}

/*
 * ��������int INC_Pid_Ctrl(int feb)
 * ����  ������ʽpid�Ĵ���SpeedΪ�趨������ٶȿ���Ϊ��Ҳ����Ϊ����feb�趨Ϊ���̷��ص�����(�����̲�������ʵ���ٶ�ֵ)
 * ����  ��SpeedΪ�趨������ٶȿ���Ϊ��Ҳ����Ϊ����feb�趨Ϊ���̷��ص�����(�����̲�������ʵ���ٶ�ֵ)
 * ���  ����������ʽpid���ں�����Ե����pwmռ�ձȵ������
 * ����  ���ڲ�����
 */
void INC_Pid_Ctrl(int Speed,int feb)//feb��������(�ڵ����������ʱencoder_count1_now(�����ݴ�����10ms�ж���))
{
	 Inc_Pid.SetPoint=Speed;
   Inc_Pid.Now_Error=Inc_Pid.SetPoint-feb;//�ٶȲ�=�趨�ٶ�-Ŀ���ٶ�
	if((abs(Inc_Pid.Now_Error))<Error_min)//abs���������ľ���ֵ��fabs����double���ľ���ֵ
  {
     INC_Pwm_var1=U0;
	 INC_Pwm_var2=U1;
  }
	else
	{
		 U0=Kpp*Inc_Pid.Now_Error+Ki*Inc_Pid.LastError+Kd*Inc_Pid.PreError;//����ʽpid��ʽ
		 Inc_Pid.PreError=Inc_Pid.LastError;  // Error[-1]  ǰһ�����
		 Inc_Pid.LastError=Inc_Pid.Now_Error; // Error[-2]  ǰ�������
		 INC_Pwm_var1=(int)U0;//ǿ��ת����������pwmΪ����
		
		 if(INC_Pwm_var1>=Umax)INC_Pwm_var1=Umax;//����ֵ�޷�����ֹ���ֱ���
		 if(INC_Pwm_var1<=Umin)INC_Pwm_var1=Umin;//����ֵ�޷�����ֹ���ֱ���
  }
	INC_Pwm_out1+=INC_Pwm_var1;//����pwm���
	if(INC_Pwm_out1>Pmax)INC_Pwm_out1=Pmax;//����ֵ�޷�(����pwm�趨ֵ��ȷ���޷���Pmaxռ�ձ�������ֵ)
	if(INC_Pwm_out1<Pmin)INC_Pwm_out1=Pmin;//����ֵ�޷�(����pwm�趨ֵ��ȷ���޷���Pmaxռ�ձ�������ֵ)
  
    TIM3->CCR2 = INC_Pwm_out1;//�����Ŀ�����޸ĵ����pwm��ռ�ձȣ�
																	//����õ���ͬ��ͨ��Ҫ��CCRN(N�ķ�ΧΪ��1~4)
																//����õ����pwm�Ķ�ʱ����ͬҪ�޸�TIMX(X�ķ�ΧΪ��0~7
}
		    /*�����������*/
////		#include "stm32f10x.h"
////		#include "delay.h"
////		#include "sys.h"
////		#include "usart.h"
////		//#include "usart.h"
////		#include "OLED.h"		
////		//#include "pwm0.h"
////		#include "pwm1.h"
////		//#include "pwm2.h"
////		#include "encoder.h"
////		#include "exti.h"
////		#include "incrementalpid.h"
////		//#include "ultrasonic.h"
////		//#include "steppermotor.h"
////		//#include "key.h"
////		//#include "usart.h"
////		 int main(void)
////		 {	
////			delay_init();	    	 //��ʱ������ʼ��	  
////			//NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
////			//uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600
////			 //USART1_Config();
////			 USART2_Config();
////			//USART1_Config();
////			//USART2_Config();
////			OLED_Init();
////			//TIM3_PWM0_Init();
////			TIM3_PWM1_Init();
////			//TIM3_PWM2_Init();
////			//OLED_BMP(0, 0, 127, 63, );
////			TIM2_Config();//����������
////			TIMER4_INIT();//�ڶ�ʱ��4����10ms�ж�����
////			MBTIM4_NVIC();//��ʱ��4��10ms�ж����ȼ������� 
////			INC_Pid_Init();//����ʽpid��ʼ��
////			//Ultran_all_Init(void);//��ƽ������������ʼ��
////			 //Us100_USART3_Init();
////			//Stepper_Motor_Init();//���������ʼ������
////			//KEY_Init();//�ĸ�����������ʼ������
////			//KEY_Matrix_Init();//���󰴼���ʼ������
////			while(1)
////			{  
////					 INC_Pid_Ctrl(0,encoder_count1_now_v);
////					
////					 printf("  ");
////					 UART2_Oscilloscope(encoder_count1_now_v , 0);
////					// delay_ms(100);
////			}
////		}
