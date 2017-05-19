#ifndef _rx621_dtc_HPP_
#define _rx621_dtc_HPP_

#include"iodefine.h"

#define DTC_FULL_ADDRESS_MODE

#pragma unpack

#ifdef DTC_FULL_ADDRESS_MODE

typedef struct{
	unsigned short _dumy;
	union{
		unsigned char BYTE;
		struct{
			unsigned char dumy:2;
			unsigned char DM:2;
			unsigned char DTS:1;
			unsigned char DISEL:1;
			unsigned char CHNS:1;
			unsigned char CHNE:1;
		}BIT;
	}MRB;
	union{
		unsigned char BYTE;
		struct{
			unsigned char dumy:2;
			unsigned char SM:2;
			unsigned char SZ:2;
			unsigned char MD:2;
		}BIT;
	}MRA;
	
	void *SAR;
	void *DAR;
	unsigned short CRB;
	unsigned short CRA;
}DTC_TABLE;

#elif defined(DTC_SHORT_ADDRESS_MODE)
//未対応
typedef struct{
}DTC_TABLE;

#else
	#error アドレスモードが設定されていません
#endif

#pragma packoption

#ifdef __cplusplus
extern "C"{
#endif

extern void DTC_init(void);
extern volatile DTC_TABLE *DTC_CreateVect(unsigned int);
extern int DTC_SetTable(unsigned int,volatile DTC_TABLE*);

#ifdef __cplusplus
}
#endif

#endif