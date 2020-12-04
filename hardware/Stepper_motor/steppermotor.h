#ifndef _steppermotor_H
#define _steppermotor_H
#include "stm32f10x.h"
#include "OLED.h"
#include "delay.h"

/*此部分的定义是对于步进电机转速要求较高的时候用的，一般频率高于2000HZ时候就用它*/
typedef struct{
   int  BandF;//频段频率
	 long BandP;//频段脉冲
	 long TotalP;//累计脉冲
}BAND;//此结构体是为步进电机要做高速运动而准备的结构体

extern BAND BAND_C;//此结构体是为步进电机要做高速运动而准备的结构体

/*此部分是为单片机产生固定脉冲和要求单片机运行速度不是很高的时候用的，一般步进电机的运行频率在2000HZ以内就可以直接用下面的*/
#define  MOTOR_COTR_PORT    GPIOC
#define  MOTOR_COTR_DIR     GPIO_Pin_4
#define  MOTOR_COTR_RCC     RCC_APB2Periph_GPIOC
#define compile_assert(exp) {extern char lxyppc_at_163_dot_com[(exp) ? 1:-1];}
#define     _GetInternalTrigger(S,M) \
   (S == TIM1 ? (M == TIM5 ? TIM_TS_ITR0 : (M == TIM2 ? TIM_TS_ITR1 : (M == TIM3 ? TIM_TS_ITR2 :(M == TIM4 ? TIM_TS_ITR3 : (u16)-1)))): \
   (S == TIM8 ? (M == TIM1 ? TIM_TS_ITR0 : (M == TIM2 ? TIM_TS_ITR1 : (M == TIM4 ? TIM_TS_ITR2 :(M == TIM5 ? TIM_TS_ITR3 : (u16)-1)))): \
   (S == TIM2 ? (M == TIM1 ? TIM_TS_ITR0 : (M == TIM8 ? TIM_TS_ITR1 : (M == TIM3 ? TIM_TS_ITR2 :(M == TIM4 ? TIM_TS_ITR3 : (u16)-1)))): \
   (S == TIM3 ? (M == TIM1 ? TIM_TS_ITR0 : (M == TIM2 ? TIM_TS_ITR1 : (M == TIM5 ? TIM_TS_ITR2 :(M == TIM4 ? TIM_TS_ITR3 : (u16)-1)))): \
   (S == TIM4 ? (M == TIM1 ? TIM_TS_ITR0 : (M == TIM2 ? TIM_TS_ITR1 : (M == TIM3 ? TIM_TS_ITR2 :(M == TIM8 ? TIM_TS_ITR3 : (u16)-1)))): \
   (S == TIM5 ? (M == TIM2 ? TIM_TS_ITR0 : (M == TIM3 ? TIM_TS_ITR1 : (M == TIM4 ? TIM_TS_ITR2 :(M == TIM8 ? TIM_TS_ITR3 : (u16)-1)))): \
   (u16)-1))))))
#define     GetInternalTrigger(SlaveTimer,MasterTimer)      _GetInternalTrigger(SlaveTimer,MasterTimer)

#define Stepper_motor_pulse     RCC_APB2Periph_GPIOA
#define Stepper_motor_pulse_G 	GPIOA
#define Stepper_motor_pulse_Out	GPIO_Pin_8//脉冲产生引脚

#define Stepper_motor_time      RCC_APB2Periph_GPIOB
#define Stepper_motor_time_G	  GPIOB
#define Stepper_motor_time_Out	GPIO_Pin_1//提取脉冲数引脚

extern int waveNumber;
extern int TIM3_Period;//TIM3设置预分频数
extern int TIM3_waveNumber;//设置跳变值，当计数器计数到这个值的给定公式时，电平发生跳变
extern int TIM1_Period;//TIM1设置预分频数
extern int TIM1_waveNumber;////设置跳变值，当计数器计数到这个值的给定公式时，电平发生跳变

void Stepper_Motor_Init(void);
void stepper_low_speed_Move(signed int step,unsigned max_speed);
void stop_tim3(void);
void BAND_C_Init(void);//此结构体是为步进电机要做高速运动而准备的结构体
void stepper_high_speed_Move(signed int step,unsigned max_speed);
void BandTabCreate_accelerate(long StaF,long MovF,signed char stepper_direction);
void BandTabCreate_decelerate(long StaF,long MovF);//StaF为起使频率，MovF为目标频率,symbol如果为大于0的数表示加速过程，否则表示减速过程
void Stepper_Init(void);
#endif
