#include"SPI/_rx621_RSPI1.hpp"
#include"./DTC/_rx621_dtc.h"
#include"stddef.h"
#include"iodefine.h"
#include"machine.h"

unsigned long RSPI1_Bus::tx_data[TX_RX_BUFFER_SIZE];	//転送中の送信バッファ
unsigned long RSPI1_Bus::rx_data[TX_RX_BUFFER_SIZE];	//転送中の受信バッファ
unsigned char RSPI1_Bus::len;				//転送中の全転送数
volatile DTC_TABLE *RSPI1_Bus::tx_table;
volatile DTC_TABLE *RSPI1_Bus::rx_table;

bool RSPI1_Bus::singleton = false;

RSPI1_Bus::RSPI1_Bus(void)
{
	MSTP(RSPI1) = 0;
}

int RSPI1_Bus::Begin(unsigned int attr)
{
	if(this->singleton != false)return 0;
	
	//使用するSSのアクティブ極性の設定
// 0でLowアクティブ、1でHighアクティブ
	RSPI1.SSLP.BYTE = 0x00;
	
//SPCKのクロック周波数の設定
//SPCMDの設定にも依存するので注意
	this->SetBitrate(0x02);
	
	RSPI1.SPDCR.BIT.SPFC = 0;	//（初期設定の）フレーム数は 1
	RSPI1.SPDCR.BIT.SPRDTD = 0;	//SPDRの読出しは受信バッファに設定
	RSPI1.SPDCR.BIT.SLSEL = 0;	//面倒なのですべてRSPIで使用できるように設定
	RSPI1.SPDCR.BIT.SPLW = 1;	//SPDRは32bitアクセス
	
	RSPI1.SPCR2.BIT.SPPE = 0;	//パリティは付加しない
	
	RSPI1.SPCMD0.BIT.CPHA = 1;	//クロックの極性は一般的な奴
	RSPI1.SPCMD0.BIT.CPOL = 1;	//
	RSPI1.SPCMD0.BIT.BRDV = 0;	//
	RSPI1.SPCMD0.BIT.SSLA = 0;
	RSPI1.SPCMD0.BIT.SSLKP = 0;
	RSPI1.SPCMD0.BIT.SPB = 0x0f;
	RSPI1.SPCMD0.BIT.LSBF = 0;
	
	PORTE.DDR.BYTE = 0x77;
	PORTE.ICR.BIT.B7= 1;
	PORTE.PCR.BYTE |= 0xE0;
	IOPORT.PFHSPI.BYTE = 0xff;
	
	RSPI1.SPCR.BIT.SPMS = 0;
	RSPI1.SPCR.BIT.TXMD = 0;
	RSPI1.SPCR.BIT.MSTR = 1;
#ifdef RSPI_DTC_UNUSE
	RSPI1.SPCR.BIT.SPE = 1;
#endif
	char dumy = RSPI1.SPCR.BYTE;
	
	this->singleton = true;
	
	tx_table = DTC_CreateVect(VECT(RSPI1,SPTI1));
	rx_table = DTC_CreateVect(VECT(RSPI1,SPRI1));
	
	return 0;
}

int RSPI1_Bus::SetBitrate(unsigned char spr)
{
	RSPI1.SPBR = spr;
	return 0;
}

