#include"MotorSystem_Control.hpp"
#include <stdio.h>Å@
#include<stddef.h>

#pragma pack
typedef union{
	unsigned char c_data[8];
	struct{
		float f;
		unsigned char nc[4];
	}FLOAT;
}DATA_TRANSER;
#pragma packoption

MotorSystem::MotorSystem(CAN_bus *b,unsigned char i)
{
	static int num=0;
	CAN_MSG msg;
	
	bus = b;
	id = i;
	
	begin_finish = false;
	
	msg.Set(0x0000 | (id & 0x0f),0,0,0,0,NULL);
	msg.handle = ReceiveHandle;
	msg.attr = this;
	
	bus->ReceiveSet(msg,num++,0);
	
	velocity = 0;
}

void MotorSystem::SendData(MotorSystem_CMD cmd,unsigned char len,unsigned char data[])
{
	CAN_MSG msg;
	
	msg.Set((cmd << 4) | (id & 0x0f),0,0,0,len,data);
	msg.handle = send_handle;
	msg.attr = NULL;
	
	bus->Send(msg);
}

void MotorSystem::SendRTRData(MotorSystem_CMD cmd)
{
	CAN_MSG msg;
	
	msg.Set((cmd << 4) | (id & 0x0f),0,1,0,0,NULL);
	msg.handle = send_handle;
	msg.attr = NULL;
	
	bus->Send(msg);
}

void MotorSystem::Begin(void)
{
	SendRTRData(BEGIN);
	//printf("%d",begin_finish);
	while(!this->begin_finish);
}

void MotorSystem::SetVelocity(float velocity)
{
	DATA_TRANSER data;
	data.FLOAT.f = velocity;
	SendData(SET_VELOCITY,4,data.c_data);
}

void MotorSystem::GetVelocityRequest(void)
{
	SendRTRData(GET_VELOCITY);
}

void MotorSystem::SetVcc(float vcc)
{
	DATA_TRANSER data;
	data.FLOAT.f = vcc;
	SendData(SET_VCC,4,data.c_data);
}

void MotorSystem::SetCGain_P(float gain)
{
	DATA_TRANSER data;
	data.FLOAT.f = gain;
	SendData(SET_CGAIN_P,4,data.c_data);
}

void MotorSystem::SetCGain_I(float gain)
{
	DATA_TRANSER data;
	data.FLOAT.f = gain;
	SendData(SET_CGAIN_I,4,data.c_data);
}

void MotorSystem::SetCGain_D(float gain)
{
	DATA_TRANSER data;
	data.FLOAT.f = gain;
	SendData(SET_CGAIN_D,4,data.c_data);
}

void MotorSystem::SetVGain_P(float gain)
{
	DATA_TRANSER data;
	data.FLOAT.f = gain;
	SendData(SET_VGAIN_P,4,data.c_data);
}

void MotorSystem::SetVGain_I(float gain)
{
	DATA_TRANSER data;
	data.FLOAT.f = gain;
	SendData(SET_VGAIN_I,4,data.c_data);
}

void MotorSystem::SetVGain_D(float gain)
{
	DATA_TRANSER data;
	data.FLOAT.f = gain;
	SendData(SET_VGAIN_D,4,data.c_data);
}

float MotorSystem::GetVelocity(void)
{
	return velocity;
}

HandleReturn MotorSystem::send_handle(CAN_MSG msg)
{
	return NON_REQUEST;
}

HandleReturn MotorSystem::ReceiveHandle(CAN_MSG msg)
{
	MotorSystem *This = (MotorSystem*)msg.attr;
	DATA_TRANSER trans;
	
	for(int i =0;i < msg.DLC;i++){
		trans.c_data[i] = msg.data[i];
	}
	
	switch(msg.SID >> 4){
	case GET_VELOCITY:
		This->velocity = trans.FLOAT.f;
		break;
	case BEGIN:
		This->begin_finish = true;
		break;
	}
	return RX_RESET;
}
