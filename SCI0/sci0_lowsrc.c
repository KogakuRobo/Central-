#include "CentralLibrary.h"
#include <stddef.h>
#include "../lowsrc.h"
#include "sci0_lowsrc.h"
#include "_rx621_dtc.h"

static const char g_sci0_path[] = SCI0_PATH;
static volatile DTC_TABLE *tx_table = NULL;

static int n_openfile = 0;					//sci0の開いたファイル数
#define IS_OPEN_FILE_NOTHING	(n_openfile == 0)

long sci0_open(_FD*,const char*,long);
long sci0_write(_FD*,const unsigned char*,long);
long sci0_read(_FD*,unsigned char*,long);
long sci0_close(_FD*);

_FD _sci0_driver = {
	._open = sci0_open,
	._read = sci0_read,
	._write = sci0_write,
	._close = sci0_close,
};

long sci0_open(_FD* fd,
	const char *path,
	long mode)
{
	if(IS_OPEN_FILE_NOTHING){
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
	}
	n_openfile++;
	return 0;
}

long sci0_close(_FD *fd)
{
	if(n_openfile > 1)
		n_openfile--;
	else if(n_openfile == 1)
	{
		n_openfile--;
		IEN(SCI0,TXI0) = 0;
		IEN(SCI0,TEI0) = 0;
		
		MSTP(SCI0) = 1;
		
	}
	else return -1;
	return 0;
}

long sci0_write(_FD *fd,
	const unsigned char *buf,
	long count)
{
	DTC.DTCCR.BIT.RRS = 0;
	
	tx_table->MRA.BIT.MD = 0x00;			//ノーマル転送
	tx_table->MRA.BIT.SZ = 0x00;			//ロングワードサイズ
	tx_table->MRA.BIT.SM = 0x02;			//転送元をインクリメント
	tx_table->SAR = (void *)buf;	//転送元をバッファに設定
	
	tx_table->MRB.BIT.CHNE = 0;			//チェーン転送禁止
	tx_table->MRB.BIT.CHNS = 0;			//意味なし
	tx_table->MRB.BIT.DISEL = 0;			//全データ転送後割り込み
	tx_table->MRB.BIT.DTS = 0;			//意味なし
	tx_table->MRB.BIT.DM = 0x00;			//アドレス固定
	tx_table->DAR = (void *)&SCI0.TDR;	//転送先にSPDRを設定
	
	tx_table->CRA = count;
	tx_table->CRB = 0xFFFF;
	
	DTC_SetTable(VECT(SCI0,TXI0),tx_table);
	DTCE(SCI0,TXI0) = 1;

	DTC.DTCCR.BIT.RRS = 1;
	
	while(SCI0.SSR.BIT.TEND != 1);
	
	while(SCI0.SCR.BYTE != 0xa0)SCI0.SCR.BYTE = 0xa0;
	return count;
}

long sci0_read(_FD *fd,
	unsigned char *buf,
	long count)
{
	return -1;
}

void sci0_init(void)
{
	set_io_driver(g_sci0_path,&_sci0_driver);
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