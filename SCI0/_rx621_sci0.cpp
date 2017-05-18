#include"_rx621_sci0.hpp"
#include"_rx621_dtc.hpp"
#include"iodefine.h"
#include<stdio.h>
#include<machine.h>

static char g_DATA[256];
static volatile DTC_TABLE *tx_table;

void sci0_uart_init(void){
	
	MSTP(SCI0) = 0;
	
	PORT2.DDR.BIT.B1 = 0;		//RxD1
	PORT2.DDR.BIT.B0 = 1;		//TxD1
	
	PORT2.ICR.BIT.B1 = 1;
	
	SCI0.SMR.BYTE = 0x00;
	SCI0.SEMR.BIT.ABCS = 1;
	SCI0.BRR = 26 - 1;
	
	IPR(SCI0,TXI0) = 13;
	IEN(SCI0,TXI0) = 1;
	IPR(SCI0,TEI0) = 12;
	IEN(SCI0,TEI0) = 1;
	tx_table = DTC_CreateVect(VECT(SCI0,TXI0));
	tx_table->CRA = 0x0000;
#ifndef SCI0_DTC_USE
	#error SCI0のDTCの使用が定義されていません。
#endif
}

int sci0_printf(char *format,...){
	va_list arg;
	int i;
	
	while(tx_table->CRA != 0);

	va_start(arg,format);
	i = vsprintf(g_DATA,format,arg);
	va_end(arg);
	
	DTC.DTCCR.BIT.RRS = 0;
	
	tx_table->MRA.BIT.MD = 0x00;			//ノーマル転送
	tx_table->MRA.BIT.SZ = 0x00;			//ロングワードサイズ
	tx_table->MRA.BIT.SM = 0x02;			//転送元をインクリメント
	tx_table->SAR = (void *)g_DATA;	//転送元をバッファに設定
	
	tx_table->MRB.BIT.CHNE = 0;			//チェーン転送禁止
	tx_table->MRB.BIT.CHNS = 0;			//意味なし
	tx_table->MRB.BIT.DISEL = 0;			//全データ転送後割り込み
	tx_table->MRB.BIT.DTS = 0;			//意味なし
	tx_table->MRB.BIT.DM = 0x00;			//アドレス固定
	tx_table->DAR = (void *)&SCI0.TDR;	//転送先にSPDRを設定
	
	tx_table->CRA = i;
	tx_table->CRB = 0xFFFF;
	
	DTC_SetTable(VECT(SCI0,TXI0),tx_table);
	DTCE(SCI0,TXI0) = 1;

	DTC.DTCCR.BIT.RRS = 1;
	
	while(SCI0.SSR.BIT.TEND != 1);
	
	while(SCI0.SCR.BYTE != 0xa0)SCI0.SCR.BYTE = 0xa0;
	return i;
	
}

#pragma interrupt SCI0_TXI0(vect = VECT(SCI0,TXI0))
void SCI0_TXI0(void)
{
	SCI0.SCR.BIT.TEIE = 1;
}

#pragma interrupt SCI0_TEI0(vect = VECT(SCI0,TEI0))
void SCI0_TEI0(void)
{
	if(tx_table->CRA != 0){//念のため。
		return ;
	}
	SCI0.SCR.BIT.TE = 0;
	SCI0.SCR.BIT.TEIE = 0;
	while(SCI0.SCR.BYTE != 0x80);
}