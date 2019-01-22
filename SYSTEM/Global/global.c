#include "global.h"


/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
 //uint16_t 	HoldReg[100] = {0x0001,0x0203,0x0304,0x0506,0x0708,0x090a,0x0b0c,0x0d0e};								//modbus 缓存
 uint16_t    Delay_Time[DelayTime_length];  				//定义使用到的延时个数 DelayTime_length在delay.h中
 uint16_t  	Delay_TimeFlag[DelayTime_length];  			//定义延时判定标志位 
 uint8_t     Input_Buffer[INPUT_IO_LENGTH]; 	            //输入缓存
 uint8_t     Output_Buffer[OUTPUT_IO_LENGTH];	        //输出IO缓存
 uint8_t     DI_UP[8] = {0};                             //输入上升沿
 uint8_t     DI_DOWN[8] = {0};                           //输入下降沿
// uint8_t     G_LANstate;                                 //网络接受状态，0待接受，1接收到"{",2接收到"}".
// char   JSONRcvbuf[200] = {0};						//网络接受JSON缓存数组，第一位为有效数字；
// char   *PointJSONRcvbuf = JSONRcvbuf;				//JSONbuf的指针；
