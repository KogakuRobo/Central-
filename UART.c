#include "UART.h"
#include "stdio.h"
#include "iodefine.h"

char g_DATA[255];

void DMAC0_Init(void){
	
	const static char *InitialCall = "Initial\r\n";

	MSTP(DMAC) = 0;
			
	ICU.DMRSR0 = VECT(SCI1,TXI1);
	
	DMAC0.DMCNT.BIT.DTE = 0;
	DMAC0.DMAMD.WORD = 0x8000;
	DMAC0.DMTMD.WORD = 0x0001;
	
	DMAC0.DMSAR = (void *)InitialCall;
	DMAC0.DMDAR = (void *)&SCI1.TDR;
	DMAC0.DMCRA = 9;
	
	DMAC0.DMCNT.BIT.DTE = 1;
	DMAC.DMAST.BIT.DMST = 1;
}

void SCI1_Init(void){

	MSTP(SCI1) = 0;
	
	SCI1.SMR.BYTE = 0x00;
	SCI1.SEMR.BIT.ABCS = 1;
	SCI1.BRR = 26-1;
	
	DMAC0_Init();
	
	IPR(SCI1,TXI1) = 15;
	IEN(SCI1,TXI1) = 1;
	
	SCI1.SCR.BIT.TIE = 1;
	
	SCI1.SCR.BIT.TE = 1;
}

void printf_Init(void)
{
	DMAC0_Init();
	SCI1_Init();
}

void sci_print(char *data,int len)
{
	int i;
	while(DMAC0.DMSTS.BIT.DTIF != 1);
	
	for(i = 0;i < len ;i++){
		g_DATA[i] = data[i];
	}
	
	DMAC0.DMSAR = (void *)g_DATA;
	DMAC0.DMCRA = len;
	
	DMAC0.DMCNT.BIT.DTE = 1;
	SCI1.SCR.BIT.TE = 1;
}

void sci_printf(char *format,...)
{
	va_list arg;
	int num;
	
	while(DMAC0.DMSTS.BIT.DTIF != 1);
	
	va_start(arg,format);
	
	num = vsprintf(g_DATA,format,arg);

	DMAC0.DMSAR = (void *)g_DATA;
	DMAC0.DMCRA = num;
	
	DMAC0.DMCNT.BIT.DTE = 1;
	SCI1.SCR.BIT.TE = 1;
}