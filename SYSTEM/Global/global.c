#include "global.h"


/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/
 //uint16_t 	HoldReg[100] = {0x0001,0x0203,0x0304,0x0506,0x0708,0x090a,0x0b0c,0x0d0e};								//modbus ����
 uint16_t    Delay_Time[DelayTime_length];  				//����ʹ�õ�����ʱ���� DelayTime_length��delay.h��
 uint16_t  	Delay_TimeFlag[DelayTime_length];  			//������ʱ�ж���־λ 
 uint8_t     Input_Buffer[INPUT_IO_LENGTH]; 	            //���뻺��
 uint8_t     Output_Buffer[OUTPUT_IO_LENGTH];	        //���IO����
 uint8_t     DI_UP[8] = {0};                             //����������
 uint8_t     DI_DOWN[8] = {0};                           //�����½���
// uint8_t     G_LANstate;                                 //�������״̬��0�����ܣ�1���յ�"{",2���յ�"}".
// char   JSONRcvbuf[200] = {0};						//�������JSON�������飬��һλΪ��Ч���֣�
// char   *PointJSONRcvbuf = JSONRcvbuf;				//JSONbuf��ָ�룻
