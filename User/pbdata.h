#ifndef _pbdata_H
#define _pbdata_H

#define uint32 unsigned long
#define uint16 unsigned int
#define uint8 unsigned char

#include "stm32f10x.h"

//�������

extern u8 dt;

//���庯��

void delay_ms(uint32 nms);
void delay_us(uint32 nus);

#endif
