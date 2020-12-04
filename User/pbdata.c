#include "stm32f10x.h"
#include "pbdata.h"

void delay_ms(uint32 nms)
{
	uint32 temp;
	SysTick->LOAD = 9000*nms;
	SysTick->VAL = 0X00;
	SysTick->CTRL = 0X01;
	do
	{
	temp = 	SysTick->CTRL;
	}while((temp&0X01)&&(!(temp&(1<<16))));
	SysTick->CTRL = 0X00;
	SysTick->VAL = 0X00;
}

void delay_us(uint32 nus)
{
	uint32 temp;
	SysTick->LOAD = 9*nus;
	SysTick->VAL = 0X00;
	SysTick->CTRL = 0X01;
	do
	{
	temp = 	SysTick->CTRL;
	}while((temp&0X01)&&(!(temp&(1<<16))));
	SysTick->CTRL = 0X00;
	SysTick->VAL = 0X00;
}

