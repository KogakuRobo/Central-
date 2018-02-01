#include <stdio.h>
#include <stdlib.h>
#include <iodefine.h>
#include "System.hpp"
#include "ENUM.hpp"
#include "Define.hpp"

#ifdef __cplusplus
extern "C" {
void abort(void);
void rotary1_overflow();
void rotary1_underflow();
void rotary2_overflow();
void rotary2_underflow();
void rotary3_overflow();
void rotary3_underflow();
void rotary4_overflow();
void rotary4_underflow();
}
#endif

int g_check[4] = {0, 0, 0, 0};
int g_rotary_count[4] = {0, 0, 0, 0};

void CMT0_init(void){	//割り込み勉強用関数(初期設定)
	CMT0.CMCR.BIT.CKS = 2;
	CMT0.CMCR.BIT.CMIE = 1;
	CMT0.CMCOR = 3750-1;
	IPR(CMT0,CMI0) = 14;
	IEN(CMT0,CMI0) = 1;
	CMT.CMSTR0.BIT.STR0 = 1;
	
	//割り込み周期：100Hz
}

void CMT1_init(void){	//割り込み勉強用関数(初期設定)
	CMT1.CMCR.BIT.CKS = 2;
	CMT1.CMCR.BIT.CMIE = 1;
	CMT1.CMCOR = 3750-1;
	IPR(CMT1,CMI1) = 14;
	IEN(CMT1,CMI1) = 1;
	CMT.CMSTR0.BIT.STR0 = 1;
	
	//割り込み周期：100Hz
}

void MSTP_init(void){	// モジュールストップ解除
	MSTP(MTU1) = 0;
	MSTP(MTU2) = 0;
	MSTP(MTU4) = 0;
	MSTP(MTU6) = 0;
	MSTP(MTU7) = 0;
	MSTP(MTU8) = 0;
	MSTP(MTU9) = 0;
	MSTP(MTU10) = 0;
	MSTP(CMT0) = 0;
}

void OSC_init(void){	//クロック設定(p245)
	SYSTEM.SCKCR.LONG = 0x00010100;
	while(SYSTEM.SCKCR.LONG != 0x00010100);
	
	SYSTEM.BCKCR.BYTE = 0x01;
	while(SYSTEM.BCKCR.BYTE != 0x01);
}

void GPIO_init(void){	// 出力設定
	PORTA.DDR.BYTE = 0xCD;
//	PORTA.DDR.BYTE = 0x9D;	//旧マイコンボード
	
	PORTB.DDR.BYTE = 0xBB;
	PORT2.DDR.BIT.B4 = 1;
	
	PORTC.ICR.BYTE = 0xff;
//	PORTB.ICR.BYTE = 0x0C

	IOPORT.PFCMTU.BIT.TCLKS = 1;
	IOPORT.PFDMTU.BIT.TCLKS = 0;
}

void MTU_LIST(void){//関数内で宣言しなおさないように！！

	MTU1_init();	//ロータリー４
	MTU2_init();	//ロータリー３
	MTU4_init();	//モータ２
	MTU6_init();	//モータ１
	MTU7_init();	//ロータリー２
	MTU8_init();	//ロータリー１
	MTU9_init();	//モータ３
	MTU10_init();  //モータ４
}

void MTU1_init(void){	//ロータリー　２
	MTU1.TMDR.BYTE=0x04;
	MTU1.TIER.BYTE=0x30;
	
	MTU1.TCNT=0;
	
	IPR(MTU1,TCIV1)=15;
	IPR(MTU1,TCIU1)=15;
	IEN(MTU1,TCIV1)=1;
	IEN(MTU1,TCIU1)=1;

	MTUA.TSTR.BIT.CST1=1;
}

void MTU2_init(void){	//ロータリー　１
	MTU2.TMDR.BYTE=0x04;
	MTU2.TIER.BYTE=0x30;
	
	MTU2.TCNT=0;
	
	IPR(MTU2,TCIV2)=15;
	IPR(MTU2,TCIU2)=15;
	IEN(MTU2,TCIV2)=1;
	IEN(MTU2,TCIU2)=1;

	MTUA.TSTR.BIT.CST2=1;
}

void MTU4_init(void){	//モータ２
	MTUA.TRWER.BYTE = 0x01;
	MTU4.TCR.BYTE = 0x20;
	MTU4.TMDR.BYTE = 0x02;
	MTU4.TIORH.BYTE = 0x56;
	
	MTUA.TSTR.BIT.CST4 = 0;
	MTUA.TOER.BYTE = 0xC2;
	
	MTU4.TGRA = 2400-1;
	MTU4.TGRB = 1600;
	MTU4.TCNT = 0;
	
	MTUA.TSTR.BIT.CST4=1;
}
	
