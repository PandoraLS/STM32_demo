/******************** (C) COPYRIGHT 2015 ****************************************
 * �ļ���  ��incrementalpid.c
 * ����    ������ʽpid     
 * ʵ��ƽ̨��ipc8ѧϰ��
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
//#include "usart.h"
#include "pwm1.h"
#include "OLED.h"	

INC_PID PID1;//ȫ�ֿ���	

/*
 * ��������void INC_Pid_Init(void)
 * ����  ����ʼ��Inc_Pid�ṹ���Ա����
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void INC_Pid_Init(void)
{
  PID1.Last_Error=0;   //Error[-1]  ǰһ�����
	PID1.Prev_Error=0;   //Error[-2]  ǰ�������
	PID1.Sum_Error1=0;    //�ۼ����
	PID1.Sum_Error2=0;    //�ۼ����
	
	PID1.Proportion=0;   //Proportion ����ϵ��
	PID1.Integral=0;     //���ֳ���
	PID1.Derivative=0;   //΢�ֳ���
	
	PID1.Set_Point=0;    //�趨Ŀ���ٶ�
}

/*
 * ��������void PID_setpoint(INC_PID*PIDx,u16 setvalue)
 * ����  ���趨PID��Ԥ��ֵ
 * ����  ��PIDx���Ǹ�PID����Ԫ��setvalueΪPID��Ԥ��ֵ
 * ���  ����
 * ����  ���ⲿ����
 */
void PID_Setpoint(INC_PID*PIDx,int setvalue)
{
  PIDx->Set_Point=setvalue; 
}

/*
 * ��������void PID_set(float pp,float ii,float dd)
 * ����  ���趨PID Kp Ki Kd����������ֵ
 * ����  ��Kp Ki Kd����������ֵ
 * ���  ����
 * ����  ���ⲿ����
 */
void PID_Set(float pp,float ii,float dd)
{
  PID1.Proportion=pp;
  PID1.Integral=ii;
  PID1.Derivative=dd; 
}

/*
 * ��������void set_speed(float W1)
 * ����  ���趨������ٶ�
 * ����  ��
 * ���  ����
 * ����  ���ⲿ����
 */
void INC_PID_Calc(INC_PID *PIDx,int nextpoint)
{
	int iError,iincpid_ch1,iincpid_ch2;
	iError=PIDx->Set_Point-nextpoint;  //��ǰ���
	
	iincpid_ch1=					    //��������
	PIDx->Proportion*iError	        //e[k]��
	-PIDx->Integral*PIDx->Last_Error	  //e[k-1]
	+PIDx->Derivative*PIDx->Prev_Error;//e[k-2]
	PIDx->Prev_Error=PIDx->Last_Error; //���������ڼ���
	PIDx->Last_Error=iError;
	
	iincpid_ch2=					    //��������
	-PIDx->Proportion*iError	        //e[k]��
	+PIDx->Integral*PIDx->Last_Error	  //e[k-1]
	-PIDx->Derivative*PIDx->Prev_Error;//e[k-2]
	
	PIDx->Prev_Error=PIDx->Last_Error; //���������ڼ���
	PIDx->Last_Error=iError;
	//����ֵ�޷�����ֹ���ֱ���
	if(iincpid_ch1>Umax)iincpid_ch1=Umax;
	else if(iincpid_ch1<Umin)iincpid_ch1=Umin;
	if(iincpid_ch2>Umax)iincpid_ch2=200;
	else if(iincpid_ch2<Umin)iincpid_ch2=Umin;
	
	PIDx->Sum_Error1+=iincpid_ch1;
	PIDx->Sum_Error2+=iincpid_ch2;
	
	//���ֵ�޷�������PWM�趨ֵ��ȷ���޷���Pmaxռ�ձ�������ֵ��
	if(PIDx->Sum_Error1>=Pmax)PIDx->Sum_Error1=Pmax;
	else if(PIDx->Sum_Error1<=Pmin)PIDx->Sum_Error1=Pmin;
	if(PIDx->Sum_Error2>=Pmax)PIDx->Sum_Error2=Pmax;
	else if(PIDx->Sum_Error2<=Pmin)PIDx->Sum_Error2=Pmin;
	
	TIM3->CCR3 = PIDx->Sum_Error1;	
	TIM3->CCR4 = PIDx->Sum_Error2;
}

