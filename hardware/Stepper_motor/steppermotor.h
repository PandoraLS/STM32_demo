#ifndef _steppermotor_H
#define _steppermotor_H
#include "stm32f10x.h"
#include "OLED.h"
#include "delay.h"

/*�˲��ֵĶ����Ƕ��ڲ������ת��Ҫ��ϸߵ�ʱ���õģ�һ��Ƶ�ʸ���2000HZʱ�������*/
typedef struct{
   int  BandF;//Ƶ��Ƶ��
	 long BandP;//Ƶ������
	 long TotalP;//�ۼ�����
}BAND;//�˽ṹ����Ϊ�������Ҫ�������˶���׼���Ľṹ��

extern BAND BAND_C;//�˽ṹ����Ϊ�������Ҫ�������˶���׼���Ľṹ��

/*�˲�����Ϊ��Ƭ�������̶������Ҫ��Ƭ�������ٶȲ��Ǻܸߵ�ʱ���õģ�һ�㲽�����������Ƶ����2000HZ���ھͿ���ֱ���������*/
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
#define Stepper_motor_pulse_Out	GPIO_Pin_8//�����������

#define Stepper_motor_time      RCC_APB2Periph_GPIOB
#define Stepper_motor_time_G	  GPIOB
#define Stepper_motor_time_Out	GPIO_Pin_1//��ȡ����������

extern int waveNumber;
extern int TIM3_Period;//TIM3����Ԥ��Ƶ��
extern int TIM3_waveNumber;//��������ֵ�������������������ֵ�ĸ�����ʽʱ����ƽ��������
extern int TIM1_Period;//TIM1����Ԥ��Ƶ��
extern int TIM1_waveNumber;////��������ֵ�������������������ֵ�ĸ�����ʽʱ����ƽ��������

void Stepper_Motor_Init(void);
void stepper_low_speed_Move(signed int step,unsigned max_speed);
void stop_tim3(void);
void BAND_C_Init(void);//�˽ṹ����Ϊ�������Ҫ�������˶���׼���Ľṹ��
void stepper_high_speed_Move(signed int step,unsigned max_speed);
void BandTabCreate_accelerate(long StaF,long MovF,signed char stepper_direction);
void BandTabCreate_decelerate(long StaF,long MovF);//StaFΪ��ʹƵ�ʣ�MovFΪĿ��Ƶ��,symbol���Ϊ����0������ʾ���ٹ��̣������ʾ���ٹ���
void Stepper_Init(void);
#endif
