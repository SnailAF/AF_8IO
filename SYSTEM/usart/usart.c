#include "sys.h"
#include "usart.h"	  
#include "stm32f10x_usart.h"
//#include "includes.h"	
#include <stdarg.h>

/***********************************************************************
* 本文件初始化串口1-5，
* 串口4为与485通信用，
* 串口3与主机通信用，使用的是modbus协议。
**********************************************************************/	  


//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
uint8_t USART1_RX_BUF[USART1_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
uint16_t USART1_RX_STA=0;       //接收状态标记	  

 /***************USART3(PC) 初始化*********************************
*	函数名称：USART3_Init();
*	函数功能：与主机进行通信的端口，走的是ModBus协议。
*	被引函数：ModBus.c->ModInit()；
* 	函数输入：波特率														    
*	函数返回：无， 
*	函数编写：user-F
*	编写日期：2015 年 3 月 25 日
*	函数版本：V1.0
******************************************************/
//注意,读取USARTx->SR能避免莫名其妙的错误   	
uint8_t USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
uint16_t USART3_RX_STA=0;       //接收状态标记
void UART3_Init(u32 bound)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//使能USART3，GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
 	USART_DeInit(USART3);  //复位串口3
	 //USART3_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化PA2
   
    //USART3_RX	  PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PA3

   //Usart3 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART3, &USART_InitStructure); //初始化串口
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART3, ENABLE);                    //使能串口 

}

 /***************UART4(4851) 初始化*********************************
*	函数名称：UART4_Init();
*	函数功能：第一路485串口的初始化。
*	被引函数：
* 函数输入：波特率														    
*	函数返回：无， 
*	函数编写：user-F
*	编写日期：2015 年 7 月 24 日
*	函数版本：V1.0
********************************************************************/ 	
uint8_t UART4_RX_BUF[UART4_REC_LEN];     									          //接收缓冲,最大USART_REC_LEN个字节.
uint16_t UART4_RX_STA=0;       												              //接收状态标记
void UART4_Init(u32 bound)
{
    //GPIO端口设置
	GPIO_InitTypeDef 	GPIO_InitStructure;
	USART_InitTypeDef 	USART_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	        //使能UART4，GPIOC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
 	USART_DeInit(UART4);  												                //复位串口4
	//UART4_TX   PC.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 						        //PC.10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			        //50MHz频率
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				        //复用推挽输出
	GPIO_Init(GPIOC, &GPIO_InitStructure); 								        //初始化PC10
   
	//UART4_RX	  PC.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	         //浮空输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);  							         //初始化PC11

	//UART4 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;        //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		         //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				         //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);										             //根据指定的参数初始化VIC寄存器
  
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;						         //一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	   //字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			   //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;					   //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
                                                                 //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式

	USART_Init(UART4, &USART_InitStructure); 							         //初始化串口
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);						       //开启中断
	USART_Cmd(UART4, ENABLE);                    						       //使能串口 

}

/********************************************************************************************************************************************
*
*							Printf 定义 多串口自由使用。
*
*********************************************************************************************************************************************/



void U3Printf(char *fmt, ...)
{
    char buffer[USART3_TX_LEN  - 1];
    uint16_t i = 0;
    uint16_t len;

    __va_list arg_ptr; 											//Define convert parameters variable
    __va_start(arg_ptr, fmt); 									//Init variable
    len = _vsnprintf(buffer, USART3_TX_LEN+1, fmt, arg_ptr); 	//parameters list format to buffer
    __va_end(arg_ptr);
    while ((i < USART3_TX_LEN) && (i < len) && (len > 0))
    {
        USART_SendData(USART3, (uint8_t) buffer[i++]);
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET); 
    }
    va_end(arg_ptr);
}
/***************U4Printf 发送*********************************
*	函数名称：U4Printf(char *fmt, ...)
*	函数功能：串口3模拟使用Printf
* 	函数输入：要发送得数据，格式于Printf一样，
*	函数返回：无。
*	函数调用：外部调用
*	函数编写：user-F
*	编写日期：2015 年 8 月 13 日
*	函数版本：V1.0
***********************************************************/

void U4Printf(char *fmt, ...)
{
    char buffer[UART4_TX_LEN - 1];
    uint16_t i = 0;
    uint16_t len;    
    __va_list arg_ptr; 											//Define convert parameters variable
    __va_start(arg_ptr, fmt); 									//Init variable
    len = _vsnprintf(buffer, UART4_TX_LEN+1, fmt, arg_ptr); 	//parameters list format to buffer
    __va_end(arg_ptr);
    RW_485 = WRITE;
    while ((i < UART4_TX_LEN) && (i < len) && (len > 0))
    {
        USART_SendData(UART4, (uint8_t) buffer[i++]);
        while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET); 
    }
        RW_485 = READ;
    va_end(arg_ptr);

}
/***************Usart3（232） 发送*********************************
******************************************************************/

