/********************************************************************************
 * �ļ���  ��led.c
 * ����    ��led Ӧ�ú�����         
 * ʵ��ƽ̨��stm32f103c8t6
 * Ӳ�����ӣ�-----------------
 *          |   PA4 - LED1     |
 *          |   PA5 - LED2     |
 *          |   PA6 - LED3     |
 *          |   PA7 - LED4     |
 *          |   LED������      |
 *           ----------------- 
 * ��汾  ��ST3.5.0
 * ����    ��zhangwj
**********************************************************************************/
#include "led.h"

void LED_GPIO_config(void)
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*����GPIOA������ʱ��*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
	 
	/*ѡ��Ҫ���Ƶ�GPIOC����*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;	

	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*���ÿ⺯������ʼ��GPIOA*/
  	GPIO_Init(GPIOA, &GPIO_InitStructure);		  

	/* �ر�����led��	*/
	GPIO_SetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);			
}
