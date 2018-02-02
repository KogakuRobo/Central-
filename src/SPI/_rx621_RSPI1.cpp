#include"SPI/_rx621_RSPI1.hpp"
#include"./DTC/_rx621_dtc.h"
#include"stddef.h"
#include"iodefine.h"
#include"machine.h"

unsigned long RSPI1_Bus::tx_data[TX_RX_BUFFER_SIZE];	//�]�����̑��M�o�b�t�@
unsigned long RSPI1_Bus::rx_data[TX_RX_BUFFER_SIZE];	//�]�����̎�M�o�b�t�@
unsigned char RSPI1_Bus::len;				//�]�����̑S�]����
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
	
	//�g�p����SS�̃A�N�e�B�u�ɐ��̐ݒ�
// 0��Low�A�N�e�B�u�A1��High�A�N�e�B�u
	RSPI1.SSLP.BYTE = 0x00;
	
//SPCK�̃N���b�N���g���̐ݒ�
//SPCMD�̐ݒ�ɂ��ˑ�����̂Œ���
	this->SetBitrate(0x02);
	
	RSPI1.SPDCR.BIT.SPFC = 0;	//�i�����ݒ�́j�t���[������ 1
	RSPI1.SPDCR.BIT.SPRDTD = 0;	//SPDR�̓Ǐo���͎�M�o�b�t�@�ɐݒ�
	RSPI1.SPDCR.BIT.SLSEL = 0;	//�ʓ|�Ȃ̂ł��ׂ�RSPI�Ŏg�p�ł���悤�ɐݒ�
	RSPI1.SPDCR.BIT.SPLW = 1;	//SPDR��32bit�A�N�Z�X
	
	RSPI1.SPCR2.BIT.SPPE = 0;	//�p���e�B�͕t�����Ȃ�
	
	RSPI1.SPCMD0.BIT.CPHA = 1;	//�N���b�N�̋ɐ��͈�ʓI�ȓz
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
	
	if(len == 1){		//8bit �]��
		RSPI1.SPCMD0.BIT.SPB = 0x07;
		if(tx != NULL)RSPI1.SPDR.LONG = tx[0];
		while(!RSPI1.SPSR.BIT.SPTEF);
		if(rx != NULL)rx[0] = RSPI1.SPDR.LONG;
	}else if(len == 2){		//16bit�]��
		RSPI1.SPCMD0.BIT.SPB = 0x0F;
		if(tx != NULL)RSPI1.SPDR.LONG = (tx[0] << 8) | tx[1];
		while(!RSPI1.SPSR.BIT.SPTEF);
		if(rx != NULL){
			rx[0] = (RSPI1.SPDR.LONG >> 8);
			rx[1] = (RSPI1.SPDR.LONG);
		}
	}else if(len == 3){		//24bit�]��
		RSPI1.SPCMD0.BIT.SPB = 0x01;
		if(tx != NULL)RSPI1.SPDR.LONG = (tx[0] << 16) | (tx[1] << 8) | tx[2];
		while(!RSPI1.SPSR.BIT.SPTEF);
		if(rx != NULL){
			rx[0] = (RSPI1.SPDR.LONG >> 16);
			rx[1] = (RSPI1.SPDR.LONG >> 8);
			rx[2] = (RSPI1.SPDR.LONG);
		}
	}else if(len == 4){		//32bit�]��
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
	if(_len < 1)return 1;				//�]������0byte�ȉ��̓G���[
	this->len = _len;				//�]�����擾
	
	for(int i =0;i < len;i++){
		this->tx_data[i] = tx[i];		//32bit�o�b�t�@�ɃR�s�[�iSPDR�̓s���j
	}
	
	DTC.DTCCR.BIT.RRS = 0;
	
	tx_table->MRA.BIT.MD = 0x00;			//�m�[�}���]��
	tx_table->MRA.BIT.SZ = 0x02;			//�����O���[�h�T�C�Y
	tx_table->MRA.BIT.SM = 0x02;			//�]�������C���N�������g
	tx_table->SAR = (void *)&this->tx_data[0];	//�]�������o�b�t�@�ɐݒ�
	
	tx_table->MRB.BIT.CHNE = 0;			//�`�F�[���]���֎~
	tx_table->MRB.BIT.CHNS = 0;			//�Ӗ��Ȃ�
	tx_table->MRB.BIT.DISEL = 0;			//�S�f�[�^�]���㊄�荞��
	tx_table->MRB.BIT.DTS = 0;			//�Ӗ��Ȃ�
	tx_table->MRB.BIT.DM = 0x00;			//�A�h���X�Œ�
	tx_table->DAR = (void *)&RSPI1.SPDR.LONG;	//�]�����SPDR��ݒ�
	
	tx_table->CRA = this->len;
	tx_table->CRB = 0xFFFF;
	
	rx_table->MRA.BIT.MD = 0x00;		//�m�[�}���]��
	rx_table->MRA.BIT.SZ = 0x02;		//�����O���[�h�T�C�Y
	rx_table->MRA.BIT.SM = 0x00;		//�Œ�
	rx_table->SAR = (void *)&RSPI1.SPDR.LONG;	//�]������SPDR�ɐݒ�
	
	rx_table->MRB.BIT.CHNE = 0;		//�`�F�[���]���֎~
	rx_table->MRB.BIT.CHNS = 0;		//�Ӗ��Ȃ�
	rx_table->MRB.BIT.DISEL = 0;		//�S�f�[�^�]���㊄�荞��
	rx_table->MRB.BIT.DTS = 0;		//�Ӗ��Ȃ�
	rx_table->MRB.BIT.DM = 0x02;		//�A�h���X�Œ�
	rx_table->DAR = (void *)this->rx_data;	//�]�����SPDR��ݒ�
	
	rx_table->CRA = this->len;
	rx_table->CRB = 0xFFFF;
	
	DTC.DTCCR.BIT.RRS = 1;
	
	DTC_SetTable(VECT(RSPI1,SPTI1),tx_table);
	DTC_SetTable(VECT(RSPI1,SPRI1),rx_table);
		
	DTCE(RSPI1,SPTI1) = 1;
	DTCE(RSPI1,SPRI1) = 1;
	
	SetBitrate(0x02);
	RSPI1.SPCMD0.BIT.CPHA = 1;		//�N���b�N�̋ɐ��͈�ʓI�ȓz
	RSPI1.SPCMD0.BIT.CPOL = 1;		//
	RSPI1.SPCMD0.BIT.BRDV = 0;		//
	RSPI1.SPCMD0.BIT.SSLA = 0;		//�g�p����SSL��0
	if(len == 1)
		RSPI1.SPCMD0.BIT.SSLKP = 0;	//1byte�]���Ȃ��SSL��ێ����Ȃ�
	else
		RSPI1.SPCMD0.BIT.SSLKP = 1;	//2byte�ȏ�ł́A���̓]���܂�SSL��ێ�
		
	RSPI1.SPCMD0.BIT.SPB = 0x07;		//8bit�I�[�_�[�]��
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
	
	while(rx_table->CRA != 0);		//�]���I���҂�
	
	if(rx != NULL){					//��M���K�v�ł����
		for(int i =0;i < len;i++){		//RX�o�b�t�@��]��
			rx[i] = this->rx_data[i];
		}
	}
	while(RSPI1.SPCR.BIT.SPE != 0)RSPI1.SPCR.BIT.SPE = 0;
	return 0;
}
#else 
#error DTC�̎g�p���I������Ă��܂���
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
