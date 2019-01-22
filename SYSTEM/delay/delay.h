#ifndef __DELAY_H
#define __DELAY_H
//#include "sys.h"
//#include "stm32f10x.h"	
#include "stdint.h"
#include "global.h" 

//0-19定时器系统用定时器
#define Run_Led_Delaytime       Delay_Time[0]
#define Run_Led_DelaytimeFlag	Delay_TimeFlag[0]
#define IOUT_Timeout  			Delay_Time[1]
#define IOUT_TimeoutFlag 		Delay_TimeFlag[1]
#define ADC_DelayTime  			Delay_Time[2]
#define ADC_DelayTimeoutFlag 	Delay_TimeFlag[2] 
#define SD_DelayTime  			Delay_Time[3]
#define SD_DelayimeoutFlag 		Delay_TimeFlag[3] 
#define SPI485_DelayTime        Delay_Time[4]
#define SPI485_DelayTimeFlag    Delay_TimeFlag[4]
#define Buzzer_Delaytime		Delay_Time[5]
#define Buzzer_DelaytimeFlag	Delay_TimeFlag[5]
#define DAC_Delaytime		    Delay_Time[6]
#define DAC_DelaytimeFlag   	Delay_TimeFlag[6]
#define Input_Delaytime         Delay_Time[7] 
#define Input_DelaytimeFlag   	Delay_TimeFlag[7]

//用户程序定时器从20开始
#define Game_Delaytime0         Delay_Time[21]
#define Game_DelaytimeFlag0     Delay_TimeFlag[21]
#define Game_Delaytime1         Delay_Time[22]
#define Game_DelaytimeFlag1     Delay_TimeFlag[22]
#define Game_Delaytime2         Delay_Time[23]
#define Game_DelaytimeFlag2     Delay_TimeFlag[23]
#define Game_Delaytime3         Delay_Time[24]
#define Game_DelaytimeFlag3     Delay_TimeFlag[24]
#define Game_Delaytime4         Delay_Time[25]
#define Game_DelaytimeFlag4     Delay_TimeFlag[25]
#define Game_Delaytime5         Delay_Time[26]
#define Game_DelaytimeFlag5     Delay_TimeFlag[26]
#define Game_Delaytime6         Delay_Time[27]
#define Game_DelaytimeFlag6     Delay_TimeFlag[27]
#define Game_Delaytime7         Delay_Time[28]
#define Game_DelaytimeFlag7     Delay_TimeFlag[28]
#define Game_Delaytime8         Delay_Time[29]
#define Game_DelaytimeFlag8     Delay_TimeFlag[29]
#define Game_Delaytime9         Delay_Time[30]
#define Game_DelaytimeFlag9     Delay_TimeFlag[30]
#define Game_Delaytime10        Delay_Time[31]
#define Game_DelaytimeFlag10    Delay_TimeFlag[31]
#define Game_Delaytime11        Delay_Time[32]
#define Game_DelaytimeFlag11    Delay_TimeFlag[32]
#define Game_Delaytime12        Delay_Time[33]
#define Game_DelaytimeFlag12    Delay_TimeFlag[33]
#define Game_Delaytime13        Delay_Time[34]
#define Game_DelaytimeFlag13    Delay_TimeFlag[34]
#define Game_Delaytime14        Delay_Time[35]
#define Game_DelaytimeFlag14    Delay_TimeFlag[35]


void SCT_Delay_Init(void);          //初始化
void SCT_Delay_ADD(void);           //定时器自加
void SysTick_Handler(void);         //中断处理
void delay_us(uint16_t time);
void delay_ms(uint16_t time);

#endif
