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


 
 /***************USART3(MCUBUS) �жϺ���***********************
*	�������ƣ�USART3_IRQHandler();
*	�������ܣ�USART ���жϴ��� Ĭ��ModBus
*   �������룺�ޡ�														    
*	������д��user-F
*	��д���ڣ�2015 �� 3 �� 25 ��
*	�����汾��V1.0
**************************************************************/
 void UART3_IRQ(void);
 void USART3_IRQHandler(void)   //USART3�ж�
 {
   
	  UART3_IRQ();
 }
  /* �������ܣ�Modbusר�ö�ʱ����TIM2��
 	�������룺�ޡ�														    
	����������ޡ�													  
 */
 
extern void prvvTIMERExpiredISR( void );
void TIM2_IRQHandler(void)
{
    prvvTIMERExpiredISR();
}
 /***************UART4(485) �жϺ���*********************************
*	�������ƣ�UART4_IRQHandler();
*	�������ܣ�USART ���жϴ���
* 	�������룺�ޡ�														    
*	������д��user-F
*	��д���ڣ�2015 �� 7 �� 25 ��
*	�����汾��V1.0
**************************************************************/
 void UART4_IRQHandler(void)   //UART4�ж�
 {
//     uint8_t Rcv_Data;
    if(USART_GetITStatus(UART4,USART_IT_RXNE)!=RESET)//�����ж�
    {
        
        RS485Rcv();//����RS485��Ϣ����
//        Rcv_Data=USART_ReceiveData(USART3);
//        ModRcv(Rcv_Data); //����modbus��Ϣ����

    }

    if(USART_GetITStatus(UART4,USART_IT_TC)!=RESET)	//�����ж�
    {
         RS485Send();//����RS485��Ϣ����

    }
	  
 }
