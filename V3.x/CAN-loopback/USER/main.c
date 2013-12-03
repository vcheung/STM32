/**
  ******************************************************************************
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "led.h"
#include "usart1.h"
#include "can.h"


int main(void)
{
	LED_GPIO_config();
	USART1_Config();
	
	printf("\r\n CAN回环测试 \r\n");
	
	USER_CAN_Init();
	printf("\r\n CAN回环测试初始化 \r\n");

	USER_CAN_Test();
	printf("\r\n CAN回环测试结束 \r\n");

	while (1);

}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
