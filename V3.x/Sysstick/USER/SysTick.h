#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

void SysTick_Init(void);
void Delay_us(__IO u32 nTime);	//nTime��volatile��������ʱ���ܷ����仯��

#endif	/*	__SYSTICK_H	*/
