#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

void SysTick_Init(void);
void Delay_us(__IO u32 nTime);	//nTime是volatile变量，随时可能发生变化的

#endif	/*	__SYSTICK_H	*/
