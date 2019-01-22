 #include "includes.h"
#include "stm32f10x_spi.h"
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "GAME.h"
#include "test.h"
#include "delay.h"
#include "mb.h"
#define IO_MODE HoldReg[0]

// MODE COM6 9600,0,8,1 ASSIGN COM6 <S2IN>S2OUT

/*
*************************************************************************************************
                                    全局变量
*************************************************************************************************
*/
 extern uint8_t SPI1_Buffer_Rx[200];
 extern u8 SPI1_Buffer_Tx[200];
 void TEST(void);
 void (*Printf)(char *fmt, ...);
 int main(void)
 {	
    u8 spidata = 0xff;
    u8 i;
    u8 seed= 0x01;
    u8 inbuf[8];
    u8 outbuf[8];
    u8 spi_send_temp;
	Sys_Init();						//系统初始化
    Printf = U4Printf;
    RW_485 = WRITE;
	Printf(" Initial success\n");
    RW_485 = READ;
    //IO_Init();  
     IO_MODE = 01;
     HoldReg[0] = 04;
     IO_OE    = RESET;
     delay_ms(100);
     	eMBInit( MB_RTU, 0x01, 0, 9600, MB_PAR_NONE );
	
	/* Enable the Modbus Protocol Stack. */
	eMBEnable(  );
//     while(1)
//     {
//        DO0 = RESET;
//         //DO1 = RESET;
//        delay_us(1);
//         DO0 = SET;
//         //DO1 = SET;
//        delay_us(1);
//     }
    while(1)
    {      
        IO_MODE = HoldReg[0];
        Input_Scan();
        Output_Scan();
        if(IO_MODE == 01)       //主模式
        {            
            Output_Buffer[0] = Input_Buffer[7];
            Output_Buffer[1] = Input_Buffer[6];
            Output_Buffer[2] = Input_Buffer[5];
            Output_Buffer[3] = Input_Buffer[4];
            Output_Buffer[4] = Input_Buffer[3];
            Output_Buffer[5] = Input_Buffer[2];
            Output_Buffer[6] = Input_Buffer[1];
            Output_Buffer[7] = Input_Buffer[0];
        }
        if(IO_MODE == 02)       //从模式
        {
            SPI485_TEST(0);
            spi_send_temp = 0;
            for(i=0; i< 8; i++)
            {
                outbuf[i] = (SPI1_Buffer_Rx[0]>>i) & 0x01;
                spi_send_temp |= (inbuf[i]&0x01)<<i;
            }
            SPI1_Buffer_Tx[0] = spi_send_temp;
            inbuf[0] = DI0;
            inbuf[1] = DI1;
            inbuf[2] = DI2;
            inbuf[3] = DI3;
            inbuf[4] = DI4;
            inbuf[5] = DI5;
            inbuf[6] = DI6;
            inbuf[7] = DI7;
            DO0 = outbuf[7];
            DO1 = outbuf[6];
            DO2 = outbuf[5];
            DO3 = outbuf[4];
            DO4 = outbuf[3];
            DO5 = outbuf[2];
            DO6 = outbuf[1];
            DO7 = outbuf[0];
        }
        
        if(IO_MODE == 03)       //testself
        {
            SPI485_TEST(1); //把读到的数据在下次发送出去 
            spi_send_temp = 0;
            for(i=0; i< 8; i++)
            {
                outbuf[i] = (SPI1_Buffer_Rx[0]>>i) & 0x01;
                spi_send_temp |= (inbuf[i]&0x01)<<i;
            }
            SPI1_Buffer_Tx[0] = spi_send_temp;
            inbuf[0] = DI0;
            inbuf[1] = DI1;
            inbuf[2] = DI2;
            inbuf[3] = DI3;
            inbuf[4] = DI4;
            inbuf[5] = DI5;
            inbuf[6] = DI6;
            inbuf[7] = DI7;
            DO0 = outbuf[0];
            DO1 = outbuf[1];
            DO2 = outbuf[2];
            DO3 = outbuf[3];
            DO4 = outbuf[4];
            DO5 = outbuf[5];
            DO6 = outbuf[6];
            DO7 = outbuf[7];
        }
        RUN_LED_Desplay(500);
        IWDG_Feed();  //喂狗   
        delay_ms(1);
    }	
}
void TEST(void)
{
    while(1)
	{		
        delay_ms(10);
        SPI485_TEST(8);
        LED_TEST();
        IO_TEST();		
        IWDG_Feed();  //喂狗。
        delay_us(1);
        Output_Buffer[0] = Input_Buffer[0];
        Output_Scan();
        Input_Scan();;
     //  delay_ms(100);
	}
}







