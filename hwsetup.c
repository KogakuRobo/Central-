/************************************************************************
*
* Device     : RX/RX600/RX621,RX62N
*
* File Name  : hwsetup.c
*
* Abstract   : Hardware Setup file.
*
* History    : 1.00  (2010-03-05)  [Hardware Manual Revision : 0.50]
*
* NOTE       : THIS IS A TYPICAL EXAMPLE.
*
* Copyright (C) 2009 Renesas Electronics Corporation.
* and Renesas Solutions Corporation. All rights reserved.
*
************************************************************************/

#include <stddef.h>
#include <stdio.h>
#include "lowsrc.h"
#include "CentralLibrary.h"
#include "syscall_table.h"
#include "Description.h"
#include "iodefine.h"

#include "./DTC/_rx621_dtc.h"
#include "./E1/E1_lib.h"
#include "../SCI0/sci0_lowsrc.h"

#ifdef __cplusplus
extern "C" {
#endif
extern void HardwareSetup(void);
#ifdef __cplusplus
}
#endif

void OSC_Init(void);
extern void main(void);
void HardwareSetup(void)
{
	OSC_Init();
	E1_init();
	DTC_init();
	sci0_init();
	
	sys_init();
}

void OSC_Init(void)
{
	SYSTEM.SCKCR.BIT.ICK = 0;
	while(SYSTEM.SCKCR.BIT.ICK != 0);
	SYSTEM.SCKCR.BIT.PSTOP1 = 1;
	SYSTEM.SCKCR.BIT.PSTOP0 = 1;
	SYSTEM.SCKCR.BIT.BCK = 1;
	SYSTEM.SCKCR.BIT.PCK = 1;
	while(SYSTEM.SCKCR.BIT.PCK != 1);
	
	SYSTEM.SUBOSCCR.BIT.SUBSTOP = 1;
}