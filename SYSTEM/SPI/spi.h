#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
 

 
 void SPI485_Init(void);			 //初始化SPI口
 void SPI485_SetSpeed(uint16_t SpeedSet); //设置SPI速度   
 uint8_t SPI1_ReadWriteByte(uint8_t TxData);
 void SPI1_SetSpeed(uint16_t SpeedSet);
 void SPI1_Init(void);
 void SPI_Master_Init(void);
 void SPI_Slave_Init(void);
 uint8_t SPI485_ReadWriteByte(uint8_t TxData);//SPI总线读写一个字节
 uint8_t SPI485_WriteByte(uint16_t TxData);	 
#endif

