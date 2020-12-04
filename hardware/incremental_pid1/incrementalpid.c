/******************** (C) COPYRIGHT 2015 ****************************************
 * 文件名  ：incrementalpid.c
 * 描述    ：增量式pid     
 * 实验平台：ipc8学习板
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
//#include "usart.h"
#include "pwm1.h"
#include "OLED.h"	

INC_PID PID1;//全局可用	

/*
 * 函数名：void INC_Pid_Init(void)
 * 描述  ：初始化Inc_Pid结构体成员变量
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void INC_Pid_Init(void)
{
  PID1.Last_Error=0;   //Error[-1]  前一拍误差
	PID1.Prev_Error=0;   //Error[-2]  前两拍误差
	PID1.Sum_Error1=0;    //累计误差
	PID1.Sum_Error2=0;    //累计误差
	
	PID1.Proportion=0;   //Proportion 比例系数
	PID1.Integral=0;     //积分常数
	PID1.Derivative=0;   //微分常数
	
	PID1.Set_Point=0;    //设定目标速度
}

/*
 * 函数名：void PID_setpoint(INC_PID*PIDx,u16 setvalue)
 * 描述  ：设定PID的预期值
 * 输入  ：PIDx是那个PID处理单元，setvalue为PID的预期值
 * 输出  ：无
 * 调用  ：外部调用
 */
void PID_Setpoint(INC_PID*PIDx,int setvalue)
{
  PIDx->Set_Point=setvalue; 
}

/*
 * 函数名：void PID_set(float pp,float ii,float dd)
 * 描述  ：设定PID Kp Ki Kd的三个参数值
 * 输入  ：Kp Ki Kd的三个参数值
 * 输出  ：无
 * 调用  ：外部调用
 */
void PID_Set(float pp,float ii,float dd)
{
  PID1.Proportion=pp;
  PID1.Integral=ii;
  PID1.Derivative=dd; 
}

/*
 * 函数名：void set_speed(float W1)
 * 描述  ：设定电机的速度
 * 输入  ：
 * 输出  ：无
 * 调用  ：外部调用
 */
void INC_PID_Calc(INC_PID *PIDx,int nextpoint)
{
	int iError,iincpid_ch1,iincpid_ch2;
	iError=PIDx->Set_Point-nextpoint;  //当前误差
	
	iincpid_ch1=					    //增量计算
	PIDx->Proportion*iError	        //e[k]项
	-PIDx->Integral*PIDx->Last_Error	  //e[k-1]
	+PIDx->Derivative*PIDx->Prev_Error;//e[k-2]
	PIDx->Prev_Error=PIDx->Last_Error; //储存误差便于计算
	PIDx->Last_Error=iError;
	
	iincpid_ch2=					    //增量计算
	-PIDx->Proportion*iError	        //e[k]项
	+PIDx->Integral*PIDx->Last_Error	  //e[k-1]
	-PIDx->Derivative*PIDx->Prev_Error;//e[k-2]
	
	PIDx->Prev_Error=PIDx->Last_Error; //储存误差便于计算
	PIDx->Last_Error=iError;
	//调节值限幅，防止积分饱和
	if(iincpid_ch1>Umax)iincpid_ch1=Umax;
	else if(iincpid_ch1<Umin)iincpid_ch1=Umin;
	if(iincpid_ch2>Umax)iincpid_ch2=200;
	else if(iincpid_ch2<Umin)iincpid_ch2=Umin;
	
	PIDx->Sum_Error1+=iincpid_ch1;
	PIDx->Sum_Error2+=iincpid_ch2;
	
	//输出值限幅（根据PWM设定值来确定限幅，Pmax占空比最大计数值）
	if(PIDx->Sum_Error1>=Pmax)PIDx->Sum_Error1=Pmax;
	else if(PIDx->Sum_Error1<=Pmin)PIDx->Sum_Error1=Pmin;
	if(PIDx->Sum_Error2>=Pmax)PIDx->Sum_Error2=Pmax;
	else if(PIDx->Sum_Error2<=Pmin)PIDx->Sum_Error2=Pmin;
	
	TIM3->CCR3 = PIDx->Sum_Error1;	
	TIM3->CCR4 = PIDx->Sum_Error2;
}

