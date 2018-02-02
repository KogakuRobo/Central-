#include"_rx621_can_driver.hpp"
#include"iodefine.h"
#include"machine.h"
#include<stddef.h>

_rx_CAN_bus *_rx621_CAN_bus::This;

_rx621_CAN_bus::_rx621_CAN_bus(void)
{
	
	MSTP(CAN0) = 0;
	
	CAN0.CTLR.BIT.SLPM = 0;		//スリープモードへ移行
	while(CAN0.CTLR.BIT.SLPM);	//スリープモードに移行したか確認
	
	CAN0.CTLR.BIT.CANM = CANM_RESET;
	while(CAN0.CTLR.BIT.CANM != CANM_RESET);
	
	CAN0.CTLR.BIT.MBM = 0;		//通常メールボックス
	CAN0.CTLR.BIT.IDFM = 0;
	CAN0.CTLR.BIT.MLM = 0;
	CAN0.CTLR.BIT.TPM = 0;
	CAN0.CTLR.BIT.TSPS = 3;

#if (_PCK_ == 48)
	#if (CAN_BITRATE == 1000)
	CAN0.BCR.BIT.TSEG1 = 0x03;
	CAN0.BCR.BIT.TSEG2 = 0x02;
	CAN0.BCR.BIT.BRP = 5;
	#else
		#error Don't Set CAN_BITRATE
	#endif
#else
	#error Don't Set PCK
#endif

	IPR(CAN0,TXM0) = 13;
	IEN(CAN0,TXM0) = 1;
	IPR(CAN0,RXM0) = 13;
	IEN(CAN0,RXM0) = 1;
	
	PORT3.DDR.BIT.B2 = 1;
	PORT3.DDR.BIT.B3 = 0;
	
	PORT3.ICR.BIT.B3 = 1;
	
	IOPORT.PFJCAN.BIT.CAN0E = 1;
	
	This = this;
}

void _rx621_CAN_bus::SetMode(CAN_Mode mode)
{
	CAN0.CTLR.BIT.CANM = mode;
	while(CAN0.CTLR.BIT.CANM != mode);
}

void _rx621_CAN_bus::SetMask(int num,unsigned short SID,unsigned short EID)
{
	CAN0.MKR[num].BIT.SID = SID;
	CAN0.MKR[num].BIT.EID = EID;
}

#pragma interrupt _rx621_CAN_bus::CAN0_TXM0(vect = VECT(CAN0,TXM0),enable)
void _rx621_CAN_bus::CAN0_TXM0(void)
{
	This->TXM();
}

#pragma interrupt _rx621_CAN_bus::CAN0_RXM0(vect = VECT(CAN0,RXM0),enable)
void _rx621_CAN_bus::CAN0_RXM0(void)
{
	This->RXM();
}

void _rx621_CAN_bus::p_MB_registe_write(int num,CAN_MSG &msg)
{
	CAN0.MB[num].ID.BIT.SID = msg.SID;
	CAN0.MB[num].ID.BIT.EID = msg.EID;
	CAN0.MB[num].ID.BIT.IDE = msg.IDE;
	CAN0.MB[num].ID.BIT.RTR = msg.RTR;
	CAN0.MB[num].DLC.BIT.DLC = msg.DLC;
	for(int j =0;j < msg.DLC;j++)
		CAN0.MB[num].DATA[j] = msg.data[j];
}

void _rx621_CAN_bus::p_MB_registe_read(int num,CAN_MSG &msg)
{
	msg.SID = CAN0.MB[num].ID.BIT.SID;
	msg.EID = CAN0.MB[num].ID.BIT.EID;
	msg.IDE = CAN0.MB[num].ID.BIT.IDE;
	msg.RTR = CAN0.MB[num].ID.BIT.RTR;
	msg.DLC = CAN0.MB[num].DLC.BIT.DLC;
	for(int j =0;j < msg.DLC;j++)
		msg.data[j] = CAN0.MB[num].DATA[j];
}

void _rx621_CAN_bus::p_MIER_registe_write(int num,int e)
{
	CAN0.MIER = (CAN0.MIER & ~(0x01 << num)) | (e << num);
}

void _rx621_CAN_bus::p_MKIVLR_registe_write(int num,int e)
{
	CAN0.MKIVLR = (CAN0.MKIVLR & ~(0x01 << num)) | (e << num);
}

void _rx621_CAN_bus::p_MCTL_registe_write(int num,unsigned char data)
{
	CAN0.MCTL[num].BYTE = data;
}

void _rx621_CAN_bus::p_MCTL_TRMREQ_registe_write(int num,int r)
{
	CAN0.MCTL[num].BIT.TX.TRMREQ = r;
}

void _rx621_CAN_bus::p_MCTL_RECREQ_registe_write(int num,int r)
{
	CAN0.MCTL[num].BIT.RX.RECREQ = r;
}

bool _rx621_CAN_bus::p_MCTL_is_use(int num)
{
	return (CAN0.MCTL[num].BYTE != 0x00);
}

bool _rx621_CAN_bus::p_STR_SDST(void)
{
	return CAN0.STR.BIT.SDST;
}

bool _rx621_CAN_bus::p_STR_NDST(void)
{
	return CAN0.STR.BIT.NDST;
}

bool _rx621_CAN_bus::p_STR_SENDDATA(int num)
{
	return CAN0.MCTL[num].BIT.TX.SENTDATA;
}

bool _rx621_CAN_bus::p_STR_NEWDATA(int num)
{
	return CAN0.MCTL[num].BIT.RX.NEWDATA;
}
