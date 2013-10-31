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

/********************************************************************************
 * ʵ��ƽ̨��stm32f103c8t6
 * ��汾  ��ST3.5.0
 * ʵ������LED1~LED4������˸�����1s
 * ����    ��zhangwj
**********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "led.h"
#include "SysTick.h"

int main(void)
{
	/*	LED_GPIO��ʼ��	*/
	LED_GPIO_config();

	/*	����SysTick��װ��STK_LOAD��10us�ж�һ��	*/
	SysTick_Init();
	
	while (1)
	{
		LED1( ON );			  // ��
	    Delay_us(100000);    	// 100000 * 10us = 1000ms = 1s
		LED1( OFF );		  // ��
		
		LED2( ON );
	    Delay_us(100000);
		LED2( OFF );
		
		LED3( ON );
	    Delay_us(100000);
		LED3( OFF );
		
		LED4( ON );			
	    Delay_us(100000);
		LED4( OFF );		       
	}
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
