#ifndef _rx_can_driver_HPP_
#define _rx_can_driver_HPP_
#include"CAN.h"

typedef enum{
	CANM_OPERATION = 0,
	CANM_RESET,
	CANM_HALT,
	CANM_RESET_FORCE,
}CAN_Mode;

//内部メッセージバッファ用にデータ部分を除去したCAN_MSGクラス
class can_msg_node{
public:
	HandleReturn (*handle)(CAN_MSG);
	void *attr;
	
	void operator=(CAN_MSG);
};	

class _rx_CAN_bus:public CAN_bus{
protected:
	can_msg_node buff[32];
	
	virtual void p_MB_registe_write(int num,CAN_MSG&)	= 0;
	virtual void p_MB_registe_read(int num,CAN_MSG&)	= 0;
	virtual void p_MIER_registe_write(int num,int)		= 0;
	virtual void p_MKIVLR_registe_write(int num,int)	= 0;
	virtual void p_MCTL_registe_write(int num,unsigned char)= 0;
	virtual void p_MCTL_TRMREQ_registe_write(int num,int)	= 0;
	virtual void p_MCTL_RECREQ_registe_write(int num,int)	= 0;
	virtual bool p_MCTL_is_use(int num)			= 0;
	virtual bool p_STR_NDST(void)				= 0;
	virtual bool p_STR_SDST(void)				= 0;
	virtual bool p_STR_SENDDATA(int num)			= 0;
	virtual bool p_STR_NEWDATA(int num)			= 0;
	
public:
	HandleReturn HandleCall(int num);
	void RXM(void);
	void TXM(void);
	
public:
	int Send(CAN_MSG msg);
	int ReceiveSet(CAN_MSG msg,bool mask_e = 0);
	int ReceiveSet(CAN_MSG msg,unsigned short num,bool mask_e = 0);
};

#endif