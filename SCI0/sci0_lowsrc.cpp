#include "CentralLibrary.h"
#include <stddef.h>
#include <stdlib.h>
#include <machine.h>
#include "../lowsrc.hpp"
#include "sci0_lowsrc.hpp"
#include "sci0_dev_driver.hpp"
#include "_rx621_dtc.h"
#include "_dtc_ring_buff.hpp"

#include "CentralLibrary.h"
#include "syscall_table.h"
#include "thread_control.hpp"

#define IS_OPEN_FILE_NOTHING	(n_openfile == 0)

const unsigned char 	sci0_file_desc_factor::g_sci0_path[] 	= SCI0_PATH;
int 			sci0_file_desc_factor::n_openfile 	= 0;

DTC_TABLE*	 	sci0_file_desc::tx_table		= NULL;

class stc_write{
	thread_t *tid;
	unsigned char *buff;
	long count;
	sci0_file_desc *file_desc;
public:
	stc_write(thread_t* _tid,unsigned char *_buff,long _c,sci0_file_desc *_file_desc):
	tid(_tid),buff(_buff),count(_c),file_desc(_file_desc){}

	thread_t* GetTid(void){return tid;}
	long GetCount(void){return count;}
	unsigned char*	GetBuff(void){return buff;}
};

RingBuff<stc_write*,16> write_wait;

static struct{
	//wait_flag:起動中0,停止中(wait)1.
	unsigned char thread_wait:1;
}tx_control_thread_flags;
thread_t		sci0_file_desc::tx_control_thread;


thread_t	*rx_wait_t;
RingBuff<unsigned char,256> rx_buff;
DtcRingBuff<unsigned char,32> *rx_buff_2;


//SCI0デバイスドライバロード関数（コンストラクタ）
//SCIモジュールを動作させ、カーネルに通知します。
sci0_file_desc_factor::sci0_file_desc_factor(void)
{
	_sci0_init_module();
	set_io_driver(&sci0_factor);
}

//SCI0デバイスドライバopenメソッド。
//ファイルクラスを作成し、そのアドレスを返します。
//内部処理は、通信開始とバッファ初期化
_low_file_desc_class* sci0_file_desc_factor::open(
	const char *path,
	long mode)
{
	static sci0_file_desc * file;
	if(IS_OPEN_FILE_NOTHING){
		
		SCI0.SCR.BYTE = 0;
		
		_sci0_gpio_enable(true);
		_sci0_use_parity_bit(false);
		_sci0_set_baudrate(57600);
		
		nop();
		nop();
		
		_sci0_rx_interrupt_enable(true);
		
		rx_buff_2 = new DtcRingBuff<unsigned char,32>(VECT(SCI0,RXI0));
		rx_buff_2->enable((void*)&SCI0.RDR);
		DTCE(SCI0,RXI0) = 1;
		
		SCI0.SCR.BYTE = 0xf4;
		char dumy = SCI0.RDR;
		
		rx_wait_t = NULL;
		file = new sci0_file_desc;
	}
	n_openfile++;
	return file;
}


sci0_file_desc::sci0_file_desc(void)
{
	tx_control_thread_flags.thread_wait = 0;
	if(this->tx_table == NULL){
		this->tx_table = (DTC_TABLE*)DTC_CreateVect(VECT(SCI0,TXI0));
		this->tx_table->CRA = 0x0000;
	}
	
	thread_create(&tx_control_thread,CT_PRIORITY_MAX + 1,sci0_file_desc::_tx_control_handle,(void*)this);
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
	stc_write* cmd = new stc_write(get_tid(),(unsigned char*)buf,count,this);
	write_wait.enqueue(cmd);
	
	if(tx_control_thread_flags.thread_wait == 1)thread_resume(&tx_control_thread);
	thread_suspend(get_tid());
	delete cmd;
	return count;
}

long sci0_file_desc::read(
	unsigned char *buf,
	long count)
{
	for(int i =0;i < count;i++){
		if(rx_buff_2->isEmpty()){
			rx_wait_t = get_tid();
			thread_suspend(rx_wait_t);
		}
		rx_buff_2->dequeue(buf[i]);
		
	}
		
	return count;
}

void* sci0_file_desc::_tx_control_handle(thread_t* tid,void* attr)
{
	stc_write *cmd;
	sci0_file_desc *This = (sci0_file_desc*)attr;
	while(1){
		if(write_wait.isEmpty()){	//write待ちがなければスリープ
			tx_control_thread_flags.thread_wait = 1;
			thread_suspend(tid);
			continue;		//再びチェック
		}
		//待ちフラグを解除 キューを取得
		tx_control_thread_flags.thread_wait = 0;
		write_wait.dequeue(cmd);
		
		This->buf_allocate(cmd->GetCount());
		for(int i = 0;i < cmd->GetCount();i++){
			This->tx_buf[i] = cmd->GetBuff()[i];
		}
		if(cmd->GetCount() != 1){
			DTC.DTCCR.BIT.RRS = 0;
			
			tx_table->MRA.BIT.MD = 0x00;			//ノーマル転送
			tx_table->MRA.BIT.SZ = 0x00;			//ロングワードサイズ
			tx_table->MRA.BIT.SM = 0x02;			//転送元をインクリメント
			tx_table->SAR = (void *)This->tx_buf[1];	//転送元をバッファに設定
			
			tx_table->MRB.BIT.CHNE = 0;			//チェーン転送禁止
			tx_table->MRB.BIT.CHNS = 0;			//意味なし
			tx_table->MRB.BIT.DISEL = 0;			//全データ転送後割り込み
			tx_table->MRB.BIT.DTS = 0;			//意味なし
			tx_table->MRB.BIT.DM = 0x00;			//アドレス固定
			tx_table->DAR = (void *)&SCI0.TDR;	//転送先にSPDRを設定
			
			tx_table->CRA = cmd->GetCount() - 1;
			tx_table->CRB = 0xFFFF;
	
			DTC_SetTable(VECT(SCI0,TXI0),tx_table);
			DTCE(SCI0,TXI0) = 1;

			DTC.DTCCR.BIT.RRS = 1;
		}
		IEN(SCI0,TXI0) = 1;
		SCI0.TDR = This->tx_buf[0];
		
		while(SCI0.SSR.BIT.TEND == 1);		//送信が開始されたことを確認
		IEN(SCI0,TEI0) = 1;
	
		if(SCI0.SSR.BIT.TEND != 1){
			thread_suspend(tid);
		}
	
		This->buf_clear();
		thread_resume(cmd->GetTid());
	}
	
	
	return NULL;
}

void* sci0_file_desc::_rx_control_handle(thread_t* tid,void* attr)
{
	
	return NULL;
}

#pragma interrupt SCI0_TXI0(vect = VECT(SCI0,TXI0))
void SCI0_TXI0(void)
{
}

#pragma interrupt SCI0_TEI0(vect = VECT(SCI0,TEI0),enable)
void SCI0_TEI0(void)
{
	SCI0.SSR.BIT.TEND = 0;
	IEN(SCI0,TEI0) = 0;
	kernel_resume_thread(&sci0_file_desc::tx_control_thread);
}

#pragma interrupt SCI0_RXI0(vect = VECT(SCI0,RXI0))
void SCI0_RXI0(void)
{
	//rx_buff.enqueue(SCI0.RDR);
	if(rx_wait_t != NULL){
		kernel_resume_thread(rx_wait_t);
		rx_wait_t =NULL;
	}
}

#pragma interrupt SCI0_ERI0(vect = VECT(SCI0,ERI0))
void SCI0_ERI0(void)
{
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