#include "timer.h"
#include "stm32f10x_tim.h"
//#include "includes.h"

/*****************************************************************************************

*	函数1：TIM2_Int_Init(); modbus帧判定定时器初始化
*	函数2：TIM4_Int_Init(); SV2高速输入计数器模式初始化
*	函数3：TIM4_IRQHandler();定时器4中断函数
*	函数4：TIM3_Int_Init(uint16_t arr,uint16_t psc)；SV3高速输入计数器模式初始化
*	函数5：TIM3_IRQHandler();定时器3中断函数

*******************************************************************************************/


/***************TIM2(ModBus专用) 初始化*********************************
*	函数名称：TIM2_Int_Init();
*	函数功能：TIM2初始化，ModBus的帧结束判断定时器
*	被引函数：ModBus.c->ModInit()；
* 	函数输入：定时周期arr，时钟预分频数， 时间T = （arr+1）*（psc+1）/72M														    
*	函数返回：无， 
*	函数编写：user-F
*	编写日期：2015 年 3 月 25 日
*	函数版本：V1.0
******************************************************/
void TIM2_Int_Init(uint16_t arr,uint16_t psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //TIM2时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM2, //TIM2
		TIM_IT_Update ,
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //强占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设
							 
}

/***************TIM4(SV2高速输入) 初始化*********************************
*	函数名称：TIM4_Int_Init();
*	函数功能：TIM4初始化，SV2高速输入，配置为计数器模式2，向上计数，预分频为0
			  下降沿有效，不需要滤波器。
* 	函数输入：重装载值arr，分频数psc， 个数 N = （arr+1）*（psc+1）													    
*	函数返回：无， 
*	函数编写：user-F
*	编写日期：2015 年 3 月 25 日
*	函数版本：V1.0
******************************************************/
void TIM4_Int_Init(uint16_t arr,uint16_t psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//PEO/T4_ETR,浮空输入，50Mz
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);   //使能PE通道，
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       		//PEO/TIME4_ETR
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	//下拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//50Mz输入
	GPIO_Init(GPIOE, &GPIO_InitStructure);					//初始化

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //TIM4时钟使能
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值 计数个数，
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM4向上计数模式；
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	TIM_ETRClockMode2Config(TIM4,TIM_ExtTRGPSC_DIV4,TIM_ExtTRGPolarity_Inverted,0); //外部触发模式二，4分频，低有效，不需要滤波器
	//TIM_ETRClockMode2Config(TIM4,TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_Inverted,0); //外部触发模式二，不分频，低有效，不需要滤波器

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //强占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级1级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	TIM_ITConfig( TIM4,TIM_IT_Update ,ENABLE );  //使能TIM4中断
	TIM_Cmd(TIM4, ENABLE);  //使能TIMx外设
}

#if 1
void TIM4_IRQHandler(void)   //TIM4中断
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
				
		}
}
#endif
/***************TIM3(PWM）初始化*********************************
*	函数名称：TIM3_PWM_Init();
*	函数功能：TIM3初始化，PWM初始化 TIM3_CH4
* 	函数输入：定时周期arr，时钟预分频数， 时间T = （arr+1）*（psc+1）/72M														    
*	函数返回：无， 
*	函数编写：user-F
*	编写日期：2015 年 3 月 25 日
*	函数版本：V1.0
******************************************************/
void TIM3_PWM_Init(uint16_t arr,uint16_t psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);    // 
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //使能GPIO外设时钟使能
	                                                                     	

   //设置该引脚为复用输出功能,输出TIM3 CH1的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;		 		//TIM3_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  		//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	
	TIM_TimeBaseStructure.TIM_Period = arr; 		//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc;		//设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
													//TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
													//比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; 				//设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
													//输出极性:TIM输出比较极性高
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  		//根据TIM_OCInitStruct中指定的参数初始化外设TIM3_CH4
 

	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);//CH4预装载使能	 
	
	TIM_ARRPreloadConfig(TIM3, ENABLE); 			//使能TIM3在ARR上的预装载寄存器
	
	TIM_Cmd(TIM3, ENABLE);  						//使能TIM3
							 
}
#if 1
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  ); //清除TIMx的中断待处理位:TIM 中断源 
			
	}
}
#endif
/***************TIM5(SPI专用) 初始化*********************************
*	函数名称：TIM5_Int_Init();
*	函数功能：TIM5初始化，
*	被引函数：DAC.c
* 	函数输入：定时周期arr，时钟预分频数， 时间T = （arr+1）*（psc+1）/72M														    
*	函数返回：无， 
*	函数编写：user-F
*	编写日期：2015 年 3 月 25 日
*	函数版本：V1.0
******************************************************/
void TIM5_Int_Init(uint16_t arr,uint16_t psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //TIM5时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM5, //TIM5
		TIM_IT_Update ,
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM5中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //强占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM5, DISABLE);  //失能TIMx外设
							 
}







