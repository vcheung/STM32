/*******************************************************************************
 * �ļ���  ��SysTick.c
 * ����    ��SysTick ϵͳ�δ�ʱ��10us�жϺ�����,�ж�ʱ����������ã�
 *           ���õ��� 1us 10us 1ms �жϡ�         
 * ʵ��ƽ̨��stm32f103c8t6
 * Ӳ�����ӣ�-----------------
 *          |                 |
 *          |      ��         |
 *          |                 |
 *           -----------------
 * ��汾  ��ST3.5.0
 * ����    ��zhangwj 
**********************************************************************************/
#include "SysTick.h"

static __IO u32 TimingDelay;

/*
 * ������  ��SysTick_Init
 * ����    ������ϵͳ�δ�ʱ�� SysTick
 * ����    ����
 * ���    ����
 * ����    ���ⲿ����
 * ��ʱ���ã�����Ϊ10us�ж�һ�� 
 */
 void SysTick_Init(void)
{
	/* SystemCoreClock / 1000    1ms�ж�һ��
	 * SystemCoreClock / 100000	 10us�ж�һ��
	 * SystemCoreClock / 1000000 1us�ж�һ��
	 */
	
	if (SysTick_Config(SystemCoreClock / 100000))	// ST3.5.0��汾_װ��ɹ�����0
	{ 
		/* Capture error */ 
		while (1);
	}

	/* �رյδ�ʱ����	���ڵ��� SysTick_Config()����֮��SysTick ��ʱ���ͱ������ˣ�������
	�ڳ�ʼ����ʱ�򲢲�ϣ����������������Ҫ��ʱ���ٿ�����	*/ 
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}

/*
 * ��������Delay_us
 * ����  ��us��ʱ����10usΪһ����λ
 * ����  ��- nTime
 * ���  ����
 * ����  ��Delay_us( 1 )��ʵ�ֵ���ʱΪ 1 * 10us = 10us
 *       ���ⲿ����
 */
void Delay_us(__IO u32 nTime)
{ 
	TimingDelay = nTime;	

	// ʹ�ܵδ�ʱ��  
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;

	while(TimingDelay != 0);
}

/*
 * ��������TimingDelay_Decrement
 * ����  ����ȡ���ĳ���
 * ����  ����
 * ���  ����
 * ����  ���� SysTick �жϺ��� SysTick_Handler()����
 */  
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}
