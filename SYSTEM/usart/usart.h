#ifndef __USART_H
#define __USART_H
#include <stdio.h>	
#include "stm32f10x.h"

/***********************************************************************
* ���ļ���ʼ������1-5��
* ����1Ϊ��4852ͨ���ã�
* ����2������ͨ���ã�ʹ�õ���modbusЭ�顣
* ����3Ϊ�豸��BUS����
* ����4Ϊ4851ͨ����
* ����5ΪWIFIģ���á�
**********************************************************************/	  


/**********************************************************************
								�����������ֽ�
**********************************************************************/
#define USART1_REC_LEN  		200 	 	//���崮��1�������ֽ��� 200
#define USART2_REC_LEN  		200  		//���崮��2�������ֽ��� 200
#define USART3_REC_LEN  		200  		//���崮��3�������ֽ��� 200
#define UART4_REC_LEN  			200  		//���崮��4�������ֽ��� 200
#define UART5_REC_LEN  			200  		//���崮��5�������ֽ��� 200

/**********************************************************************
								���ڽ���ʹ��
**********************************************************************/
#define EN_USART1_RX 			1			//ʹ�ܣ�1��/��ֹ��0������1����
#define EN_USART2_RX 			1			//ʹ�ܣ�1��/��ֹ��0������2����
#define EN_USART3_RX 			1			//ʹ�ܣ�1��/��ֹ��0������3����
#define EN_UART4_RX 			1			//ʹ�ܣ�1��/��ֹ��0������4����
#define EN_UART5_RX 			1			//ʹ�ܣ�1��/��ֹ��0������5����

/**********************************************************************
								Printf������ֽ���
**********************************************************************/
#define USART1_TX_LEN  		200 	 	//���崮��1�������ֽ��� 200
#define USART2_TX_LEN  		600  		//���崮��2�������ֽ��� 200
#define USART3_TX_LEN  		200  		//���崮��3�������ֽ��� 200
#define UART4_TX_LEN  		200  		//���崮��4�������ֽ��� 200
#define UART5_TX_LEN  		600  		//���崮��5�������ֽ��� 200

/**********************************************************************
								���ڽ��ջ�������
**********************************************************************/
extern u8 USART1_RX_BUF[USART1_REC_LEN]; 	//���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART1_RX_STA;         			//����״̬���
extern u8 USART2_RX_BUF[USART2_REC_LEN]; 	//���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART2_RX_STA;         			//����״̬���
extern u8 USART3_RX_BUF[USART3_REC_LEN]; 	//���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART3_RX_STA;         			//����״̬���
extern u8 UART4_RX_BUF[UART4_REC_LEN]; 	//���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 UART4_RX_STA;         			//����״̬���
extern u8 UART5_RX_BUF[UART5_REC_LEN]; 	//���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 UART5_RX_STA;         			//����״̬���

/**********************************************************************
								���ڷ��ͻ�������
**********************************************************************/
extern u8 USART1_TX_BUF[USART1_TX_LEN]; 	//���ջ���,���USART_TX_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART1_TX_STA;         			//����״̬���
extern u8 USART2_TX_BUF[USART2_TX_LEN]; 	//���ջ���,���USART_TX_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART2_TX_STA;         			//����״̬���
extern u16 USART2_TX_NUM;                   //��ǰ���͵�����λ
extern u8 USART3_TX_BUF[USART3_TX_LEN]; 	//���ջ���,���USART_TX_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART3_TX_STA;         			//����״̬���
extern u8 UART4_TX_BUF[UART4_TX_LEN]; 	//���ջ���,���USART_TX_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 UART4_TX_STA;         			//����״̬���
extern u8 UART5_TX_BUF[UART5_TX_LEN]; 	//���ջ���,���USART_TX_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 UART5_TX_STA;         			//����״̬���


/**********************************************************************
								���ڳ�ʼ��
**********************************************************************/
 void UART1_Init(u32 bound);				//4852
 void UART2_Init(u32 bound);				//PC	
 void UART3_Init(u32 bound);				//MCYBUS
 void UART4_Init(u32 bound);				//4851
 void UART5_Init(u32 bound);				//LAN/WIFI
 
 /**********************************************************************
								Printf ����
**********************************************************************/
 void U1Printf(char *fmt, ...);				//����1ģ��Printf
 void U2Printf(char *fmt, ...);				//����2ģ��Printf
 void U3Printf(char *fmt, ...);				//����3ģ��Printf
 void U4Printf(char *fmt, ...);				//����4ģ��Printf
 void U5Printf(char *fmt, ...);				//����5ģ��Printf
#endif


