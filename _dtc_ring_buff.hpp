#ifndef _dtc_ring_buff_HPP_
#define _dtc_ring_buff_HPP_

#include "RingBuff.hpp"
#include "./DTC/_rx621_dtc.h"

//�o�b�t�@�t�����o�͂ł��Ȃ����A�G���v�e�B���o���o�O������Ǝv����B

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
		while(isAct());//���쒆
		
		DTC.DTCCR.BIT.RRS = 0;
			
		table->MRA.BIT.MD = 0x01;			//���s�[�g�]��
		table->MRA.BIT.SZ = 0x00;			//�o�C�g�T�C�Y
		table->MRA.BIT.SM = 0x00;			//�]�������Œ�
		table->SAR = from_addr;			//�]�������o�b�t�@�ɐݒ�
			
		table->MRB.BIT.CHNE = 0;			//�`�F�[���]���֎~
		table->MRB.BIT.CHNS = 0;			//�Ӗ��Ȃ�
		table->MRB.BIT.DISEL = 1;			//�]���̓s�x���荞��
		table->MRB.BIT.DTS = 0;			//�]���悪���s�[�g�̈�
		table->MRB.BIT.DM = 0x02;			//�C���N�������g
		table->DAR = this->buff;	//�]�����SPDR��ݒ�
			
		table->CRA = (size << 8);
		table->CRB = 0xFFFF;
			
		DTC_SetTable(vect,table);
			//DTCE(SCI0,TXI0) = 1;
		return 0;
	}
	//���M�o�b�t�@�Ƃ��Ă͔�Ή�
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