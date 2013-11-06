/*******************************************************************************
 * 文件名  ：rtc.c
 * 描述    ：利用STM32的RTC实现一个简易的电子时钟。
 *           显示格式为 Time: XX : XX : XX(时：分：秒)。         
 * 实验平台：stm32f103c8t6
 * 硬件连接：-------------------
 *          |                   |
 *          | VBAT引脚需外接电池|
 *          |                   |
 *           -----------------
 * 库版本  ：ST3.5.0
 * 参考资料：超透彻的STM32讲解资料——RTC时钟.docx
 * 作者    ：zhangwj 
**********************************************************************************/
#include "rtc.h"
#include "stdio.h"

/* 秒中断标志，进入秒中断时置1，当时间被刷新之后清0 */
__IO uint32_t TimeDisplay;	

/*
 * 函数名：NVIC_Configuration
 * 描述  ：配置RTC秒中断的主中断优先级为1，次优先级为0
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* 配置优先级组_第1组：最高1位用来配置抢占优先级，低3位用来配置响应优先级。 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	/* Enable the RTC Interrupt */
	/* 选择RTC的IRQ通道 */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*
 * 函数名：RTC_Configuration
 * 描述  ：配置RTC
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void RTC_Configuration(void)
{
	/* Enable PWR（电源控制） and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	/* Allow access to BKP Domain 
	 * 允许进入后备区域进行写操作，因为后备寄存器中放的是重要的数据，默认是不允许往里面写入值的 */
	PWR_BackupAccessCmd(ENABLE);
	
	/* Reset Backup Domain 
	 *将后背寄存器的寄存器值设为默认值 */
	BKP_DeInit();
	
	/* Enable LSE 
	 *打开外部低速晶振，RTC可以选择的时钟源是外部和内部低速晶振及外部高速晶振，这里我们选择外部低速晶振32768HZ */
	RCC_LSEConfig(RCC_LSE_ON);
	/* Wait till LSE is ready 
	 *等待外部低速晶振准备就序 */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{}
	
	/* Select LSE as RTC Clock Source 
	 * 选择外部低速晶振为RTC的时钟源 */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
	/* Enable RTC Clock */
	RCC_RTCCLKCmd(ENABLE);
	
	/* Wait for RTC registers synchronization
	 * 等待RTC寄存器与RTC的APB时钟同步 */
	RTC_WaitForSynchro();
	
	/* Wait until last write operation on RTC registers has finished
	 * 等待上次对RTC寄存器配置完成 */
	RTC_WaitForLastTask();
	
	/* Enable the RTC Second
	 * 使能RTC中断_秒中断 */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	
	/* Set RTC prescaler: set RTC period to 1sec
	 * 设置RTC的预分频值，因为外部低速晶振是32768，所以选择RTC计数器计数频率
	 * = RTCCLK/RTC_PR =(32.768 KHz)/(32767+1) */
	RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
}

/*
 * 函数名：USART_Scanf
 * 描述  ：串口从超级终端中获取数值
 * 输入  ：- value 用户在超级终端中输入的数值
 * 输出  ：无
 * 调用  ：内部调用，被Time_Regulate(void)调用
 */ 
uint8_t USART_Scanf(uint32_t value)
{
	uint32_t index = 0;
	uint32_t tmp[2] = {0, 0};
	
	while (index < 2)
	{
		/* Loop until RXNE = 1
		 * 等待数据接受完成，当接收引脚有数据时，状态寄存器的USART_FLAG_RXNE就会为1 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
		{}
		/* 将数据给数组tmp, 从串口终端里面输进去的数是ASCII码值 */
		tmp[index++] = (USART_ReceiveData(USART1));
		/* 从串口终端里面输进去的数是ASCII码值
		 * 判断接受到的数据是否在0到9之间 */
		if ((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39))
		{
			printf("\n\rPlease enter valid number between 0 and 9");
			index--;
		}
	}
	/* Calculate the Corresponding value
	 * 将接受的两个数据组成一个两位数 */
	index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10);
	/* Checks
	 * 判断组成后的值是否有无，比如组成的小时不能超过23，分秒不能超过59，
	 * value 即为23，或59 */
	if (index > value)
	{
		printf("\n\rPlease enter valid number between 0 and %d", value);
		return 0xFF;
	}
	return index;
}

