/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 
#include "includes.h"
#include "GAME.h"
#include "rs485.h"

 
void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}
 
void PendSV_Handler(void)
{
}
 
//void SysTick_Handler(void)
//{
//	
//}


 
 /***************USART3(MCUBUS) 中断函数***********************
*	函数名称：USART3_IRQHandler();
*	函数功能：USART 的中断处理 默认ModBus
*   函数输入：无。														    
*	函数编写：user-F
*	编写日期：2015 年 3 月 25 日
*	函数版本：V1.0
**************************************************************/
 void UART3_IRQ(void);
 void USART3_IRQHandler(void)   //USART3中断
 {
   
	  UART3_IRQ();
 }
  /* 函数功能：Modbus专用定时器（TIM2）
 	函数输入：无。														    
	函数输出：无。													  
 */
 
extern void prvvTIMERExpiredISR( void );
void TIM2_IRQHandler(void)
{
    prvvTIMERExpiredISR();
}
 /***************UART4(485) 中断函数*********************************
*	函数名称：UART4_IRQHandler();
*	函数功能：USART 的中断处理
* 	函数输入：无。														    
*	函数编写：user-F
*	编写日期：2015 年 7 月 25 日
*	函数版本：V1.0
**************************************************************/
 void UART4_IRQHandler(void)   //UART4中断
 {
//     uint8_t Rcv_Data;
    if(USART_GetITStatus(UART4,USART_IT_RXNE)!=RESET)//接收中断
    {
        
        RS485Rcv();//用于RS485信息接收
//        Rcv_Data=USART_ReceiveData(USART3);
//        ModRcv(Rcv_Data); //用于modbus信息接收

    }

    if(USART_GetITStatus(UART4,USART_IT_TC)!=RESET)	//发送中断
    {
         RS485Send();//用于RS485信息发送

    }
	  
 }
