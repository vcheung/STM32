/*******************************************************************************
 * 文件名  ：SysTick.c
 * 描述    ：SysTick 系统滴答时钟10us中断函数库,中断时间可自由配置，
 *           常用的有 1us 10us 1ms 中断。         
 * 实验平台：stm32f103c8t6
 * 硬件连接：-----------------
 *          |                 |
 *          |      无         |
 *          |                 |
 *           -----------------
 * 库版本  ：ST3.5.0
 * 作者    ：zhangwj 
**********************************************************************************/
#include "SysTick.h"

static __IO u32 TimingDelay;

/*
 * 函数名  ：SysTick_Init
 * 描述    ：启动系统滴答定时器 SysTick
 * 输入    ：无
 * 输出    ：无
 * 调用    ：外部调用
 * 延时设置：设置为10us中断一次 
 */
 void SysTick_Init(void)
{
	/* SystemCoreClock / 1000    1ms中断一次
	 * SystemCoreClock / 100000	 10us中断一次
	 * SystemCoreClock / 1000000 1us中断一次
	 */
	
	if (SysTick_Config(SystemCoreClock / 100000))	// ST3.5.0库版本_装入成功返回0
	{ 
		/* Capture error */ 
		while (1);
	}

	/* 关闭滴答定时器。	由于调用 SysTick_Config()函数之后，SysTick 定时器就被开启了，但我们
	在初始化的时候并不希望这样，而是在需要的时候再开启。	*/ 
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}

/*
 * 函数名：Delay_us
 * 描述  ：us延时程序，10us为一个单位
 * 输入  ：- nTime
 * 输出  ：无
 * 调用  ：Delay_us( 1 )则实现的延时为 1 * 10us = 10us
 *       ：外部调用
 */
void Delay_us(__IO u32 nTime)
{ 
	TimingDelay = nTime;	

	// 使能滴答定时器  
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;

	while(TimingDelay != 0);
}

/*
 * 函数名：TimingDelay_Decrement
 * 描述  ：获取节拍程序
 * 输入  ：无
 * 输出  ：无
 * 调用  ：在 SysTick 中断函数 SysTick_Handler()调用
 */  
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}
