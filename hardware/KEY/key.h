#ifndef __KEY_H
#define __KEY_H	 
#include "stm32f10X.h"

#define dey		5		//按键延时数，调节消抖时间

/*矩阵按键*/
void KEY_Matrix_Init(void);	//矩阵按键初始化
u8 Read_KeyValue_Matrix(void);//读取键值
void Key_Value_Show(void);		//读取并OLED显示键值

#endif
