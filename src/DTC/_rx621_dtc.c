#include"_rx621_dtc.h"

#pragma pack

#pragma address DTC_VectorTable=0x00000000
void *DTC_VectorTable[256];

//変更する場合にはセクション設定も変更が必要です。
#define TABLE_AREA_LENGTH 256
#pragma address dtc_table=0x00000400
volatile DTC_TABLE dtc_table[TABLE_AREA_LENGTH];

#pragma packoption

unsigned char table_State[32] = {0};

int SerchTableState(unsigned char num)				//DTCが使用されているかサーチ
{
	unsigned char e = num % 8;
	unsigned char s = (num - e) / 8;
	return (table_State[s] >> e) & 0x01;
}

void WriteTableState(unsigned char num,unsigned char d)		//DTCステータスの書き換え
{
	unsigned char e = num % 8;
	unsigned char s = (num - e) / 8;
	
	table_State[s] = table_State[s]  & ~(0x01 << e);
	table_State[s] = table_State[s]  | ((d & 0x01) << e);
}

void DTC_init(void)
{
	MSTP(DTC) = 0;				//DTCモジュール起動
	DTC.DTCVBR = DTC_VectorTable;		//ベクタテーブル配置
	DTC.DTCADMOD.BIT.SHORT = 0;		//フルアドレスモード
	
	DTC.DTCST.BIT.DTCST = 1;		//DTC駆動
}

volatile DTC_TABLE *DTC_CreateVect(unsigned int size)
{
	static unsigned char num = 0;
	for(unsigned char i = num + 1;i != num;i++)		//使われていないテーブルを検索
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

//volatile int DTC_ReleaseVect(

int DTC_SetTable(unsigned int vect,volatile DTC_TABLE* table){
	DTC_VectorTable[vect] = (void*)table;		//テーブルにを登録
	return 0;
}

int DTC_IsACT(unsigned char vect)
{
	return (DTC.DTCSTS.BIT.ACT == 1 ) && ( DTC.DTCSTS.BIT.VECN == vect );
}
