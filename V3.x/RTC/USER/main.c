/******************************************************************************
 * 文件名  ：main.c
 * 描述    ：利用STM32的RTC实现一个简易的电子时钟。在超级终端中显示时间值。
 *           显示格式为 Time: XX:XX:XX(时：分：秒)，当时间
 *           计数为：23：59：59 时将刷新为：00：00：00。         
 * 实验平台：野火STM32开发板
 * 库版本  ：ST3.5.0
 * 调用    ：调用外部函数USART1_Config()，ADC1_Init()，printf
 *
 * 作者    ：zhangwj 
**********************************************************************************/
#include "stm32f10x.h"
#include "rtc.h"
#include "usart1.h"


int main(void)
{
	/* USART1 config */
	USART1_Config();
	
	/* 配置RTC秒中断优先级 */
	NVIC_Configuration();

	printf( "\r\n This is a RTC demo...... \r\n" );

	/* 通过读取后备寄存器中的值来判断是否为第一次使用RTC，
	 * 0xA4A5只是我们自己随便规定的一个数 */
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	{
		/* Backup data register value is not correct or not yet programmed (when
		the first time the program is executed) */
		printf("\r\nThis is a RTC demo!\r\n");
		printf("\r\n\n RTC not yet configured....");
		
		/* RTC Configuration */
		RTC_Configuration();
		
		printf("\r\n RTC configured....");
		
		/* Adjust time by values entred by the user on the hyperterminal */
		Time_Adjust();
		
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	}
	else
	{
		/* Check if the Power On Reset flag is set
		 * 查stm32参考手册6.3.10，控制/状态寄存器(RCC_CSR)的上电/掉电复位标志PORRST 
		 * PORRST = 0：无上电/掉电复位发生；1：发生上电/掉电复位 */
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
			printf("\r\n\n Power On Reset occurred....");
		}
		/* Check if the Pin Reset flag is set
		 * NRST引脚复位标志PINRST
		 * PINRST = 0：无NRST引脚复位发生；1：发生NRST引脚复位 */
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
		{
			printf("\r\n\n External Reset occurred....");
		}
		
		printf("\r\n No need to configure RTC....");
		/* Wait for RTC registers synchronization */
		RTC_WaitForSynchro();
		
		/* Enable the RTC Second
		 * 掉电或复位后RTC的中断允许位为会被恢复到默认值，而不会像RTC其他的
		 * 配置位掉电后也会保持不变，如果我们不是第一次使用RTC，上电后系统不
		 * 会运行上面的if句，如果再没有下面的开RTC中断语句，那么RTC不会进入
		 * 中断，当然也就不会执行中断中的中断服务函数 */
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
	}
	
	#ifdef RTCClockOutput_Enable
	/* Enable PWR and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);
	
	/* Disable the Tamper Pin */
	BKP_TamperPinCmd(DISABLE); /* To output RTCCLK/64 on Tamper pin, the tamper
	             functionality must be disabled */
	
	/* Enable RTC Clock Output on Tamper Pin */
	BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);
	#endif
	
	/* Clear reset flags */
	RCC_ClearFlag();
	
	/* Display time in infinite loop */
	Time_Show();
	
	while (1)
	{
	
	}
}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
