/**
  ******************************************************************************
  * 描述    ：（1）将printf函数重定向到USART1。这样就可以用printf函数将单片机的数据
  *           打印到PC上的超级终端或串口调试助手。
  *           （2）自编写格式输出函数USART1_printf。实现同样的输出功能。         
  * 实验平台：stm32f103c8t6
  * 硬件连接：------------------------
  *          | PA9  - USART1(Tx)      |
  *          | PA10 - USART1(Rx)      |
  *           ------------------------
  * 库版本  ：ST3.5.0
  * 作者    ：zhangwj   
  ******************************************************************************
  *  
  * 在我们的 main.c 文件中要把 stdio.h 这个头文件包含进来，还要在
  * 编译器中设置一个选项 Use MicroLIB (使用微库)，在 Options for target。
  * 这个微库是 keil MDK 为嵌入式应用量身定做的 C 库，我们要先具有库，才能重定向。
  * 勾选使用之后，我们就可以使用 printf() 这个函数了。
  *
  **/	
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "usart1.h"

int main(void)
{
	USART1_Config();

	while (1)
	{
		printf("\r\n this is a printf demo \r\n");
		printf("\r\n zhangwj @110实验室 :) \r\n");

		USART1_printf(USART1, "\r\n 欢迎 This is a USART1_printf demo \r\n");
		USART1_printf(USART1, "\r\n ("__DATE__ " - " __TIME__ ") \r\n");
	}

}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
