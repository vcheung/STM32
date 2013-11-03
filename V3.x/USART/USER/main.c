/**
  ******************************************************************************
  * ����    ����1����printf�����ض���USART1�������Ϳ�����printf��������Ƭ��������
  *           ��ӡ��PC�ϵĳ����ն˻򴮿ڵ������֡�
  *           ��2���Ա�д��ʽ�������USART1_printf��ʵ��ͬ����������ܡ�         
  * ʵ��ƽ̨��stm32f103c8t6
  * Ӳ�����ӣ�------------------------
  *          | PA9  - USART1(Tx)      |
  *          | PA10 - USART1(Rx)      |
  *           ------------------------
  * ��汾  ��ST3.5.0
  * ����    ��zhangwj   
  ******************************************************************************
  *  
  * �����ǵ� main.c �ļ���Ҫ�� stdio.h ���ͷ�ļ�������������Ҫ��
  * ������������һ��ѡ�� Use MicroLIB (ʹ��΢��)���� Options for target��
  * ���΢���� keil MDK ΪǶ��ʽӦ���������� C �⣬����Ҫ�Ⱦ��п⣬�����ض���
  * ��ѡʹ��֮�����ǾͿ���ʹ�� printf() ��������ˡ�
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
		printf("\r\n zhangwj @110ʵ���� :) \r\n");

		USART1_printf(USART1, "\r\n ��ӭ This is a USART1_printf demo \r\n");
		USART1_printf(USART1, "\r\n ("__DATE__ " - " __TIME__ ") \r\n");
	}

}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