#ifdef RSPI_DTC_UNUSE
unsigned int RSPI1_Bus::DataRW(unsigned int num,unsigned char* tx,unsigned char *rx,unsigned short len)
{
	if(len == 0)return 0;
	
	RSPI1.SPCR.BIT.SPE = 1;
	
	if(len == 1){		//8bit 転送
		RSPI1.SPCMD0.BIT.SPB = 0x07;
		if(tx != NULL)RSPI1.SPDR.LONG = tx[0];
		while(!RSPI1.SPSR.BIT.SPTEF);
		if(rx != NULL)rx[0] = RSPI1.SPDR.LONG;
	}else if(len == 2){		//16bit転送
		RSPI1.SPCMD0.BIT.SPB = 0x0F;
		if(tx != NULL)RSPI1.SPDR.LONG = (tx[0] << 8) | tx[1];
		while(!RSPI1.SPSR.BIT.SPTEF);
		if(rx != NULL){
			rx[0] = (RSPI1.SPDR.LONG >> 8);
			rx[1] = (RSPI1.SPDR.LONG);
		}
	}else if(len == 3){		//24bit転送
		RSPI1.SPCMD0.BIT.SPB = 0x01;
		if(tx != NULL)RSPI1.SPDR.LONG = (tx[0] << 16) | (tx[1] << 8) | tx[2];
		while(!RSPI1.SPSR.BIT.SPTEF);
		if(rx != NULL){
			rx[0] = (RSPI1.SPDR.LONG >> 16);
			rx[1] = (RSPI1.SPDR.LONG >> 8);
			rx[2] = (RSPI1.SPDR.LONG);
		}
	}else if(len == 4){		//32bit転送
		RSPI1.SPCMD0.BIT.SPB = 0x02;
		if(tx != NULL)RSPI1.SPDR.LONG = (tx[0] << 24) | (tx[1] << 16) | (tx[2] << 8) | tx[3];
		while(!RSPI1.SPSR.BIT.SPTEF);
		if(rx != NULL){
			rx[0] = (RSPI1.SPDR.LONG >> 24);
			rx[1] = (RSPI1.SPDR.LONG >> 16);
			rx[2] = (RSPI1.SPDR.LONG >> 8);
			rx[3] = (RSPI1.SPDR.LONG);
		}
	}
		
	return 0;
}
#elif defined(RSPI_DTC_USE)
unsigned int RSPI1_Bus::DataRW(unsigned int num,unsigned char* tx,unsigned char *rx,unsigned short _len)
{
	if(_len < 1)return 1;				//転送長が0byte以下はエラー
	this->len = _len;				//転送長取得
	
	for(int i =0;i < len;i++){
		this->tx_data[i] = tx[i];		//32bitバッファにコピー（SPDRの都合）
	}
	
	DTC.DTCCR.BIT.RRS = 0;
	
	tx_table->MRA.BIT.MD = 0x00;			//ノーマル転送
	tx_table->MRA.BIT.SZ = 0x02;			//ロングワードサイズ
	tx_table->MRA.BIT.SM = 0x02;			//転送元をインクリメント
	tx_table->SAR = (void *)&this->tx_data[0];	//転送元をバッファに設定
	
	tx_table->MRB.BIT.CHNE = 0;			//チェーン転送禁止
	tx_table->MRB.BIT.CHNS = 0;			//意味なし
	tx_table->MRB.BIT.DISEL = 0;			//全データ転送後割り込み
	tx_table->MRB.BIT.DTS = 0;			//意味なし
	tx_table->MRB.BIT.DM = 0x00;			//アドレス固定
	tx_table->DAR = (void *)&RSPI1.SPDR.LONG;	//転送先にSPDRを設定
	
	tx_table->CRA = this->len;
	tx_table->CRB = 0xFFFF;
	
	rx_table->MRA.BIT.MD = 0x00;		//ノーマル転送
	rx_table->MRA.BIT.SZ = 0x02;		//ロングワードサイズ
	rx_table->MRA.BIT.SM = 0x00;		//固定
	rx_table->SAR = (void *)&RSPI1.SPDR.LONG;	//転送元をSPDRに設定
	
	rx_table->MRB.BIT.CHNE = 0;		//チェーン転送禁止
	rx_table->MRB.BIT.CHNS = 0;		//意味なし
	rx_table->MRB.BIT.DISEL = 0;		//全データ転送後割り込み
	rx_table->MRB.BIT.DTS = 0;		//意味なし
	rx_table->MRB.BIT.DM = 0x02;		//アドレス固定
	rx_table->DAR = (void *)this->rx_data;	//転送先にSPDRを設定
	
	rx_table->CRA = this->len;
	rx_table->CRB = 0xFFFF;
	
	DTC.DTCCR.BIT.RRS = 1;
	
	DTC_SetTable(VECT(RSPI1,SPTI1),tx_table);
	DTC_SetTable(VECT(RSPI1,SPRI1),rx_table);
		
	DTCE(RSPI1,SPTI1) = 1;
	DTCE(RSPI1,SPRI1) = 1;
	
	SetBitrate(0x02);
	RSPI1.SPCMD0.BIT.CPHA = 1;		//クロックの極性は一般的な奴
	RSPI1.SPCMD0.BIT.CPOL = 1;		//
	RSPI1.SPCMD0.BIT.BRDV = 0;		//
	RSPI1.SPCMD0.BIT.SSLA = 0;		//使用するSSLは0
	if(len == 1)
		RSPI1.SPCMD0.BIT.SSLKP = 0;	//1byte転送ならばSSLを保持しない
	else
		RSPI1.SPCMD0.BIT.SSLKP = 1;	//2byte以上では、次の転送までSSLを保持
		
	RSPI1.SPCMD0.BIT.SPB = 0x07;		//8bitオーダー転送
	RSPI1.SPCMD0.BIT.LSBF = 0;		//

	IPR(RSPI1,SPTI1) = 15;
	IEN(RSPI1,SPTI1) = 1;
	IPR(RSPI1,SPRI1) = 15;
	IEN(RSPI1,SPRI1) = 1;
	
	RSPI1.SPCR.BIT.SPMS = 0;
	RSPI1.SPCR.BIT.TXMD = 0;
	RSPI1.SPCR.BIT.MSTR = 1;
	RSPI1.SPCR.BIT.SPRIE = 1;
	RSPI1.SPCR.BIT.SPTIE = 1;
	IR(RSPI1,SPTI1) = 0;
	RSPI1.SPCR.BIT.SPE = 1;
	
	char dumy = RSPI1.SPCR.BYTE;
	
	while(rx_table->CRA != 0);		//転送終了待ち
	
	if(rx != NULL){					//受信が必要であれば
		for(int i =0;i < len;i++){		//RXバッファを転送
			rx[i] = this->rx_data[i];
		}
	}
	while(RSPI1.SPCR.BIT.SPE != 0)RSPI1.SPCR.BIT.SPE = 0;
	return 0;
}
#else 
#error DTCの使用が選択されていません
#endif

#pragma interrupt RSPI1_Bus::RSPI1_SPTI1(vect = VECT(RSPI1,SPTI1),enable)
void RSPI1_Bus::RSPI1_SPTI1(void)
{
	RSPI1.SPCMD0.BIT.SSLKP = 0;
}

#pragma interrupt RSPI1_Bus::RSPI1_SPRI1(vect = VECT(RSPI1,SPRI1),enable)
void RSPI1_Bus::RSPI1_SPRI1(void)
{
}
