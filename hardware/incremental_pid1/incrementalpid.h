#ifndef _incrementalpid_H_
#define _incrementalpid_H_

#include<stm32f10x.h>
//调节值限幅，防止积分饱和
#define Umax 400
#define Umin -400

//输出值限幅（根据PWM设定值来确定限幅，Pmax占空比最大计数值）
#define Pmax 4500
#define Pmin 0

typedef struct INC_PID1 
{            //增量式pid的结构体定义

	int   Set_Point;     //设定目标速度

	int   Sum_Error1;    // 累计误差
	
	int   Sum_Error2;    // 累计误差

	float Proportion ;  // 比例常数

  float Integral ;    //积分常数
	
	float Derivative;   //微分常数

	int   Last_Error;   // Error[-1]  前一拍误差

	int   Prev_Error;   // Error[-2]  前两拍误差

} INC_PID;

extern INC_PID PID1;//全局可用								 
extern int INC_Pwm_out;//把这个值赋值给相关的占空比变量。
//void Set_Speed(INC_PID *PIDx,int encoder_count1_now_v);
void INC_Pid_Init(void);
void PID_Setpoint(INC_PID*PIDx,int setvalue);
void PID_Set(float pp,float ii,float dd);
void INC_PID_Calc(INC_PID *PIDx,int nextpoint);
#endif

