#include "sys.h"
#include "includes.h"
#include "delay.h"

void NVIC_Configuration(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级

}
/***************系统初始化*********************************
*	函数名称：Sys_Init()
*	函数功能: 初始化基本函数
*   函数输入：无；														    
*	函数返回：无。
*	函数编写：user-F
*	编写日期：2015 年 3 月 19 日
*	函数版本：V1.0
******************************************************/
void Sys_Init()
{
	NVIC_Configuration();       // 设置中断优先级分组	
	SCT_Delay_Init();	    	//延时函数初始化
	IO_Init();				    //m1 IO口输入输出配置初始化
	//ModInit(01);  				//MODBUS初始化
	NVIC_Configuration();		// 设置中断优先级分组	
    SPI_Master_Init(); 			//dac初始化
    SPI_Slave_Init();
	//IWDG_Init(4,625);    		//看门狗初始化，预分频数为64,重载值为625,溢出时间为1s	
	//UART3_Init(9600);			//MCUBUS初始化
	UART4_Init(9600);			//485初始化
	//SPI485_SetSpeed(SPI_BaudRatePrescaler_32);  //最大不超过32分频
}

