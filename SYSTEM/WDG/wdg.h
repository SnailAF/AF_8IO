#ifndef __WDG_H
#define __WDG_H
//#include "sys.h"
#include "stm32f10x_iwdg.h"
#include "stm32f10x.h"



void IWDG_Init(uint8_t prer,uint16_t rlr);
void IWDG_Feed(void);

#endif
