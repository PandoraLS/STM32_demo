#ifndef _incrementalpid_H_
#define _incrementalpid_H_

#include<stm32f10x.h>
//����ֵ�޷�����ֹ���ֱ���
#define Umax 200
#define Umin -200

//����ֵ�޷�(����pwm�趨ֵ��ȷ���޷���Pmaxռ�ձ�������ֵ)
#define Pmax 999


#define Pmin 10

/*Ŀǰ�����������Ǹ���MINERTIA MOTOR�����������ʱ�ıȽϺõ�ֵ*/
#define Kp 0.6//pid���ڵı�������������Kp�ķ�Χһ��WΪ:1-10��
#define Ti 0.1//pid���ڵĻ��ֳ���������Ti�ķ�Χһ��WΪ:0.05-0��
#define Td 0.2//pid���ڵ�΢��ʱ�䳣��������Ti�ķ�Χһ��WΪ:0-0.01��
#define T 0.099//��������

#define Kpp Kp*(1+(T/Ti)+(Td/T))
#define Ki  (-Kp)*(1+(2*Td/T))
#define Kd  0.01//Kp*Td/T
#define Error_min 1//������ֵ��С���������ʱ����pid����
                   //��������СʱƵ������������
typedef struct INC_PID1 {              //����ʽpid�Ľṹ�嶨��

	int  SetPoint;           //�趨ֵ

//	int  Proportion;         // Proportion ����ϵ��

//	int  Integral;           // Integral   ����ϵ��

//	int  Derivative;         // Derivative  ΢��ϵ��
	
	int  Now_Error;           // Error[0]    ��ǰ���

	int  LastError;          // Error[-1]  ǰһ�����

	int  PreError;           // Error[-2]  ǰ�������

} INC_PID;

extern INC_PID Inc_Pid;//ȫ�ֿ���								 
extern int INC_Pwm_out;//�����ֵ��ֵ����ص�ռ�ձȱ�����
                       //��CCR1_Val(CCR1_Va2\CCR3_Val\CCR4_Val) = INC_Pwm_out;
											 //CCR1_Val��pwm�ļ���										 
void INC_Pid_Init(void);//��ʼ��Inc_Pid�ṹ���Ա����
void INC_Pid_Ctrl(int Speed,int feb);

#endif

