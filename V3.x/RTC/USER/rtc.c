/*******************************************************************************
 * �ļ���  ��rtc.c
 * ����    ������STM32��RTCʵ��һ�����׵ĵ���ʱ�ӡ�
 *           ��ʾ��ʽΪ Time: XX : XX : XX(ʱ���֣���)��         
 * ʵ��ƽ̨��stm32f103c8t6
 * Ӳ�����ӣ�-------------------
 *          |                   |
 *          | VBAT��������ӵ��|
 *          |                   |
 *           -----------------
 * ��汾  ��ST3.5.0
 * �ο����ϣ���͸����STM32�������ϡ���RTCʱ��.docx
 * ����    ��zhangwj 
**********************************************************************************/
#include "rtc.h"
#include "stdio.h"

/* ���жϱ�־���������ж�ʱ��1����ʱ�䱻ˢ��֮����0 */
__IO uint32_t TimeDisplay;	

/*
 * ��������NVIC_Configuration
 * ����  ������RTC���жϵ����ж����ȼ�Ϊ1�������ȼ�Ϊ0
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* �������ȼ���_��1�飺���1λ����������ռ���ȼ�����3λ����������Ӧ���ȼ��� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	/* Enable the RTC Interrupt */
	/* ѡ��RTC��IRQͨ�� */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*
 * ��������RTC_Configuration
 * ����  ������RTC
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void RTC_Configuration(void)
{
	/* Enable PWR����Դ���ƣ� and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	/* Allow access to BKP Domain 
	 * ���������������д��������Ϊ�󱸼Ĵ����зŵ�����Ҫ�����ݣ�Ĭ���ǲ�����������д��ֵ�� */
	PWR_BackupAccessCmd(ENABLE);
	
	/* Reset Backup Domain 
	 *���󱳼Ĵ����ļĴ���ֵ��ΪĬ��ֵ */
	BKP_DeInit();
	
	/* Enable LSE 
	 *���ⲿ���پ���RTC����ѡ���ʱ��Դ���ⲿ���ڲ����پ����ⲿ���پ�����������ѡ���ⲿ���پ���32768HZ */
	RCC_LSEConfig(RCC_LSE_ON);
	/* Wait till LSE is ready 
	 *�ȴ��ⲿ���پ���׼������ */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{}
	
	/* Select LSE as RTC Clock Source 
	 * ѡ���ⲿ���پ���ΪRTC��ʱ��Դ */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
	/* Enable RTC Clock */
	RCC_RTCCLKCmd(ENABLE);
	
	/* Wait for RTC registers synchronization
	 * �ȴ�RTC�Ĵ�����RTC��APBʱ��ͬ�� */
	RTC_WaitForSynchro();
	
	/* Wait until last write operation on RTC registers has finished
	 * �ȴ��ϴζ�RTC�Ĵ���������� */
	RTC_WaitForLastTask();
	
	/* Enable the RTC Second
	 * ʹ��RTC�ж�_���ж� */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	
	/* Set RTC prescaler: set RTC period to 1sec
	 * ����RTC��Ԥ��Ƶֵ����Ϊ�ⲿ���پ�����32768������ѡ��RTC����������Ƶ��
	 * = RTCCLK/RTC_PR =(32.768 KHz)/(32767+1) */
	RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
}

/*
 * ��������USART_Scanf
 * ����  �����ڴӳ����ն��л�ȡ��ֵ
 * ����  ��- value �û��ڳ����ն����������ֵ
 * ���  ����
 * ����  ���ڲ����ã���Time_Regulate(void)����
 */ 
