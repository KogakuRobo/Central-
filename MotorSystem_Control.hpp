#ifndef _MotorSystem_Control_HPP_
#define _MotorSystem_Control_HPP_

#include"CAN.h"

//コマンドコード　7bit
//[6] - [5] 優先度(00b:通常、01b:予約、10b:デバッグ・調整、11b:監視
//      [4] R/W (R = 1,W = 0)
//[3] - [0] 識別子
typedef enum{
	SET_VELOCITY	= 0x00,
	GET_VELOCITY	= 0x70,
	SET_TORQUE	= 0x01,
	GET_TORQUE	= 0x71,
	SET_DUTY	= 0x02,
	GET_DUTY	= 0x72,
	GET_CURRENT	= 0x73,
	
	SET_MODE	= 0x04,
	GET_MODE	= 0x14,
	GET_STATE	= 0x74,
	
	SET_VCC		= 0x45,
	SET_PPR		= 0x46,
	SET_KT		= 0x47,
	
	SET_VGAIN_P	= 0x48,
	GET_V_P		= 0x78,
	SET_VGAIN_I	= 0x49,
	GET_V_I		= 0x79,
	SET_VGAIN_D	= 0x4a,
	GET_V_D		= 0x7a,
	
	SET_CGAIN_P	= 0x4c,
	GET_C_P		= 0x7c,
	SET_CGAIN_I	= 0x4d,
	GET_C_I		= 0x7d,
	SET_CGAIN_D	= 0x4e,
	GET_C_D		= 0x7e,
	
	BEGIN		= 0x44,
	
}MotorSystem_CMD;//IDの上位7bit分

class MotorSystem{
	unsigned char id;
	CAN_bus *bus;
	
	float velocity;
	
	bool begin;	//begin完了フラグ
	
	void SendData(MotorSystem_CMD cmd,unsigned char len,unsigned char data[]);
	void SendRTRData(MotorSystem_CMD cmd);
public:
	MotorSystem(CAN_bus *b,unsigned char i);
	
	void Begin(void);
	
	void SetVelocity(float velocity);
	void GetVelocityRequest(void);
	void SetVcc(float vcc);
	void SetCGain_P(float gain);
	void SetCGain_I(float gain);
	void SetCGain_D(float gain);
	void SetVGain_P(float gain);
	void SetVGain_I(float gain);
	void SetVGain_D(float gain);
	
	float GetVelocity(void);
	
static HandleReturn send_handle(CAN_MSG);

static HandleReturn ReceiveHandle(CAN_MSG);
};

#endif