/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "led.h"

void Delay(__IO u32 nCount);

int main(void)
{
	
	LED_GPIO_config();
	
	while (1)
	{
		LED1( ON );			  // 亮
		Delay(0x0FFFEF);
		LED1( OFF );		  // 灭
		
		LED2( ON );
		Delay(0x0FFFEF);
		LED2( OFF );
		
		LED3( ON );
		Delay(0x0FFFEF);
		LED3( OFF );
		
		LED4( ON );			
		Delay(0x0FFFEF);
		LED4( OFF );		       
	}
}

void Delay(__IO u32 nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