void MTU6_init(void){	// モータ１
	MTU6.TCR.BYTE = 0x20;
	MTU6.TMDR.BYTE = 0x02;
	MTU6.TIORH.BYTE = 0x56;

	MTU6.TGRA = 2400-1;
	MTU6.TGRB = 1600;
	
	MTU6.TCNT = 0;
	
	MTUB.TSTR.BIT.CST0=1;

}

void MTU7_init(void){	//ロータリー４
	MTU7.TMDR.BYTE=0x04;
	MTU7.TIER.BYTE=0x30;
	
	MTU7.TCNT=0;
	
	IPR(MTU7,TCIV7)=15;
	IPR(MTU7,TCIU7)=15;
	IEN(MTU7,TCIV7)=1;
	IEN(MTU7,TCIU7)=1;

	MTUB.TSTR.BIT.CST1=1;
}

void MTU8_init(void){	//ロータリー３
	MTU8.TMDR.BYTE=0x04;
	MTU8.TIER.BYTE=0x30;
	
	MTU8.TCNT=0;
	
	IPR(MTU8,TCIV8)=15;
	IPR(MTU8,TCIU8)=15;
	IEN(MTU8,TCIV8)=1;
	IEN(MTU8,TCIU8)=1;

	MTUB.TSTR.BIT.CST2=1;
}

void MTU9_init(void){	// モータ3
	MTUB.TRWER.BYTE = 0x01;
	MTU9.TCR.BYTE = 0x20;
	MTU9.TMDR.BYTE = 0x02;
	MTU9.TIORH.BYTE = 0x56;
	
	MTUB.TSTR.BIT.CST3 = 0;
	MTUB.TOER.BYTE = 0xC2;

	MTU9.TGRA = 2400-1;
	MTU9.TGRB = 1600;
	MTU9.TCNT = 0;
	
	MTUB.TSTR.BIT.CST3=1;
}

void MTU10_init(void){	// モータ4
	MTUB.TRWER.BYTE = 0x01;
	MTU10.TCR.BYTE = 0x20;
	MTU10.TMDR.BYTE = 0x02;
	MTU10.TIORH.BYTE = 0x56;
	
	MTUB.TSTR.BIT.CST4 = 0;
	MTUB.TOER.BYTE = 0xC2;

	MTU10.TGRA = 2400-1;
	MTU10.TGRB = 1600;
	MTU10.TCNT = 0;
	
	MTUB.TSTR.BIT.CST4=1;

}

int ROTARY(int dir){	//ロータリーエンコーダの制御

	switch (dir)	//サンプルプログラムから引用
	{
		case RO1:
			g_rotary_count[RO1] = (signed int)(g_rotary_count[RO1] & 0xFFFF0000);
			return (signed long)(g_rotary_count[RO1] | (unsigned int)RO_1);
			break;
		case RO2:
			g_rotary_count[RO2] = (signed int)(g_rotary_count[RO2] & 0xFFFF0000);
			return (signed long)(g_rotary_count[RO2] | (unsigned int)RO_2) ;
			break;
		case RO3:
			g_rotary_count[RO3] = (signed int)(g_rotary_count[RO3] & 0xFFFF0000);
			return (signed long)(g_rotary_count[RO3] | (unsigned int)RO_3);
			break;
		case RO4:
			g_rotary_count[RO4] = (signed int)(g_rotary_count[RO4] & 0xFFFF0000);
			return (signed long)(g_rotary_count[RO4] | (unsigned int)RO_4);
			break;
		default:
			return 0;
			break;
	}
}

//オーバーフロー・アンダーフロー用//
void rotary1_overflow()
{
	g_rotary_count[RO1] += 0x00010000;
}

void rotary1_underflow()
{
	g_rotary_count[RO1] -= 0x00010000;
}

void rotary2_overflow()
{
	g_rotary_count[RO2] += 0x00010000;
}

void rotary2_underflow()
{
	g_rotary_count[RO2] -= 0x00010000;
}

void rotary3_overflow()
{
	g_rotary_count[RO3] += 0x00010000;
}

void rotary3_underflow()
{
	g_rotary_count[RO3] -= 0x00010000;
}

void rotary4_overflow()
{
	g_rotary_count[RO4] += 0x00010000;
}

void rotary4_underflow()
{
	g_rotary_count[RO4] -= 0x00010000;
}