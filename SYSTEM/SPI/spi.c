#include "spi.h"
#include "stm32f10x_spi.h"
#include "stdio.h"
#include "stm32f10x.h"
#include "stm32f10x_dma.h"
//#inclde "includes.h"

/**************define*****************/
#define SPI1_ReciveBufferSize   100
#define SPI1_SendBufferSize     100

/************局部变量****************/
uint8_t SPI1_RX_Buff[SPI1_ReciveBufferSize];
uint8_t SPI1_TX_Buff[SPI1_SendBufferSize];

/************外部函数****************/
uint8_t SPI1_ReadWriteByte(uint8_t TxData);

/*******************************************************************************
* Function Name  : SPI1_DMA_Configuration
* Description    : 配置SPI1_RX的DMA通道2，SPI1_TX的DMA通道3
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
  DMA_InitStructure.DMA_PeripheralBaseAddr  = (uint32_t)&SPI1->DR;              //设置 SPI1 发送外设(0x4001300C) 地址(目的地址)
  DMA_InitStructure.DMA_MemoryBaseAddr      = (uint32_t)SPI1_RX_Buff;           //设置 SRAM 存储地址(目的地址)
  DMA_InitStructure.DMA_DIR                 = DMA_DIR_PeripheralSRC;            //传输方向 外设-内存
  DMA_InitStructure.DMA_BufferSize          = SPI1_ReciveBufferSize;            //设置 SPI1 发送长度
  DMA_InitStructure.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;        //外设地址增量(不变)
  DMA_InitStructure.DMA_MemoryInc           = DMA_MemoryInc_Enable;             //内存地址增量(变化)
  DMA_InitStructure.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_Byte;      //外设传输宽度(字节)
  DMA_InitStructure.DMA_MemoryDataSize      = DMA_MemoryDataSize_Byte;          //内存传输宽度(字节)
  DMA_InitStructure.DMA_Mode                = DMA_Mode_Normal;                  //传输方式,一次传输完停止,不重新加载
  DMA_InitStructure.DMA_Priority            = DMA_Priority_VeryHigh;            //中断方式-高(三级)
  DMA_InitStructure.DMA_M2M                 = DMA_M2M_Disable;                  //内存到内存方式禁止
  DMA_Init(DMA1_Channel2, &DMA_InitStructure);
  
  DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
  /* Enable SPI1 DMA RX request */
  SPI1->CR2 |= 1<<0;                                                             //接收缓冲区DMA使能
  DMA_Cmd(DMA1_Channel2, ENABLE);
    
    
    /* DMA1 Channel3 (triggered by SPI1 Tx event) Config */
  DMA_DeInit(DMA1_Channel3);  
  DMA_InitStructure.DMA_PeripheralBaseAddr  = (uint32_t)&SPI1->DR;          //设置  接收外设(0x4001300C) 地址(源地址)
  DMA_InitStructure.DMA_MemoryBaseAddr      = (uint32_t)SPI1_TX_Buff;       //设置 SRAM 存储地址(源地址)
  DMA_InitStructure.DMA_DIR                 = DMA_DIR_PeripheralDST;        //传输方向 内存-外设
  DMA_InitStructure.DMA_BufferSize          = SPI1_SendBufferSize;          //设置 SPI1 接收长度
  DMA_InitStructure.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;    //外设地址增量(不变)
  DMA_InitStructure.DMA_MemoryInc           = DMA_MemoryInc_Enable;         //内存地址增量(变化)
  DMA_InitStructure.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_Byte;  //外设传输宽度(字节)
  DMA_InitStructure.DMA_MemoryDataSize      = DMA_MemoryDataSize_Byte;      //内存传输宽度(字节)
  DMA_InitStructure.DMA_Mode                = DMA_Mode_Normal;              //传输方式,一次传输完停止,不重新加载
  DMA_InitStructure.DMA_Priority            = DMA_Priority_VeryHigh;        //中断方式-高(三级)
  DMA_InitStructure.DMA_M2M                 = DMA_M2M_Disable;              //内存到内存方式禁止
  DMA_Init(DMA1_Channel3, &DMA_InitStructure);
  
  DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);                           //开启 DMA1_Channel3 传输完成中断
  DMA_ITConfig(DMA1_Channel3, DMA_IT_TE, ENABLE);                           //开启 DMA1_Channel3 传输错误中断
  /* Enable SPI1 DMA TX request */
  SPI1->CR2 |= 1<<1;                                                        //发送缓冲区DMA使能
  DMA_Cmd(DMA1_Channel3, DISABLE);                                          //开启 DMA 通道 DMA1_Channel3
}  

