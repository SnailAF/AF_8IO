#ifndef __TIMER_H
#define __TIMER_H
//#include "sys.h"
#include "stm32f10x.h"
 	  

#define EN_TIM3	0	//1Ϊ������ʱ��3�жϺ�����0Ϊ�������ж�
void TIM2_Int_Init(uint16_t arr,uint16_t psc); 
void TIM3_Int_Init(uint16_t arr,uint16_t psc);
void TIM4_Int_Init(uint16_t arr,uint16_t psc);
void TIM5_Int_Init(uint16_t arr,uint16_t psc);
#endif
