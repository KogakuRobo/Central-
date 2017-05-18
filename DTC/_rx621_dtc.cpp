#include"_rx621_dtc.hpp"

#pragma pack

#pragma address DTC_VectorTable=0x00000000
void *DTC_VectorTable[256];

#pragma address dtc_table=0x00000400
volatile DTC_TABLE dtc_table[256];

#pragma packoption

unsigned char table_State[32] = {0};

bool SerchTableState(unsigned char num)				//DTC���g�p����Ă��邩�T�[�`
{
	unsigned char e = num % 8;
	unsigned char s = (num - e) / 8;
	return (table_State[s] >> e) & 0x01;
}

void WriteTableState(unsigned char num,unsigned char d)		//DTC�X�e�[�^�X�̏�������
{
	unsigned char e = num % 8;
	unsigned char s = (num - e) / 8;
	
	table_State[s] = table_State[s]  & ~(0x01 << e);
	table_State[s] = table_State[s]  | ((d & 0x01) << e);
}

void DTC_init(void)
{
	MSTP(DTC) = 0;				//DTC���W���[���N��
	DTC.DTCVBR = DTC_VectorTable;		//�x�N�^�e�[�u���z�u
	DTC.DTCADMOD.BIT.SHORT = 0;		//�t���A�h���X���[�h
	
	DTC.DTCST.BIT.DTCST = 1;		//DTC�쓮
}

volatile DTC_TABLE *DTC_CreateVect(unsigned int size)
{
	static unsigned char num = 0;
	for(unsigned char i = num + 1;i != num;i++)		//�g���Ă��Ȃ��e�[�u��������
	{
		if(SerchTableState(i) == 0)
		{
			WriteTableState(i,1);
			num = i;
			break;
		}
	}
	return &dtc_table[num];
}

int DTC_SetTable(unsigned int vect,volatile DTC_TABLE* table){
	DTC_VectorTable[vect] = (void*)table;		//�e�[�u���ɂ�o�^
	return 0;
}

