#ifndef __GLOBAL_H
#define __GLOBAL_H	
//#include "stm32f10x.h"
#include "stdint.h"

#define DelayTime_length   50       //�ܶ�ʱ������
#define INPUT_IO_LENGTH    8		//����IO����
#define OUTPUT_IO_LENGTH   8		//���IO����

 //extern uint16_t 	HoldReg[100];								//modbus ����
 extern uint16_t     Delay_Time[DelayTime_length];  		    //����ʹ�õ�����ʱ���� DelayTime_length��delay.h��
 extern uint16_t     Delay_TimeFlag[DelayTime_length];  	    //������ʱ�ж���־λ 
 extern uint8_t      Input_Buffer[INPUT_IO_LENGTH]; 	        //���뻺��
 extern uint8_t      Output_Buffer[OUTPUT_IO_LENGTH];	        //���IO����
 extern uint8_t      DI_UP[8];                                  //����������
 extern uint8_t      DI_DOWN[8];                                //�����½���

																  
#endif