/***************485_SPI初始化*********************************
*	函数名称：SPI485_Init(); SPI2
*	函数功能：SPI485 初始化
*   函数输入：无；														    
*	函数返回：无。
*	函数说明：配置为只发送主模式，8位帧结构，默认分频值256 可以通过SPI485_SetSpeed（）；函数修改。
*            72M下最大不超过32分频，建议64分频以上。
*	函数编写：user-F
*	编写日期：2015 年 3 月 19 日
*	函数版本：V1.0
************************************************************/
SPI_InitTypeDef  SPI_InitStructure;

void SPI_Master_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_I2S_DeInit(SPI2);
	
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2, ENABLE );	          //SPI2使能
    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	          //GPIOB使能
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  					        //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  					        //PB.12(SPI485_NSS)推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  
 	GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15);          //输出高
    
  //SPI2 配置
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工。
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;							//选择了串行时钟的稳态:时钟闲时为低（上升沿有效）
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						//数据捕获于第一个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;							//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  								//根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	

 
	SPI_Cmd(SPI2, ENABLE);                                            //使能SPI外设
	SPI485_ReadWriteByte(0xff);                                       //启动传输	
}   
//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8分频   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16分频  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_256 256分频 (SPI 281.25K@sys 72M)
//SPI 速度设置函数
//SpeedSet:0~7
//SPI速度=fAPB2/2^(SpeedSet+1)
//APB2时钟一般为72Mhz
  
void SPI_Master_SetSpeed(uint16_t SpeedSet)
{
    SPI_InitStructure.SPI_BaudRatePrescaler = SpeedSet ;
  	SPI_Init(SPI2, &SPI_InitStructure);
    SPI_Cmd(SPI2,ENABLE);
} 

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
uint8_t SPI485_ReadWriteByte(uint8_t TxData)
{		
	uint8_t retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) 	//检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI_I2S_SendData(SPI2, TxData); 								//通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)	//检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPI2); 								//返回通过SPIx最近接收的数据					    
}

uint8_t SPI485_WriteByte(uint16_t TxData)
{		
	uint8_t retry=0;		   
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) 	//检查指定的SPI标志位设置与否:发送缓存空标志位
		{
		retry++;
		if(retry>200)return 0;
		}			  
		SPI_I2S_SendData(SPI2, TxData); 							//通过外设SPIx发送一个数据	
		return 1;
}
 

/***************SPI Slave初始化*********************************
*	函数名称：SPI_Slave(); SPI1
*	函数功能：SPISlave 初始化
*   函数输入：无；														    
*	函数返回：无。
*	函数说明：配置为Slave模式，8位帧结构，默认分频值256 可以通过SPI485_SetSpeed（）；函数修改。
*            72M下最大不超过32分频，建议64分频以上。
*	函数编写：user-F
*	编写日期：2015 年 3 月 19 日
*	函数版本：V1.0
************************************************************/
void SPI_Slave_Init(void) 
{ 
    GPIO_InitTypeDef GPIO_InitStructure; 
    SPI_InitTypeDef SPI_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure; 
    
    //Enable SPI1 clock and GPIO clock for SPI1 and SPI 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO|RCC_APB2Periph_SPI1, ENABLE); 
    //IO初始化 
    
    //Configure SPI1 pins: SCK, MISO and MOSI 
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4|GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
 
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工。
	SPI_InitStructure.SPI_Mode = SPI_Mode_Slave ;						//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;							//选择了串行时钟的稳态:时钟闲时为低（上升沿有效）
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						//数据捕获于第一个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;							//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;  //定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;							//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  								//根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
            
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //使用中断进行接收，因此设置NVIC的优先机组，1表示1bit抢占优先级
    
    NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 
    
    /* Enable SPI2 RXNE interrupt */    
    SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_RXNE,ENABLE); 
    SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_TXE,ENABLE);
    //Enable SPI2 
    SPI_Cmd(SPI1, ENABLE); //先不启动SPI，在收到特定命令的时候再启动
} 

uint8_t SPI1_Buffer_Rx[200];
uint8_t SPI1_Buffer_Tx[200];
uint8_t RxIdx = 0;

//Master中断服务程序
void SPI2_IRQHandler(void) 
{ 
    //接收数据 
    //printf("SPI_IRQ %d\n",RxIdx);
    //SPI1_Buffer_Rx[RxIdx++] = SPI_I2S_ReceiveData(SPI1);
} 

//Slave
void SPI1_IRQHandler(void) 
{ 
    //接收数据 
    if(SPI_I2S_GetITStatus(SPI1,SPI_I2S_IT_TXE))
    {
        SPI_I2S_SendData(SPI1,SPI1_Buffer_Tx[0]);
    }
    if(SPI_I2S_GetITStatus(SPI1,SPI_I2S_IT_RXNE))
    {
        SPI1_Buffer_Rx[0] = SPI_I2S_ReceiveData(SPI1);
    }
}
 
