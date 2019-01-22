#include "spi.h"
#include "stm32f10x_spi.h"
#include "stdio.h"
#include "stm32f10x.h"
#include "stm32f10x_dma.h"
//#inclde "includes.h"

/**************define*****************/
#define SPI1_ReciveBufferSize   100
#define SPI1_SendBufferSize     100

/************�ֲ�����****************/
uint8_t SPI1_RX_Buff[SPI1_ReciveBufferSize];
uint8_t SPI1_TX_Buff[SPI1_SendBufferSize];

/************�ⲿ����****************/
uint8_t SPI1_ReadWriteByte(uint8_t TxData);

/*******************************************************************************
* Function Name  : SPI1_DMA_Configuration
* Description    : ����SPI1_RX��DMAͨ��2��SPI1_TX��DMAͨ��3
* Input          : None
* Output         : None
* Return         : None
* Attention             : 
*******************************************************************************/
void SPI1_DMA_Configuration( void )
{
    DMA_InitTypeDef DMA_InitStructure;
    
    /* DMA1 Channel2 (triggered by SPI1 Rx event) Config */
  DMA_DeInit(DMA1_Channel2);  
  DMA_InitStructure.DMA_PeripheralBaseAddr  = (uint32_t)&SPI1->DR;              //���� SPI1 ��������(0x4001300C) ��ַ(Ŀ�ĵ�ַ)
  DMA_InitStructure.DMA_MemoryBaseAddr      = (uint32_t)SPI1_RX_Buff;           //���� SRAM �洢��ַ(Ŀ�ĵ�ַ)
  DMA_InitStructure.DMA_DIR                 = DMA_DIR_PeripheralSRC;            //���䷽�� ����-�ڴ�
  DMA_InitStructure.DMA_BufferSize          = SPI1_ReciveBufferSize;            //���� SPI1 ���ͳ���
  DMA_InitStructure.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;        //�����ַ����(����)
  DMA_InitStructure.DMA_MemoryInc           = DMA_MemoryInc_Enable;             //�ڴ��ַ����(�仯)
  DMA_InitStructure.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_Byte;      //���贫����(�ֽ�)
  DMA_InitStructure.DMA_MemoryDataSize      = DMA_MemoryDataSize_Byte;          //�ڴ洫����(�ֽ�)
  DMA_InitStructure.DMA_Mode                = DMA_Mode_Normal;                  //���䷽ʽ,һ�δ�����ֹͣ,�����¼���
  DMA_InitStructure.DMA_Priority            = DMA_Priority_VeryHigh;            //�жϷ�ʽ-��(����)
  DMA_InitStructure.DMA_M2M                 = DMA_M2M_Disable;                  //�ڴ浽�ڴ淽ʽ��ֹ
  DMA_Init(DMA1_Channel2, &DMA_InitStructure);
  
  DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
  /* Enable SPI1 DMA RX request */
  SPI1->CR2 |= 1<<0;                                                             //���ջ�����DMAʹ��
  DMA_Cmd(DMA1_Channel2, ENABLE);
    
    
    /* DMA1 Channel3 (triggered by SPI1 Tx event) Config */
  DMA_DeInit(DMA1_Channel3);  
  DMA_InitStructure.DMA_PeripheralBaseAddr  = (uint32_t)&SPI1->DR;          //����  ��������(0x4001300C) ��ַ(Դ��ַ)
  DMA_InitStructure.DMA_MemoryBaseAddr      = (uint32_t)SPI1_TX_Buff;       //���� SRAM �洢��ַ(Դ��ַ)
  DMA_InitStructure.DMA_DIR                 = DMA_DIR_PeripheralDST;        //���䷽�� �ڴ�-����
  DMA_InitStructure.DMA_BufferSize          = SPI1_SendBufferSize;          //���� SPI1 ���ճ���
  DMA_InitStructure.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;    //�����ַ����(����)
  DMA_InitStructure.DMA_MemoryInc           = DMA_MemoryInc_Enable;         //�ڴ��ַ����(�仯)
  DMA_InitStructure.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_Byte;  //���贫����(�ֽ�)
  DMA_InitStructure.DMA_MemoryDataSize      = DMA_MemoryDataSize_Byte;      //�ڴ洫����(�ֽ�)
  DMA_InitStructure.DMA_Mode                = DMA_Mode_Normal;              //���䷽ʽ,һ�δ�����ֹͣ,�����¼���
  DMA_InitStructure.DMA_Priority            = DMA_Priority_VeryHigh;        //�жϷ�ʽ-��(����)
  DMA_InitStructure.DMA_M2M                 = DMA_M2M_Disable;              //�ڴ浽�ڴ淽ʽ��ֹ
  DMA_Init(DMA1_Channel3, &DMA_InitStructure);
  
  DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);                           //���� DMA1_Channel3 ��������ж�
  DMA_ITConfig(DMA1_Channel3, DMA_IT_TE, ENABLE);                           //���� DMA1_Channel3 ��������ж�
  /* Enable SPI1 DMA TX request */
  SPI1->CR2 |= 1<<1;                                                        //���ͻ�����DMAʹ��
  DMA_Cmd(DMA1_Channel3, DISABLE);                                          //���� DMA ͨ�� DMA1_Channel3
}  

