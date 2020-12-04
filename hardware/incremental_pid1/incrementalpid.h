#ifndef _incrementalpid_H_
#define _incrementalpid_H_

#include<stm32f10x.h>
//����ֵ�޷�����ֹ���ֱ���
#define Umax 400
#define Umin -400

//���ֵ�޷�������PWM�趨ֵ��ȷ���޷���Pmaxռ�ձ�������ֵ��
#define Pmax 4500
#define Pmin 0

typedef struct INC_PID1 
{            //����ʽpid�Ľṹ�嶨��

	int   Set_Point;     //�趨Ŀ���ٶ�

	int   Sum_Error1;    // �ۼ����
	
	int   Sum_Error2;    // �ۼ����

	float Proportion ;  // ��������

  float Integral ;    //���ֳ���
	
	float Derivative;   //΢�ֳ���

	int   Last_Error;   // Error[-1]  ǰһ�����

	int   Prev_Error;   // Error[-2]  ǰ�������

} INC_PID;

extern INC_PID PID1;//ȫ�ֿ���								 
extern int INC_Pwm_out;//�����ֵ��ֵ����ص�ռ�ձȱ�����
//void Set_Speed(INC_PID *PIDx,int encoder_count1_now_v);
void INC_Pid_Init(void);
void PID_Setpoint(INC_PID*PIDx,int setvalue);
void PID_Set(float pp,float ii,float dd);
void INC_PID_Calc(INC_PID *PIDx,int nextpoint);
#endif

