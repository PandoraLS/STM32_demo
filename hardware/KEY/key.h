#ifndef __KEY_H
#define __KEY_H	 
#include "stm32f10X.h"

#define dey		5		//������ʱ������������ʱ��

/*���󰴼�*/
void KEY_Matrix_Init(void);	//���󰴼���ʼ��
u8 Read_KeyValue_Matrix(void);//��ȡ��ֵ
void Key_Value_Show(void);		//��ȡ��OLED��ʾ��ֵ

#endif