/***************485_SPI��ʼ��*********************************
*	�������ƣ�SPI485_Init(); SPI2
*	�������ܣ�SPI485 ��ʼ��
*   �������룺�ޣ�														    
*	�������أ��ޡ�
*	����˵��������Ϊֻ������ģʽ��8λ֡�ṹ��Ĭ�Ϸ�Ƶֵ256 ����ͨ��SPI485_SetSpeed�����������޸ġ�
*            72M����󲻳���32��Ƶ������64��Ƶ���ϡ�
*	������д��user-F
*	��д���ڣ�2015 �� 3 �� 19 ��
*	�����汾��V1.0
************************************************************/
SPI_InitTypeDef  SPI_InitStructure;

void SPI_Master_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_I2S_DeInit(SPI2);
	
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2, ENABLE );	          //SPI2ʹ��
    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	          //GPIOBʹ��
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  					        //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  					        //PB.12(SPI485_NSS)�������
    GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  
 	GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15);          //�����
    
  //SPI2 ����
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫����
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;							//ѡ���˴���ʱ�ӵ���̬:ʱ����ʱΪ�ͣ���������Ч��
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						//���ݲ����ڵ�һ��ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;							//CRCֵ����Ķ���ʽ
	SPI_Init(SPI2, &SPI_InitStructure);  								//����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	

 
	SPI_Cmd(SPI2, ENABLE);                                            //ʹ��SPI����
	SPI485_ReadWriteByte(0xff);                                       //��������	
}   
//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8��Ƶ   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16��Ƶ  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_256 256��Ƶ (SPI 281.25K@sys 72M)
//SPI �ٶ����ú���
//SpeedSet:0~7
//SPI�ٶ�=fAPB2/2^(SpeedSet+1)
//APB2ʱ��һ��Ϊ72Mhz
  
void SPI_Master_SetSpeed(uint16_t SpeedSet)
{
    SPI_InitStructure.SPI_BaudRatePrescaler = SpeedSet ;
  	SPI_Init(SPI2, &SPI_InitStructure);
    SPI_Cmd(SPI2,ENABLE);
} 

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
uint8_t SPI485_ReadWriteByte(uint8_t TxData)
{		
	uint8_t retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) 	//���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI_I2S_SendData(SPI2, TxData); 								//ͨ������SPIx����һ������
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)	//���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPI2); 								//����ͨ��SPIx������յ�����					    
}

uint8_t SPI485_WriteByte(uint16_t TxData)
{		
	uint8_t retry=0;		   
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) 	//���ָ����SPI��־λ�������:���ͻ���ձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}			  
		SPI_I2S_SendData(SPI2, TxData); 							//ͨ������SPIx����һ������	
		return 1;
}
 

/***************SPI Slave��ʼ��*********************************
*	�������ƣ�SPI_Slave(); SPI1
*	�������ܣ�SPISlave ��ʼ��
*   �������룺�ޣ�														    
*	�������أ��ޡ�
*	����˵��������ΪSlaveģʽ��8λ֡�ṹ��Ĭ�Ϸ�Ƶֵ256 ����ͨ��SPI485_SetSpeed�����������޸ġ�
*            72M����󲻳���32��Ƶ������64��Ƶ���ϡ�
*	������д��user-F
*	��д���ڣ�2015 �� 3 �� 19 ��
*	�����汾��V1.0
************************************************************/
void SPI_Slave_Init(void) 
{ 
    GPIO_InitTypeDef GPIO_InitStructure; 
    SPI_InitTypeDef SPI_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure; 
    
    //Enable SPI1 clock and GPIO clock for SPI1 and SPI 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO|RCC_APB2Periph_SPI1, ENABLE); 
    //IO��ʼ�� 
    
    //Configure SPI1 pins: SCK, MISO and MOSI 
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4|GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
 
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫����
	SPI_InitStructure.SPI_Mode = SPI_Mode_Slave ;						//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;							//ѡ���˴���ʱ�ӵ���̬:ʱ����ʱΪ�ͣ���������Ч��
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						//���ݲ����ڵ�һ��ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;							//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;  //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;							//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);  								//����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
            
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //ʹ���жϽ��н��գ��������NVIC�����Ȼ��飬1��ʾ1bit��ռ���ȼ�
    
    NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 
    
    /* Enable SPI2 RXNE interrupt */    
    SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_RXNE,ENABLE); 
    SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_TXE,ENABLE);
    //Enable SPI2 
    SPI_Cmd(SPI1, ENABLE); //�Ȳ�����SPI�����յ��ض������ʱ��������
} 

uint8_t SPI1_Buffer_Rx[200];
uint8_t SPI1_Buffer_Tx[200];
uint8_t RxIdx = 0;

//Master�жϷ������
void SPI2_IRQHandler(void) 
{ 
    //�������� 
    //printf("SPI_IRQ %d\n",RxIdx);
    //SPI1_Buffer_Rx[RxIdx++] = SPI_I2S_ReceiveData(SPI1);
} 

//Slave
void SPI1_IRQHandler(void) 
{ 
    //�������� 
    if(SPI_I2S_GetITStatus(SPI1,SPI_I2S_IT_TXE))
    {
        SPI_I2S_SendData(SPI1,SPI1_Buffer_Tx[0]);
    }
    if(SPI_I2S_GetITStatus(SPI1,SPI_I2S_IT_RXNE))
    {
        SPI1_Buffer_Rx[0] = SPI_I2S_ReceiveData(SPI1);
    }
}
 
