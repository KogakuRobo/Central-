#include "CentralLibrary.h"
#include <stddef.h>
#include <stdlib.h>
#include "../lowsrc.hpp"
#include "sci0_lowsrc.hpp"
#include "_rx621_dtc.h"

#include "CentralLibrary.h"
#include "syscall_table.h"

#define IS_OPEN_FILE_NOTHING	(n_openfile == 0)

const unsigned char 	sci0_file_desc_factor::g_sci0_path[] 	= SCI0_PATH;
int 			sci0_file_desc_factor::n_openfile 	= 0;

thread_list		sci0_file_desc::t_list			;
DTC_TABLE*	 	sci0_file_desc::tx_table		= NULL;

_low_file_desc_class* sci0_file_desc_factor::open(
	const char *path,
	long mode)
{
	if(IS_OPEN_FILE_NOTHING){
		SCI0.SMR.BYTE = 0x00;
		SCI0.SEMR.BIT.ABCS = 1;
		SCI0.BRR = 26 - 1;
		
		PORT2.DDR.BIT.B1 = 0;		//RxD1
		PORT2.DDR.BIT.B0 = 1;		//TxD1
		PORT2.DR.BIT.B0 = 1;		//TEbitを0にすると信号線が経ち下がるため
		PORT2.ICR.BIT.B1 = 1;
		
		IPR(SCI0,TXI0) = 13;
		IPR(SCI0,TEI0) = 12;
	}
	n_openfile++;
	return new sci0_file_desc;
}

long sci0_file_desc_factor::close(_low_file_desc_class* desc)
{
	if(n_openfile > 1)
		n_openfile--;
	else if(n_openfile == 1)
	{
		n_openfile--;
		IEN(SCI0,TXI0) = 0;
		IEN(SCI0,TEI0) = 0;
	}
	else return -1;
	delete desc;
	
	return 0;
}

long sci0_file_desc::write(
	const unsigned char *buf,
	long count)
{

	this->buf_allocate(count);
	for(int i = 0;i < count;i++){
		tx_buf[i] = buf[i];
	}
	
	DTC.DTCCR.BIT.RRS = 0;
	
	tx_table->MRA.BIT.MD = 0x00;			//ノーマル転送
	tx_table->MRA.BIT.SZ = 0x00;			//ロングワードサイズ
	tx_table->MRA.BIT.SM = 0x02;			//転送元をインクリメント
	tx_table->SAR = (void *)tx_buf;	//転送元をバッファに設定
	
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
	
	IR(SCI0,TEI0) = 0;
	
	while(SCI0.SCR.BYTE != 0xa4)SCI0.SCR.BYTE = 0xa4;
	IEN(SCI0,TXI0) = 1;
	while(SCI0.SSR.BIT.TEND != 1);		//送信が開始されたことを確認
	IEN(SCI0,TEI0) = 1;
	
	if(SCI0.SSR.BIT.TEND != 1){
		suspend();
	}
	
	buf_clear();
	
	return count;
}

long sci0_file_desc::read(
	unsigned char *buf,
	long count)
{
	SCI0.SCR.BIT.RE = 1;
	while(SCI0.SSR.BIT.RDRF != 1);
	buf[0] = SCI0.RDR;
	SCI0.SCR.BIT.RE = 0;
	return 1;
}

sci0_file_desc_factor::sci0_file_desc_factor(void)
{
	MSTP(SCI0) = 0;
	set_io_driver(&sci0_factor);
}

sci0_file_desc::sci0_file_desc(void)
{
	if(this->tx_table == NULL){
		this->tx_table = (DTC_TABLE*)DTC_CreateVect(VECT(SCI0,TXI0));
		this->tx_table->CRA = 0x0000;
	}
}

#pragma interrupt SCI0_TXI0(vect = VECT(SCI0,TXI0))
void SCI0_TXI0(void)
{
	SCI0.SCR.BIT.TIE = 0;
}

#pragma interrupt SCI0_TEI0(vect = VECT(SCI0,TEI0),enable)
void SCI0_TEI0(void)
{
	SCI0.SCR.BIT.TE = 0;
	SCI0.SCR.BIT.TIE = 0;
	SCI0.SCR.BIT.TEIE = 0;
	sci0_file_desc::resume();
}

int sci0_file_desc::suspend(void){
	thread_t* tid;
	tid = get_tid();
	t_list.push_back(tid);
	thread_suspend(tid);
	return 0;
}

int sci0_file_desc::resume(void){
	thread_t* tid;
	if(!t_list.empty()){
		tid = t_list.pop_front();
		thread_resume(tid);
	}
	return 0;
}

int sci0_file_desc::buf_allocate(int count){
	tx_buf = (unsigned char*)malloc(sizeof(unsigned char) * count);
	return 0;
}

int sci0_file_desc::buf_clear(void){
	if(tx_buf != NULL){
		free(tx_buf);
		tx_buf = NULL;
	}
	return 0;
}
