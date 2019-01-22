#ifndef __GLOBAL_H
#define __GLOBAL_H	
//#include "stm32f10x.h"
#include "stdint.h"

#define DelayTime_length   50       //总定时器个数
#define INPUT_IO_LENGTH    8		//输入IO个数
#define OUTPUT_IO_LENGTH   8		//输出IO个数

 //extern uint16_t 	HoldReg[100];								//modbus 缓存
 extern uint16_t     Delay_Time[DelayTime_length];  		    //定义使用到的延时个数 DelayTime_length在delay.h中
 extern uint16_t     Delay_TimeFlag[DelayTime_length];  	    //定义延时判定标志位 
 extern uint8_t      Input_Buffer[INPUT_IO_LENGTH]; 	        //输入缓存
 extern uint8_t      Output_Buffer[OUTPUT_IO_LENGTH];	        //输出IO缓存
 extern uint8_t      DI_UP[8];                                  //输入上升沿
 extern uint8_t      DI_DOWN[8];                                //输入下降沿

																  
#endif
