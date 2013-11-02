/****************************************************************************
 * 文件名  ：exti.c
 * 描述    ：I/O线中断应用函数库         
 * 实验平台：stm32f103c8t6
 * 硬件连接：-------------------------
 *          | PA0 - key1              |
 *          | PA1 - key2(本实验没用到)|
 *          |                         |
 *           -------------------------
 * 库版本  ：ST3.0.0
 *
 * 作者    ：zhangwj 
**********************************************************************************/
#include "exti.h"

/*
 * 函数名：NVIC_Configuration
 * 描述  ：配置嵌套向量中断控制器NVIC
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	  
	/* 配置优先级组_第1组：最高1位用来配置抢占优先级，低3位用来配置响应优先级。 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	  
	/* 配置P[A|B|C|D|E]0为中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*
 * 函数名：EXTI_PA0_Config
 * 描述  ：配置 PA0 为线中断口，并设置中断优先级
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void EXTI_PA0_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/* 使能EXTIx线时钟和第二功能AFIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);

	/* 配置EXTIx中断优先级 */
	NVIC_Configuration();

	/* 配置EXTI中断线I/O */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 //上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 选定要配置为EXTI的I/O线和I/O工作模式 */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); 	//把GPIOA，Pin0设置为EXTI输入线
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;	//选择EXTI_Line0线进行配置，因为按键的PA0连接到了EXTI_Line0。
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	//把EXTI_Line0的模式设置为为中断模式EXTI_Mode_Interrupt。
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //中断触发方式_下降沿中断
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;	//中断线路使能
	EXTI_Init(&EXTI_InitStructure); 

	/* EXTI工作模式配置 */
}
