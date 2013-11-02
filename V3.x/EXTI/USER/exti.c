/****************************************************************************
 * �ļ���  ��exti.c
 * ����    ��I/O���ж�Ӧ�ú�����         
 * ʵ��ƽ̨��stm32f103c8t6
 * Ӳ�����ӣ�-------------------------
 *          | PA0 - key1              |
 *          | PA1 - key2(��ʵ��û�õ�)|
 *          |                         |
 *           -------------------------
 * ��汾  ��ST3.0.0
 *
 * ����    ��zhangwj 
**********************************************************************************/
#include "exti.h"

/*
 * ��������NVIC_Configuration
 * ����  ������Ƕ�������жϿ�����NVIC
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	  
	/* �������ȼ���_��1�飺���1λ����������ռ���ȼ�����3λ����������Ӧ���ȼ��� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	  
	/* ����P[A|B|C|D|E]0Ϊ�ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*
 * ��������EXTI_PA0_Config
 * ����  ������ PA0 Ϊ���жϿڣ��������ж����ȼ�
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void EXTI_PA0_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/* ʹ��EXTIx��ʱ�Ӻ͵ڶ�����AFIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);

	/* ����EXTIx�ж����ȼ� */
	NVIC_Configuration();

	/* ����EXTI�ж���I/O */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ѡ��Ҫ����ΪEXTI��I/O�ߺ�I/O����ģʽ */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); 	//��GPIOA��Pin0����ΪEXTI������
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;	//ѡ��EXTI_Line0�߽������ã���Ϊ������PA0���ӵ���EXTI_Line0��
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	//��EXTI_Line0��ģʽ����ΪΪ�ж�ģʽEXTI_Mode_Interrupt��
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�жϴ�����ʽ_�½����ж�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;	//�ж���·ʹ��
	EXTI_Init(&EXTI_InitStructure); 

	/* EXTI����ģʽ���� */
}
