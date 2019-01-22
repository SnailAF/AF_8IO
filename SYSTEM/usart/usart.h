#ifndef __USART_H
#define __USART_H
#include <stdio.h>	
#include "stm32f10x.h"

/***********************************************************************
* 本文件初始化串口1-5，
* 串口1为与4852通信用，
* 串口2与主机通信用，使用的是modbus协议。
* 串口3为设备间BUS总线
* 串口4为4851通信用
* 串口5为WIFI模块用。
**********************************************************************/	  


/**********************************************************************
								串口最大接收字节
**********************************************************************/
#define USART1_REC_LEN  		200 	 	//定义串口1最大接收字节数 200
#define USART2_REC_LEN  		200  		//定义串口2最大接收字节数 200
#define USART3_REC_LEN  		200  		//定义串口3最大接收字节数 200
#define UART4_REC_LEN  			200  		//定义串口4最大接收字节数 200
#define UART5_REC_LEN  			200  		//定义串口5最大接收字节数 200

/**********************************************************************
								串口接收使能
**********************************************************************/
#define EN_USART1_RX 			1			//使能（1）/禁止（0）串口1接收
#define EN_USART2_RX 			1			//使能（1）/禁止（0）串口2接收
#define EN_USART3_RX 			1			//使能（1）/禁止（0）串口3接收
#define EN_UART4_RX 			1			//使能（1）/禁止（0）串口4接收
#define EN_UART5_RX 			1			//使能（1）/禁止（0）串口5接收

/**********************************************************************
								Printf最大发送字节数
**********************************************************************/
#define USART1_TX_LEN  		200 	 	//定义串口1最大接收字节数 200
#define USART2_TX_LEN  		600  		//定义串口2最大接收字节数 200
#define USART3_TX_LEN  		200  		//定义串口3最大接收字节数 200
#define UART4_TX_LEN  		200  		//定义串口4最大接收字节数 200
#define UART5_TX_LEN  		600  		//定义串口5最大接收字节数 200

/**********************************************************************
								串口接收缓存数组
**********************************************************************/
extern u8 USART1_RX_BUF[USART1_REC_LEN]; 	//接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART1_RX_STA;         			//接收状态标记
extern u8 USART2_RX_BUF[USART2_REC_LEN]; 	//接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART2_RX_STA;         			//接收状态标记
extern u8 USART3_RX_BUF[USART3_REC_LEN]; 	//接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART3_RX_STA;         			//接收状态标记
extern u8 UART4_RX_BUF[UART4_REC_LEN]; 	//接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 UART4_RX_STA;         			//接收状态标记
extern u8 UART5_RX_BUF[UART5_REC_LEN]; 	//接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 UART5_RX_STA;         			//接收状态标记

/**********************************************************************
								串口发送缓存数组
**********************************************************************/
extern u8 USART1_TX_BUF[USART1_TX_LEN]; 	//接收缓冲,最大USART_TX_LEN个字节.末字节为换行符 
extern u16 USART1_TX_STA;         			//接收状态标记
extern u8 USART2_TX_BUF[USART2_TX_LEN]; 	//接收缓冲,最大USART_TX_LEN个字节.末字节为换行符 
extern u16 USART2_TX_STA;         			//接收状态标记
extern u16 USART2_TX_NUM;                   //当前发送的数据位
extern u8 USART3_TX_BUF[USART3_TX_LEN]; 	//接收缓冲,最大USART_TX_LEN个字节.末字节为换行符 
extern u16 USART3_TX_STA;         			//接收状态标记
extern u8 UART4_TX_BUF[UART4_TX_LEN]; 	//接收缓冲,最大USART_TX_LEN个字节.末字节为换行符 
extern u16 UART4_TX_STA;         			//接收状态标记
extern u8 UART5_TX_BUF[UART5_TX_LEN]; 	//接收缓冲,最大USART_TX_LEN个字节.末字节为换行符 
extern u16 UART5_TX_STA;         			//接收状态标记


/**********************************************************************
								串口初始化
**********************************************************************/
 void UART1_Init(u32 bound);				//4852
 void UART2_Init(u32 bound);				//PC	
 void UART3_Init(u32 bound);				//MCYBUS
 void UART4_Init(u32 bound);				//4851
 void UART5_Init(u32 bound);				//LAN/WIFI
 
 /**********************************************************************
								Printf 函数
**********************************************************************/
 void U1Printf(char *fmt, ...);				//串口1模拟Printf
 void U2Printf(char *fmt, ...);				//串口2模拟Printf
 void U3Printf(char *fmt, ...);				//串口3模拟Printf
 void U4Printf(char *fmt, ...);				//串口4模拟Printf
 void U5Printf(char *fmt, ...);				//串口5模拟Printf
#endif


