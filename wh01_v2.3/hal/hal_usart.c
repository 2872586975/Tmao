#include "stm32f10x.h"
#include "OS_System.h"
#include "hal_usart.h"

unsigned char DebugTxDMAMapBuff[DEBUG_TXBUFF_SIZE_MAX];	
volatile unsigned char DebugIsBusy; 	//Debug���������ڷ��ͱ�־
volatile Queue256 DebugTxMsg;	
 
static void hal_usart_Config(void);
static void hal_DebugSendByte(unsigned char  Dat);
static void hal_DMA_Config(void);
static void hal_DMAC4_Enable(unsigned long size);
static void hal_debugProc(void);

void hal_UsartInit(void)
{
	hal_usart_Config();
	hal_DMA_Config();
	QueueEmpty(DebugTxMsg);
	DebugIsBusy = 0;
}

void hal_UsartProc(void)
{
	hal_debugProc();
}


static void hal_debugProc(void)
{
	unsigned char i,len;
	if(DebugIsBusy)return;
	len = QueueDataLen(DebugTxMsg);
	for(i=0; i<len; i++)
	{
		//if(!DebugIsBusy)
		//{
			QueueDataOut(DebugTxMsg,&DebugTxDMAMapBuff[i]);
		//	DebugIsBusy = 1;
		//	hal_DebugSendByte(temp);
		//}
	}
	if(len)
	{
		hal_DMAC4_Enable(len);
		DebugIsBusy = 1;
	}
}


static void hal_usart_Config(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure; 
	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1,ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2,ENABLE);
	 
	/*
	*  USART1_TX -> PA9 , USART1_RX ->	PA10
	*/				
	GPIO_InitStructure.GPIO_Pin = DEBUF_TX_PIN;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_TX_PORT, &GPIO_InitStructure);		   

	GPIO_InitStructure.GPIO_Pin = DEBUF_RX_PIN;	        
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(DEBUG_TX_PORT, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(DEBUG_USART_PORT, &USART_InitStructure); 
	USART_ITConfig(DEBUG_USART_PORT, USART_IT_RXNE, ENABLE);
	USART_ITConfig(DEBUG_USART_PORT, USART_IT_TXE, DISABLE);
	USART_DMACmd(DEBUG_USART_PORT,USART_DMAReq_Tx,ENABLE);
	USART_Cmd(DEBUG_USART_PORT, ENABLE);
	
	//USART1 INT Cofig
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//Usart2
	/*
	*  USART2_TX -> PA2 , USART2_RX ->	PA3
	*/				
	GPIO_InitStructure.GPIO_Pin = WIFI_TX_PIN;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(WIFI_TX_PORT, &GPIO_InitStructure);		   

	GPIO_InitStructure.GPIO_Pin = WIFI_RX_PIN;	        
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(WIFI_RX_PORT, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(WIFI_USART_PORT, &USART_InitStructure); 
	USART_ITConfig(WIFI_USART_PORT, USART_IT_RXNE, ENABLE);
	USART_ITConfig(WIFI_USART_PORT, USART_IT_TXE, DISABLE);
	//USART_DMACmd(WIFI_USART_PORT,USART_DMAReq_Tx,ENABLE);
	USART_Cmd(WIFI_USART_PORT, ENABLE);
	
	//USART2 INT Cofig
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

static void hal_DMA_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//����DMAʱ��
 
	
	DMA_DeInit(DMA1_Channel4);
	
	//USART1����DMA    
	DMA_InitStructure.DMA_PeripheralBaseAddr = (unsigned long)(&USART1->DR);			//���ݴ���Ŀ���ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (unsigned long)DebugTxDMAMapBuff;			//���ݻ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;									//������Ϊ���ݴ����Ŀ�ĵ�
	DMA_InitStructure.DMA_BufferSize = DEBUG_TXBUFF_SIZE_MAX;						//����Buff���ݴ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	 //���������ַ�Ƿ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;				 //�����ڴ��ַ�Ƿ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//�������ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;		 	//�ڴ����ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;		  	//��ͨ����ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;	   //�����ȼ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	  		//��ֹDMA2���ڴ��໥����
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);		  	//��ʼ��DMA, USART1��DMA1��ͨ��4
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);		//���USART_TX�ж�����
	
	//USART1����DMA�ж�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//����debug usart1��DMA���ͺ���,size-��Ҫ���͵����ݴ�С
static void hal_DMAC4_Enable(unsigned long size)
{
	 DMA1_Channel4->CCR &= ~(1<<0);
	 DMA1_Channel4->CNDTR = size;
	 DMA1_Channel4->CCR |= 1<<0;
}

//debug usart1ͨ��DMA��������жϻص�����
void DMA1_Channel4_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC4) != RESET)
	{
		DMA_ClearITPendingBit(DMA1_IT_TC4);	
		DMA_Cmd(DMA1_Channel4, DISABLE);				 
		DebugIsBusy = 0;
	}
}

static void hal_DebugSendByte(unsigned char  Dat)
{
	USART_SendData(USART1, Dat);
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE); 	
	
}

static void hal_SendByte(unsigned char data)
{
	//USART_SendData(WIFI_USART_PORT,data);
	 
	USART_SendData(USART1, (uint8_t) data);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

void USART1_IRQHandler(void)
{
	unsigned char dat;
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)
	{							
		dat = USART_ReceiveData(USART1);
	//	hal_DebugSendByte(dat);
		//hal_SendByte(dat);
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		
	}
	
	if(USART_GetITStatus(USART1,USART_IT_TXE) != RESET)
	{
		 USART_ClearITPendingBit(USART1, USART_IT_TXE);
		 USART_ITConfig(USART1, USART_IT_TXE, DISABLE); 	 
	}
}


void USART2_IRQHandler(void)
{
	unsigned char dat;
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
	{							
		dat = USART_ReceiveData(USART2);
		 
		QueueDataIn(DebugTxMsg,&dat,1);
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		
	}
	
	if(USART_GetITStatus(USART2,USART_IT_TXE) != RESET)
	{
		 USART_ClearITPendingBit(USART2, USART_IT_TXE);
		 USART_ITConfig(USART2, USART_IT_TXE, DISABLE); 	 
	}
}

