/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * 文件名  ：main.c
 * 描述    ：串口1(USART1)向电脑的超级终端以1s为间隔（软件延时）打印当前ADC1的转换电压值         
 * 实验平台：stm32f103c8t6
 * 库版本  ：ST3.5.0
 *
 * 作者    ：zhangwj 
**********************************************************************************/
#include "stm32f10x.h"
#include "usart1.h"
#include "adc.h"

// ADC1转换的电压值通过MDA方式传到SRAM
extern __IO uint16_t ADC_ConvertedValue;

// 局部变量，用于保存转换计算后的电压值			 
float ADC_ConvertedValueLocal;        

// 软件延时
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
} 


int main(void)
{
	USART1_Config();

	ADC1_Init();

	printf("\r\n -------这是一个ADC实验------\r\n");
	
	while (1)
	{
		ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*3.3; // 读取转换的AD值
	
		printf("\r\n The current AD value = 0x%04X \r\n", ADC_ConvertedValue); 
		printf("\r\n The current AD value = %f V \r\n",ADC_ConvertedValueLocal); 

		Delay(0xffffee);	// 延时 														  
	}
}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
