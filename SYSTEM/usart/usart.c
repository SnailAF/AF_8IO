#include "sys.h"
#include "usart.h"	  
#include "stm32f10x_usart.h"
//#include "includes.h"	
#include <stdarg.h>

/***********************************************************************
* ���ļ���ʼ������1-5��
* ����4Ϊ��485ͨ���ã�
* ����3������ͨ���ã�ʹ�õ���modbusЭ�顣
**********************************************************************/	  


//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
uint8_t USART1_RX_BUF[USART1_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
uint16_t USART1_RX_STA=0;       //����״̬���	  

 /***************USART3(PC) ��ʼ��*********************************
*	�������ƣ�USART3_Init();
*	�������ܣ�����������ͨ�ŵĶ˿ڣ��ߵ���ModBusЭ�顣
*	����������ModBus.c->ModInit()��
* 	�������룺������														    
*	�������أ��ޣ� 
*	������д��user-F
*	��д���ڣ�2015 �� 3 �� 25 ��
*	�����汾��V1.0
******************************************************/
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
uint8_t USART3_RX_BUF[USART3_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
uint16_t USART3_RX_STA=0;       //����״̬���
void UART3_Init(u32 bound)
{
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//ʹ��USART3��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
 	USART_DeInit(USART3);  //��λ����3
	 //USART3_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��PA2
   
    //USART3_RX	  PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ��PA3

   //Usart3 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART3, &USART_InitStructure); //��ʼ������
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 

}

 /***************UART4(4851) ��ʼ��*********************************
*	�������ƣ�UART4_Init();
*	�������ܣ���һ·485���ڵĳ�ʼ����
*	����������
* �������룺������														    
*	�������أ��ޣ� 
*	������д��user-F
*	��д���ڣ�2015 �� 7 �� 24 ��
*	�����汾��V1.0
********************************************************************/ 	
uint8_t UART4_RX_BUF[UART4_REC_LEN];     									          //���ջ���,���USART_REC_LEN���ֽ�.
uint16_t UART4_RX_STA=0;       												              //����״̬���
void UART4_Init(u32 bound)
{
    //GPIO�˿�����
	GPIO_InitTypeDef 	GPIO_InitStructure;
	USART_InitTypeDef 	USART_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	        //ʹ��UART4��GPIOCʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
 	USART_DeInit(UART4);  												                //��λ����4
	//UART4_TX   PC.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 						        //PC.10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			        //50MHzƵ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				        //�����������
	GPIO_Init(GPIOC, &GPIO_InitStructure); 								        //��ʼ��PC10
   
	//UART4_RX	  PC.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	         //��������
	GPIO_Init(GPIOC, &GPIO_InitStructure);  							         //��ʼ��PC11

	//UART4 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;        //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		         //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				         //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);										             //����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;						         //һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	   //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			   //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;					   //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
                                                                 //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ

	USART_Init(UART4, &USART_InitStructure); 							         //��ʼ������
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);						       //�����ж�
	USART_Cmd(UART4, ENABLE);                    						       //ʹ�ܴ��� 

}

/********************************************************************************************************************************************
*
*							Printf ���� �മ������ʹ�á�
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
/***************U4Printf ����*********************************
*	�������ƣ�U4Printf(char *fmt, ...)
*	�������ܣ�����3ģ��ʹ��Printf
* 	�������룺Ҫ���͵����ݣ���ʽ��Printfһ����
*	�������أ��ޡ�
*	�������ã��ⲿ����
*	������д��user-F
*	��д���ڣ�2015 �� 8 �� 13 ��
*	�����汾��V1.0
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
/***************Usart3��232�� ����*********************************
******************************************************************/

