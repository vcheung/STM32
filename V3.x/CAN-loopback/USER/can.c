#include "can.h"
#include "led.h"

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
volatile TestStatus TestRx;

/* ���жϴ������з��� */
__IO uint32_t ret = 0;	//__IO��volatile
    
/*
 * ��������CAN_NVIC_Configuration
 * ����  ��CAN RX0 �ж����ȼ�����
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */
static void CAN_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Enable CAN1 RX0 interrupt IRQ channel */
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;     // �����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // �����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*
 * ��������CAN_GPIO_Config
 * ����  ��CAN GPIO ��ʱ������
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */
static void CAN_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);	                        											 
	
	/* CAN1 Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	
	/* Configure CAN pin: RX */									 // PB8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	             // ��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Configure CAN pin: TX */									 // PB9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		         // �����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//#define GPIO_Remap_CAN    GPIO_Remap1_CAN1 ��ʵ��û���õ���ӳ��I/O
	GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
}

/*
 * ��������CAN_Init
 * ����  ��CAN��ʼ���������˿ڳ�ʼ�����ж����ȼ���ʼ��
 * ����  ����
 * ���  : ��	 
 * ����  ���ⲿ����
 */
void USER_CAN_Init(void)
{
	CAN_NVIC_Configuration();
	CAN_GPIO_Config();
}

/*
 * ��������CAN_Polling
 * ����  ������ CAN �Ĺ���ģʽΪ �ػ�ģʽ
 * ����  ����
 * ���  : -PASSED   �ɹ�
 *         -FAILED   ʧ��
 * ����  ���ڲ�����
 */
TestStatus CAN_Polling(void)
{
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	CanTxMsg TxMessage;
	CanRxMsg RxMessage;
	uint32_t i = 0;
	uint8_t TransmitMailbox = 0;
	
	/* CAN register init */
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	
	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM=DISABLE;						 // ʱ�䴥��ͨ�Ž�ֹ
	CAN_InitStructure.CAN_ABOM=DISABLE;						 // �����˳������ж���λ��0���˳�
	CAN_InitStructure.CAN_AWUM=DISABLE;						 // �Զ�����ģʽ������sleep
	CAN_InitStructure.CAN_NART=DISABLE;						 // �Զ����´��ͱ��ƣ�֪�����ͳɹ�
	CAN_InitStructure.CAN_RFLM=DISABLE;						 // FIFOû���������±��ĸ��Ǿɱ���  
	CAN_InitStructure.CAN_TXFP=DISABLE;						 // ���ͱ������ȼ�ȷ������־��
	CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;	 // �ػ�ģʽ
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;				 // 1tq��BS1��BS2��ֵ���������й�
	CAN_InitStructure.CAN_BS1=CAN_BS1_8tq;
	CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;
	CAN_InitStructure.CAN_Prescaler=5;             // ��Ƶϵ��Ϊ5
	CAN_Init(CAN1, &CAN_InitStructure);            // ��ʼ��CAN1 
	
	/* CAN filter init */
	CAN_FilterInitStructure.CAN_FilterNumber=0;
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=0;
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	/* transmit */
	TxMessage.StdId=0x11;				// �趨��׼��ʶ����11λ����չ��Ϊ29λ��
	TxMessage.RTR=CAN_RTR_DATA;	// ������Ϣ��֡����Ϊ����֡������Զ��֡��
	TxMessage.IDE=CAN_ID_STD;		// ��Ϣ��־��ʵ���׼��ʶ��
	TxMessage.DLC=2;					  // ������֡��һ֡8λ
	TxMessage.Data[0]=0xCA;			// ��һ֡����
	TxMessage.Data[1]=0xFE;			// �ڶ�֡����
	
	TransmitMailbox=CAN_Transmit(CAN1, &TxMessage);
	i = 0;
	// ���ڼ����Ϣ�����Ƿ�����
	while((CAN_TransmitStatus(CAN1, TransmitMailbox) != CANTXOK) && (i != 0xFF))
	{
		i++;
	}
	
	i = 0;
	// ��鷵�صĹҺŵ���Ϣ��Ŀ
	while((CAN_MessagePending(CAN1, CAN_FIFO0) < 1) && (i != 0xFF))
	{
		i++;
	}
	
	/* receive */
	RxMessage.StdId=0x00;
	RxMessage.IDE=CAN_ID_STD;
	RxMessage.DLC=0;
	RxMessage.Data[0]=0x00;
	RxMessage.Data[1]=0x00;
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	
	
	if (RxMessage.StdId!=0x11)
	{
		return FAILED;  
	}
	
	if (RxMessage.IDE!=CAN_ID_STD)
	{
		return FAILED;
	}
	
	if (RxMessage.DLC!=2)
	{
		return FAILED;  
	}
	
	/* �жϷ��͵���Ϣ�ͽ��յ���Ϣ�Ƿ���� */
	if ((RxMessage.Data[0]<<8|RxMessage.Data[1])!=0xCAFE)
	{
		return FAILED;
	}
	
	//printf("receive data:0X%X,0X%X",RxMessage.Data[0], RxMessage.Data[1]);  
	
	return PASSED; /* Test Passed */
}

