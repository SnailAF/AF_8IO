#ifndef _MBREG_H
#define _MBREG_H


/* ----------------------- Platform includes --------------------------------*/

#include "stm32f10x.h"
#if defined (__GNUC__)
#include <signal.h>
#endif
#undef CHAR
#include "mbreg.h"

/* ----------------------- Defines ------------------------------------------*/
/* holding register address */
#define REG_HOLDING_START 0x0000
/* number of regs */
#define REG_HOLDING_NREGS 100

#define REG_COILS_START 0x0000
#define REG_COILS_NREGS 8

#define REG_DISCRETE_START 0x0000
#define REG_DISCRETE_NREGS 8

#define REG_INPUT_START 0x0000
#define REG_INPUT_NREGS 8

extern uint16_t HoldReg[];
#endif
