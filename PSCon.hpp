#ifndef _PSCon_HPP_
#define _PSCon_HPP_
#include"CAN.h"

class PSConInfo;//ëOï˚êÈåæ

class PSCon{
	CAN_bus *bus;
	unsigned char id;
	
	typedef enum{
		GET_INFO = 0x00,
	}PSCon_CMD;
	
	void SendRTRCmd(PSCon_CMD);			//=0x03
	volatile __evenaccess bool getInfo_finish;
	
	unsigned char data[8];
	
public:
	PSCon(CAN_bus*,unsigned char);
	
	PSConInfo* Get(void);
	
	static HandleReturn send_handle(CAN_MSG);

	static HandleReturn ReceiveHandle(CAN_MSG);
};

class PSConInfo{
	unsigned char data[8];
public:
	PSConInfo(unsigned char[]);
	bool Circle(void);
	bool Square(void);
	bool Cross(void);
	bool Triangle(void);
	
	bool Up(void);
	bool Down(void);
	bool Left(void);
	bool Right(void);
	
	bool L1(void);
	unsigned char L2(void){return data[3];}
	bool R1(void);
	unsigned char R2(void){return data[2];}
	
	unsigned char LeftX(void){return data[7];}
	unsigned char LeftY(void){return data[6];}
	unsigned char RightX(void){return data[5];}
	unsigned char RightY(void){return data[4];}
};

#endif