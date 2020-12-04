#ifndef _incrementalpid_H_
#define _incrementalpid_H_

#include<stm32f10x.h>
//调节值限幅，防止积分饱和
#define Umax 200
#define Umin -200

//数出值限幅(根据pwm设定值来确定限幅，Pmax占空比最大计数值)
#define Pmax 999


#define Pmin 10

/*目前这三个参数是根据MINERTIA MOTOR（电机）空载时的比较好的值*/
#define Kp 0.6//pid调节的比例常数。调节Kp的范围一般W为:1-10。
#define Ti 0.1//pid调节的积分常数。调节Ti的范围一般W为:0.05-0。
#define Td 0.2//pid调节的微分时间常数。调节Ti的范围一般W为:0-0.01。
#define T 0.099//采样周期

#define Kpp Kp*(1+(T/Ti)+(Td/T))
#define Ki  (-Kp)*(1+(2*Td/T))
#define Kd  0.01//Kp*Td/T
#define Error_min 1//误差的阙值，小于这个数的时候不做pid调整
                   //避免误差较小时频繁调节引起震荡
typedef struct INC_PID1 {              //增量式pid的结构体定义

	int  SetPoint;           //设定值

//	int  Proportion;         // Proportion 比例系数

//	int  Integral;           // Integral   积分系数

//	int  Derivative;         // Derivative  微分系数
	
	int  Now_Error;           // Error[0]    当前误差

	int  LastError;          // Error[-1]  前一拍误差

	int  PreError;           // Error[-2]  前两拍误差

} INC_PID;

extern INC_PID Inc_Pid;//全局可用								 
extern int INC_Pwm_out;//把这个值赋值给相关的占空比变量。
                       //如CCR1_Val(CCR1_Va2\CCR3_Val\CCR4_Val) = INC_Pwm_out;
											 //CCR1_Val在pwm文件中										 
void INC_Pid_Init(void);//初始化Inc_Pid结构体成员变量
void INC_Pid_Ctrl(int Speed,int feb);

#endif