uint8_t USART_Scanf(uint32_t value)
{
	uint32_t index = 0;
	uint32_t tmp[2] = {0, 0};
	
	while (index < 2)
	{
		/* Loop until RXNE = 1
		 * �ȴ����ݽ�����ɣ�����������������ʱ��״̬�Ĵ�����USART_FLAG_RXNE�ͻ�Ϊ1 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
		{}
		/* �����ݸ�����tmp, �Ӵ����ն��������ȥ������ASCII��ֵ */
		tmp[index++] = (USART_ReceiveData(USART1));
		/* �Ӵ����ն��������ȥ������ASCII��ֵ
		 * �жϽ��ܵ��������Ƿ���0��9֮�� */
		if ((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39))
		{
			printf("\n\rPlease enter valid number between 0 and 9");
			index--;
		}
	}
	/* Calculate the Corresponding value
	 * �����ܵ������������һ����λ�� */
	index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10);
	/* Checks
	 * �ж���ɺ��ֵ�Ƿ����ޣ�������ɵ�Сʱ���ܳ���23�����벻�ܳ���59��
	 * value ��Ϊ23����59 */
	if (index > value)
	{
		printf("\n\rPlease enter valid number between 0 and %d", value);
		return 0xFF;
	}
	return index;
}

/*
 * ��������Time_Regulate
 * ����  �������û��ڳ����ն��������ʱ��ֵ������ֵ������
 *         RTC �����Ĵ����С�
 * ����  ����
 * ���  ���û��ڳ����ն��������ʱ��ֵ����λΪ s
 * ����  ����Time_Adjust���ã�����USART_Scanf
 */
uint32_t Time_Regulate(void)
{
	uint32_t Tmp_HH = 0xFF, Tmp_MM = 0xFF, Tmp_SS = 0xFF;
	
	printf("\r\n==============Time Settings=====================================");
	printf("\r\n  Please Set Hours");
	
	while (Tmp_HH == 0xFF)
	{
		/* �����ڽ��ܵ����ݸ�Tmp_HH ��23���Ǵ��ݸ�USART_Scanf��value���е��β�value�� */
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
	 * ��ʱ����ת���������RTC�������� */
	return((Tmp_HH*3600 + Tmp_MM*60 + Tmp_SS));
}

/*
 * ��������Time_Adjust
 * ����  ��ʱ�����
 * ����  ����
 * ���  ����
 * ����  ������������
 */
void Time_Adjust(void)
{
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	/* Change the current time
	 * �����õĳ�ʼʱ��ֵװ��RTC��������RTC��ʼ����ʱ�������������ֵ
	 * ���ڳ�ʼֵ�Ļ������Զ�һ���1�� */
	RTC_SetCounter(Time_Regulate());
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
}

/*
 * ��������Time_Display
 * ����  ����ʾ��ǰʱ��ֵ
 * ����  ��-TimeVar RTC����ֵ����λΪ s
 * ���  ����
 * ����  ����Time_Show����
 */	
void Time_Display(uint32_t TimeVar)
{
	uint32_t THH = 0, TMM = 0, TSS = 0;
	
	/* Compute  hours
	 * ���������е�ֵ����ʱ����ֱ𸳸�THH ��TMM��TSS */
	THH = TimeVar / 3600;
	/* Compute minutes */
	TMM = (TimeVar % 3600) / 60;
	/* Compute seconds */
	TSS = (TimeVar % 3600) % 60;
	
	/* ��ʱ�䴫����λ������ʾ */
	printf(" Time: %0.2d:%0.2d:%0.2d\r", THH, TMM, TSS);
}

/*
 * ��������Time_Show
 * ����  ���ڳ����ն�����ʾ��ǰʱ��ֵ
 * ����  ����
 * ���  ����
 * ����  �����������ã����ú���Time_Display(uint32_t TimeVar)
 */   
void Time_Show(void)
{
	printf("\n\r");
	
	/* Infinite loop */
	while (1)
	{
		/* If 1s has paased
		 * ֻ��TimeDisplay Ϊ1ʱ������λ������ʾʱ�䣬
		 * TimeDisplay���û��Լ�����ı�ʾ������RTC�ж�
		 * �������б���1����1�뱻��1һ�Σ�Ҳ����1����ô�������λ����һ��ʱ�� */
		if (TimeDisplay == 1)
		{
			/* Display current time
			 * ��RTC�������е�ֵ�������� Time_Display */
			Time_Display(RTC_GetCounter());
			TimeDisplay = 0;
		}
	}
}