/*
 * 函数名：Time_Regulate
 * 描述  ：返回用户在超级终端中输入的时间值，并将值储存在
 *         RTC 计数寄存器中。
 * 输入  ：无
 * 输出  ：用户在超级终端中输入的时间值，单位为 s
 * 调用  ：被Time_Adjust调用，调用USART_Scanf
 */
uint32_t Time_Regulate(void)
{
	uint32_t Tmp_HH = 0xFF, Tmp_MM = 0xFF, Tmp_SS = 0xFF;
	
	printf("\r\n==============Time Settings=====================================");
	printf("\r\n  Please Set Hours");
	
	while (Tmp_HH == 0xFF)
	{
		/* 将串口接受的数据给Tmp_HH ，23便是传递给USART_Scanf（value）中的形参value的 */
		Tmp_HH = USART_Scanf(23);
	}
	printf(":  %d", Tmp_HH);
	printf("\r\n  Please Set Minutes");
	while (Tmp_MM == 0xFF)
	{
		Tmp_MM = USART_Scanf(59);
	}
	printf(":  %d", Tmp_MM);
	printf("\r\n  Please Set Seconds");
	while (Tmp_SS == 0xFF)
	{
		Tmp_SS = USART_Scanf(59);
	}
	printf(":  %d", Tmp_SS);
	
	/* Return the value to store in RTC counter register
	 * 将时分秒转换成秒放入RTC计数器中 */
	return((Tmp_HH*3600 + Tmp_MM*60 + Tmp_SS));
}

/*
 * 函数名：Time_Adjust
 * 描述  ：时间调节
 * 输入  ：无
 * 输出  ：无
 * 调用  ：主函数调用
 */
void Time_Adjust(void)
{
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	/* Change the current time
	 * 将设置的初始时间值装入RTC计数器，RTC开始运行时，计数器里面的值
	 * 会在初始值的基础上自动一秒加1次 */
	RTC_SetCounter(Time_Regulate());
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
}

/*
 * 函数名：Time_Display
 * 描述  ：显示当前时间值
 * 输入  ：-TimeVar RTC计数值，单位为 s
 * 输出  ：无
 * 调用  ：被Time_Show调用
 */	
void Time_Display(uint32_t TimeVar)
{
	uint32_t THH = 0, TMM = 0, TSS = 0;
	
	/* Compute  hours
	 * 将极速器中的值换成时分秒分别赋给THH ，TMM，TSS */
	THH = TimeVar / 3600;
	/* Compute minutes */
	TMM = (TimeVar % 3600) / 60;
	/* Compute seconds */
	TSS = (TimeVar % 3600) % 60;
	
	/* 将时间传到上位机上显示 */
	printf(" Time: %0.2d:%0.2d:%0.2d\r", THH, TMM, TSS);
}

/*
 * 函数名：Time_Show
 * 描述  ：在超级终端中显示当前时间值
 * 输入  ：无
 * 输出  ：无
 * 调用  ：主函数调用，调用函数Time_Display(uint32_t TimeVar)
 */   
void Time_Show(void)
{
	printf("\n\r");
	
	/* Infinite loop */
	while (1)
	{
		/* If 1s has paased
		 * 只有TimeDisplay 为1时才在上位机上显示时间，
		 * TimeDisplay是用户自己定义的标示符，在RTC中断
		 * 服务函数中被置1，即1秒被置1一次，也就是1秒才让串口向上位机发一次时间 */
		if (TimeDisplay == 1)
		{
			/* Display current time
			 * 将RTC计数器中的值传给函数 Time_Display */
			Time_Display(RTC_GetCounter());
			TimeDisplay = 0;
		}
	}
}

