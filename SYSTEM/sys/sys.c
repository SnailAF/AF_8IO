#include "sys.h"
#include "includes.h"
#include "delay.h"

void NVIC_Configuration(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�

}
/***************ϵͳ��ʼ��*********************************
*	�������ƣ�Sys_Init()
*	��������: ��ʼ����������
*   �������룺�ޣ�														    
*	�������أ��ޡ�
*	������д��user-F
*	��д���ڣ�2015 �� 3 �� 19 ��
*	�����汾��V1.0
******************************************************/
void Sys_Init()
{
	NVIC_Configuration();       // �����ж����ȼ�����	
	SCT_Delay_Init();	    	//��ʱ������ʼ��
	IO_Init();				    //m1 IO������������ó�ʼ��
	//ModInit(01);  				//MODBUS��ʼ��
	NVIC_Configuration();		// �����ж����ȼ�����	
    SPI_Master_Init(); 			//dac��ʼ��
    SPI_Slave_Init();
	//IWDG_Init(4,625);    		//���Ź���ʼ����Ԥ��Ƶ��Ϊ64,����ֵΪ625,���ʱ��Ϊ1s	
	//UART3_Init(9600);			//MCUBUS��ʼ��
	UART4_Init(9600);			//485��ʼ��
	//SPI485_SetSpeed(SPI_BaudRatePrescaler_32);  //��󲻳���32��Ƶ
}