/*
 * ��������CAN_Interrupt
 * ����  ������ CAN �Ĺ���ģʽΪ �ж�ģʽ
 * ����  ����
 * ���  : -PASSED   �ɹ�
 *         -FAILED   ʧ��
 * ����  ���ڲ�����
 */
TestStatus CAN_Interrupt(void)
{
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	CanTxMsg TxMessage;
	uint32_t i = 0;
	
	/* CAN register init */
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	
	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM=DISABLE;           // ʱ�䴥��ͨ�Ž�ֹ
	CAN_InitStructure.CAN_ABOM=DISABLE;	          // �����˳������ж���λ��0���˳�
	CAN_InitStructure.CAN_AWUM=DISABLE;	          // �Զ�����ģʽ������sleep
	CAN_InitStructure.CAN_NART=DISABLE;	          // �Զ����´��ͱ��ƣ�֪�����ͳɹ�
	CAN_InitStructure.CAN_RFLM=DISABLE;	          // FIFOû���������±��ĸ��Ǿɱ���
	CAN_InitStructure.CAN_TXFP=DISABLE;           // ���ͱ������ȼ�ȷ������־��
	CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack; // �ػ�ģʽ
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;        // 1tq��BS1��BS2��ֵ���������й�
	CAN_InitStructure.CAN_BS1=CAN_BS1_8tq;
	CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;
	CAN_InitStructure.CAN_Prescaler=1;					  // ��Ƶϵ��Ϊ1
	CAN_Init(CAN1, &CAN_InitStructure);						// ��ʼ��CAN1
	
	/* CAN filter init */
	CAN_FilterInitStructure.CAN_FilterNumber=1;
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	/* CAN FIFO0 message pending interrupt enable */ 
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE); //�ж�ʹ��
	
	/* transmit 1 message */
	TxMessage.StdId=0x00;					 // ��׼��ʶ��Ϊ0
	TxMessage.ExtId=0x1234;				 // ������չ��ʾ����29λ��
	TxMessage.IDE=CAN_ID_EXT;			 // ʹ����չ��ʶ��
	TxMessage.RTR=CAN_RTR_DATA;		 // ��Ϣ����Ϊ����֡��һ֡8λ
	TxMessage.DLC=2;							 // ������֡��Ϣ
	TxMessage.Data[0]=0xDE;				 // ��һ֡��Ϣ
	TxMessage.Data[1]=0xCA;				 // �ڶ�֡��Ϣ
	CAN_Transmit(CAN1, &TxMessage);
	
	/* initialize the value that will be returned */
	ret = 0xFF;
	
	/* receive message with interrupt handling */
	i=0;
	while((ret == 0xFF) && (i < 0xFFF))
	{
	i++;
	}
	
	if (i == 0xFFF)
	{
	ret=0;  
	}
	
	/* disable interrupt handling */
	CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);
	
	return (TestStatus)ret;
}

/*
 * ��������CAN_Test
 * ����  ��CAN �ػ�ģʽ���ж�ģʽ����
 * ����  ����
 * ���  : ��	 
 * ����  ���ⲿ����
 */
void USER_CAN_Test(void)
{
	/* CAN transmit at 100Kb/s and receive by polling in loopback mode */
	TestRx = CAN_Polling();
	
	if (TestRx == FAILED)
	{    
		LED1( OFF );	// LED1 OFF 		
	}
	else
	{    
		LED1( ON );	  // LED1 ON;		
	}
	
	/* CAN transmit at 500Kb/s and receive by interrupt in loopback mode */
	TestRx = CAN_Interrupt();
	
	if (TestRx == FAILED)
	{    
		LED2( OFF );	// LED2 OFF;		 
	}
	else
	{   
		LED2( ON );	  // LED2 ON;		
	}
}
