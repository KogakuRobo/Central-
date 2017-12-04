#include"sci0_dev_driver.hpp"
#include"iodefine.h"

#include <math.h>

int _sci0_init_module(void)
{
	MSTP(SCI0) = 0;
	
	IPR(SCI0,TXI0) = 13;
	IPR(SCI0,TEI0) = 12;
	IPR(SCI0,RXI0) = 15;
	IPR(SCI0,ERI0) = 13;
}

int _sci0_set_baudrate(unsigned long rate)
{
	unsigned short N = 0;
	int n = 0;
	
	for(n = 0;n < 4;n++){
		N = (PCK_CLOCK * 1000000) /
			(32 * pow(2,2 * n - 1) *rate);
		if(N < 255)break;
	}
	if(n == 4)return -1;
	
	SCI0.SEMR.BIT.ABCS = 1;
	SCI0.SMR.BIT.CKS = n;
	SCI0.BRR = (unsigned char)N - 1;
}

int _sci0_use_parity_bit(bool t)
{
	if(t)SCI0.SMR.BIT.PE = 1;
	else SCI0.SMR.BIT.PE = 0;
	
	return 0;
}

int _sci0_set_parity_bit(unsigned char c)
{
	return 0;
}

int _sci0_gpio_enable(bool t)
{
	if(t){
		PORT2.ODR.BIT.B0 = 1;
		PORT2.ODR.BIT.B1 = 1;
		PORT2.DR.BIT.B0 = 1;		//TEbit‚ð0‚É‚·‚é‚ÆM†ü‚ªŒo‚¿‰º‚ª‚é‚½‚ß
		PORT2.DDR.BIT.B1 = 0;		//RxD1
		PORT2.DDR.BIT.B0 = 1;		//TxD1
		PORT2.ICR.BIT.B1 = 1;
	}
	else{
		PORT2.ICR.BIT.B1 = 0;
		PORT2.DR.BIT.B0 = 1;
		PORT2.DDR.BIT.B1 = 0;		//RxD1
		PORT2.DDR.BIT.B0 = 0;		//TxD1
	}
	return 0;
}

int _sci0_rx_interrupt_enable(bool t)
{
	if(t){
		IEN(SCI0,RXI0) = 1;
		IEN(SCI0,ERI0) = 1;
	}
	else{
		IEN(SCI0,RXI0) = 0;
		IEN(SCI0,ERI0) = 0;
	}
	return 0;
}