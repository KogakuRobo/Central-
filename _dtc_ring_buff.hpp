#ifndef _dtc_ring_buff_HPP_
#define _dtc_ring_buff_HPP_

#include "RingBuff.hpp"
#include "./DTC/_rx621_dtc.h"

//バッファフル検出はできないし、エンプティ検出もバグがあると思うよ。

template<typename type = unsigned char,unsigned char size = 255>
class DtcRingBuff:public RingBuff<type,size>{
	DTC_TABLE *table;
	unsigned char vect;
private:
	bool isAct(void)
	{
		return DTC_IsACT(vect);
	}
public:
	DtcRingBuff(unsigned char _vect):vect(_vect)
	{
		this->table = (DTC_TABLE*)DTC_CreateVect(1);
		this->table->CRA = 0x0000;
	}
	int enable(void* from_addr)
	{
		while(isAct());//動作中
		
		DTC.DTCCR.BIT.RRS = 0;
			
		table->MRA.BIT.MD = 0x01;			//リピート転送
		table->MRA.BIT.SZ = 0x00;			//バイトサイズ
		table->MRA.BIT.SM = 0x00;			//転送元を固定
		table->SAR = from_addr;			//転送元をバッファに設定
			
		table->MRB.BIT.CHNE = 0;			//チェーン転送禁止
		table->MRB.BIT.CHNS = 0;			//意味なし
		table->MRB.BIT.DISEL = 1;			//転送の都度割り込み
		table->MRB.BIT.DTS = 0;			//転送先がリピート領域
		table->MRB.BIT.DM = 0x02;			//インクリメント
		table->DAR = this->buff;	//転送先にSPDRを設定
			
		table->CRA = (size << 8);
		table->CRB = 0xFFFF;
			
		DTC_SetTable(vect,table);
			//DTCE(SCI0,TXI0) = 1;
		return 0;
	}
	//送信バッファとしては非対応
	virtual int enqueue(type t)
	{
		return -1;
	}
	
	virtual bool isEmpty(void)
	{
		this->tall = size - (table->CRA & 0x00ff);
		return tall == head;
	}
	
	virtual bool isFull(void)
	{
		return false;
	}
		
};

#endif