/******************** (C) COPYRIGHT 2015 ****************************************
 * 文件名  ：incrementalpid.c
 * 描述    ：增量式pid     
 * 实验平台：野火STM32开发板
 * 硬件连接：---------------------
 *          |                     |
 *          |        无           |
 *      	  |                     | 
 *    		  |                     |
 *           ---------------------    			
 * 库版本  ：ST3.0.0
 *
 * 作者    ：逆风  QQ: 116****038
**********************************************************************************/
#include "incrementalpid.h"
#include <math.h>
#include "usart.h"
#include "pwm0.h"
#include "OLED.h"	

INC_PID Inc_Pid;//INC_PID为incrementalpid.h文件定义的结构体，Inc_Pid为INC_PID的结构体变量
int INC_Pwm_var1=0;//pid调节量
int INC_Pwm_var2=0;//pid调节量
int INC_Pwm_out1=0;//pwm输出量
int INC_Pwm_out2=0;//pwm输出量
double U0=0;//增量式pid会用到一个公式，U0表示这个公式等号的左边
double U1=0;//增量式pid会用到一个公式，U0表示这个公式等号的左边
/*
 * 函数名：void INC_Pid_Init(void)
 * 描述  ：初始化Inc_Pid结构体成员变量
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void INC_Pid_Init(void)
{
  Inc_Pid.SetPoint=0;           //设定值
//	Inc_Pid.Proportion=0;         // Proportion 比例系数
//	Inc_Pid.Integral=0;           // Integral   积分系数
//	Inc_Pid.Derivative=0;         // Derivative  微分系数
	Inc_Pid.Now_Error=0;
	Inc_Pid.LastError=0;          // Error[-1]  前一拍误差
	Inc_Pid.PreError=0;           // Error[-2]  前两拍误差
}

/*
 * 函数名：int INC_Pid_Ctrl(int feb)
 * 描述  ：增量式pid的处理Speed为设定电机的速度可以为正也可以为负，feb设定为码盘返回的数据(即码盘测出电机的实际速度值)
 * 输入  ：Speed为设定电机的速度可以为正也可以为负，feb设定为码盘返回的数据(即码盘测出电机的实际速度值)
 * 输出  ：经过增量式pid调节后，输出对电机的pwm占空比的输出量
 * 调用  ：内部调用
 */
void INC_Pid_Ctrl(int Speed,int feb)//feb采样反馈(在调用这个函数时encoder_count1_now(此数据处理在10ms中断中))
{
	 Inc_Pid.SetPoint=Speed;
   Inc_Pid.Now_Error=Inc_Pid.SetPoint-feb;//速度差=设定速度-目标速度
	if((abs(Inc_Pid.Now_Error))<Error_min)//abs是求整数的绝对值，fabs是求double数的绝对值
  {
     INC_Pwm_var1=U0;
	 INC_Pwm_var2=U1;
  }
	else
	{
		 U0=Kpp*Inc_Pid.Now_Error+Ki*Inc_Pid.LastError+Kd*Inc_Pid.PreError;//增量式pid公式
		 Inc_Pid.PreError=Inc_Pid.LastError;  // Error[-1]  前一拍误差
		 Inc_Pid.LastError=Inc_Pid.Now_Error; // Error[-2]  前两拍误差
		 INC_Pwm_var1=(int)U0;//强制转化调整量，pwm为整数
		
		 if(INC_Pwm_var1>=Umax)INC_Pwm_var1=Umax;//调节值限幅，防止积分饱和
		 if(INC_Pwm_var1<=Umin)INC_Pwm_var1=Umin;//调节值限幅，防止积分饱和
  }
	INC_Pwm_out1+=INC_Pwm_var1;//调整pwm输出
	if(INC_Pwm_out1>Pmax)INC_Pwm_out1=Pmax;//数出值限幅(根据pwm设定值来确定限幅，Pmax占空比最大计数值)
	if(INC_Pwm_out1<Pmin)INC_Pwm_out1=Pmin;//数出值限幅(根据pwm设定值来确定限幅，Pmax占空比最大计数值)
  
    TIM3->CCR2 = INC_Pwm_out1;//这个的目的是修改电机的pwm的占空比，
																	//如果用到不同的通道要改CCRN(N的范围为：1~4)
																//如果用到输出pwm的定时器不同要修改TIMX(X的范围为：0~7
}
		    /*本代码测试用*/
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
////			delay_init();	    	 //延时函数初始化	  
////			//NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
////			//uart_init(9600);	 	//串口初始化为9600
////			 //USART1_Config();
////			 USART2_Config();
////			//USART1_Config();
////			//USART2_Config();
////			OLED_Init();
////			//TIM3_PWM0_Init();
////			TIM3_PWM1_Init();
////			//TIM3_PWM2_Init();
////			//OLED_BMP(0, 0, 127, 63, );
////			TIM2_Config();//编码器配置
////			TIMER4_INIT();//在定时器4设置10ms中断配置
////			MBTIM4_NVIC();//定时器4在10ms中断优先级的配置 
////			INC_Pid_Init();//增量式pid初始化
////			//Ultran_all_Init(void);//电平触发超声波初始化
////			 //Us100_USART3_Init();
////			//Stepper_Motor_Init();//步进电机初始化程序
////			//KEY_Init();//四个独立按键初始化程序
////			//KEY_Matrix_Init();//矩阵按键初始化程序
////			while(1)
////			{  
////					 INC_Pid_Ctrl(0,encoder_count1_now_v);
////					
////					 printf("  ");
////					 UART2_Oscilloscope(encoder_count1_now_v , 0);
////					// delay_ms(100);
////			}
////		}
