/*******************************************************************************
 * �ļ���  ��adc.c
 * ����    ��adcӦ�ú�����        
 * ʵ��ƽ̨��stm32f103c8t6
 * Ӳ�����ӣ�----------------------------------------------------
 *          |                                                    |
 *          | PA7 - ADC1 �����ⲿ��ѹ(ͨ��һ��������������ѹ����)|
 *          |                                                    |
 *           ----------------------------------------------------
 * ��汾  ��ST3.5.0 
 *
 * ����    ��zhangwj
**********************************************************************************/
#include "adc.h"

#define ADC1_DR_Address    ((u32)0x40012400+0x4c)	//�� 2.3�洢��ӳ�� + 11.12.14  ADC�������ݼĴ���(ADC_DR)
__IO uint16_t ADC_ConvertedValue;

/*
 * ��������ADC1_GPIO_Config
 * ����  ��ʹ��ADC1��DMA1��ʱ�ӣ���ʼ��PC.01
 * ����  : ��
 * ���  ����
 * ����  ���ڲ�����
 */
static void ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ʹ�� DMA ʱ�� */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	/* ʹ�� GPIO ʱ�Ӽ� ADC1 ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);
	
	/* �� ADC1 ��ͨ�� 7 ʹ�õ� GPIO ���� PA7 ���ó�ģ������ģʽ 
	 * ADC ��ͨ���� GPIO ���ŵĹ�ϵ��stm32f103c8t6��ԭ��ͼ
	 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);				// PA7,����ʱ������������
}

/* ��������ADC1_Mode_Config
 * ����  ������ADC1�Ĺ���ģʽΪMDAģʽ
 *
 *             ʹ�� DMA1 ��ͨ�� 1�����ݴ� ADC ��������ݼĴ���(ADC1_DR_Address)
 *         ת�Ƶ��ڴ�(ADC_ConvertedValue ����)���ڴ桢�����ַ���̶���ÿ�δ����
 *         ���ݴ�СΪ����(16 λ)��ʹ�� DMA ѭ������ģʽ��
 *
 * ����  : ��
 * ���  ����
 * ����  ���ڲ�����
 */
static void ADC1_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	/* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1);

	/* ADC_DR ���ݼĴ��������� ADC ת�������ֵ��������Ϊ DMA �Ĵ���Դ��ַ */
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 //ADC1_DR����Ĵ����ĵ�ַ����RM0008 STM32F10x�ο��ֲ�

	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;//ADC_ConvertedValue���ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;  //�ڴ��ַ�̶�
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//����
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//ѭ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	/* Enable DMA channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	/* ADC1 configuration */	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//����ADCģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 	 //��ֹɨ��ģʽ��ɨ��ģʽ���ڶ�ͨ���ɼ�
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//��������ת��ģʽ������ͣ�ؽ���ADCת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//��ʹ���ⲿ����ת��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//�ɼ������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	 	//Ҫת����ͨ����Ŀ1
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/*����ADCʱ�ӣ�ΪPCLK2��8��Ƶ����9Hz*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	/*����ADC1��ͨ��7Ϊ55.	5���������ڣ�����Ϊ1 */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 1, ADC_SampleTime_55Cycles5);
	
	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	
	/*��λУ׼�Ĵ��� */   
	ADC_ResetCalibration(ADC1);
	/*�ȴ�У׼�Ĵ�����λ��� */
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	/* ADCУ׼ */
	ADC_StartCalibration(ADC1);
	/* �ȴ�У׼���*/
	while(ADC_GetCalibrationStatus(ADC1));
	
	/* ����û�в����ⲿ����������ʹ���������ADCת�� */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/*
 * ��������ADC1_Init
 * ����  ����
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void ADC1_Init(void)
{
	//���ú� ADC1 ���õ� I/O �˿�
	ADC1_GPIO_Config();
	
	//���� ADC1 ��ʼ���� DMA ģʽ
	ADC1_Mode_Config();
}
