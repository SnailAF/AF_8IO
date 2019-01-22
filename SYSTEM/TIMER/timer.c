#include "timer.h"
#include "stm32f10x_tim.h"
//#include "includes.h"

/*****************************************************************************************

*	����1��TIM2_Int_Init(); modbus֡�ж���ʱ����ʼ��
*	����2��TIM4_Int_Init(); SV2�������������ģʽ��ʼ��
*	����3��TIM4_IRQHandler();��ʱ��4�жϺ���
*	����4��TIM3_Int_Init(uint16_t arr,uint16_t psc)��SV3�������������ģʽ��ʼ��
*	����5��TIM3_IRQHandler();��ʱ��3�жϺ���

*******************************************************************************************/


/***************TIM2(ModBusר��) ��ʼ��*********************************
*	�������ƣ�TIM2_Int_Init();
*	�������ܣ�TIM2��ʼ����ModBus��֡�����ж϶�ʱ��
*	����������ModBus.c->ModInit()��
* 	�������룺��ʱ����arr��ʱ��Ԥ��Ƶ���� ʱ��T = ��arr+1��*��psc+1��/72M														    
*	�������أ��ޣ� 
*	������д��user-F
*	��д���ڣ�2015 �� 3 �� 25 ��
*	�����汾��V1.0
******************************************************/
void TIM2_Int_Init(uint16_t arr,uint16_t psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //TIM2ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM2, //TIM2
		TIM_IT_Update ,
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //ǿռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx����
							 
}

/***************TIM4(SV2��������) ��ʼ��*********************************
*	�������ƣ�TIM4_Int_Init();
*	�������ܣ�TIM4��ʼ����SV2�������룬����Ϊ������ģʽ2�����ϼ�����Ԥ��ƵΪ0
			  �½�����Ч������Ҫ�˲�����
* 	�������룺��װ��ֵarr����Ƶ��psc�� ���� N = ��arr+1��*��psc+1��													    
*	�������أ��ޣ� 
*	������д��user-F
*	��д���ڣ�2015 �� 3 �� 25 ��
*	�����汾��V1.0
******************************************************/
void TIM4_Int_Init(uint16_t arr,uint16_t psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//PEO/T4_ETR,�������룬50Mz
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);   //ʹ��PEͨ����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       		//PEO/TIME4_ETR
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	//��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//50Mz����
	GPIO_Init(GPIOE, &GPIO_InitStructure);					//��ʼ��

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //TIM4ʱ��ʹ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ ����������
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM4���ϼ���ģʽ��
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	TIM_ETRClockMode2Config(TIM4,TIM_ExtTRGPSC_DIV4,TIM_ExtTRGPolarity_Inverted,0); //�ⲿ����ģʽ����4��Ƶ������Ч������Ҫ�˲���
	//TIM_ETRClockMode2Config(TIM4,TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_Inverted,0); //�ⲿ����ģʽ��������Ƶ������Ч������Ҫ�˲���

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //ǿռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	TIM_ITConfig( TIM4,TIM_IT_Update ,ENABLE );  //ʹ��TIM4�ж�
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx����
}

#if 1
void TIM4_IRQHandler(void)   //TIM4�ж�
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
				
		}
}
#endif
/***************TIM3(PWM����ʼ��*********************************
*	�������ƣ�TIM3_PWM_Init();
*	�������ܣ�TIM3��ʼ����PWM��ʼ�� TIM3_CH4
* 	�������룺��ʱ����arr��ʱ��Ԥ��Ƶ���� ʱ��T = ��arr+1��*��psc+1��/72M														    
*	�������أ��ޣ� 
*	������д��user-F
*	��д���ڣ�2015 �� 3 �� 25 ��
*	�����汾��V1.0
******************************************************/
void TIM3_PWM_Init(uint16_t arr,uint16_t psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);    // 
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //ʹ��GPIO����ʱ��ʹ��
	                                                                     	

   //���ø�����Ϊ�����������,���TIM3 CH1��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;		 		//TIM3_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  		//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	
	TIM_TimeBaseStructure.TIM_Period = arr; 		//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc;		//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
													//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
													//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; 				//���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
													//�������:TIM����Ƚϼ��Ը�
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  		//����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIM3_CH4
 

	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);//CH4Ԥװ��ʹ��	 
	
	TIM_ARRPreloadConfig(TIM3, ENABLE); 			//ʹ��TIM3��ARR�ϵ�Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM3, ENABLE);  						//ʹ��TIM3
							 
}
#if 1
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  ); //���TIMx���жϴ�����λ:TIM �ж�Դ 
			
	}
}
#endif
/***************TIM5(SPIר��) ��ʼ��*********************************
*	�������ƣ�TIM5_Int_Init();
*	�������ܣ�TIM5��ʼ����
*	����������DAC.c
* 	�������룺��ʱ����arr��ʱ��Ԥ��Ƶ���� ʱ��T = ��arr+1��*��psc+1��/72M														    
*	�������أ��ޣ� 
*	������д��user-F
*	��д���ڣ�2015 �� 3 �� 25 ��
*	�����汾��V1.0
******************************************************/
void TIM5_Int_Init(uint16_t arr,uint16_t psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //TIM5ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM5, //TIM5
		TIM_IT_Update ,
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM5�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //ǿռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM5, DISABLE);  //ʧ��TIMx����
							 
}







