#include"PSCon.hpp"
#include<stddef.h>

PSCon::PSCon(CAN_bus* _bus,unsigned char i)
{
	CAN_MSG msg;
	
	this->bus = _bus;
	id = i;
	
	msg.Set(0x0000 | (id & 0x0f),0,0,0,0,NULL);
	msg.handle = PSCon::ReceiveHandle;
	msg.attr = this;
	
	bus->ReceiveSet(msg,4,0);
	
}

void PSCon::SendRTRCmd(PSCon_CMD cmd)
{
	CAN_MSG msg;
	
	msg.Set((cmd << 4) | (id & 0x0f),0,1,0,0,NULL);
	msg.handle = send_handle;
	msg.attr = NULL;
	
	bus->Send(msg);
}

PSConInfo* PSCon::Get(void)
{
	getInfo_finish = false;
	this->SendRTRCmd(GET_INFO);
	while(!getInfo_finish);
	return new PSConInfo(this->data);
}

HandleReturn PSCon::send_handle(CAN_MSG msg)
{
	return NON_REQUEST;
}

HandleReturn PSCon::ReceiveHandle(CAN_MSG msg)
{
	PSCon* This = (PSCon*)msg.attr;
	switch(msg.SID >> 4){
	case GET_INFO:
		for(int i = 0; i < 8;i++){
			This->data[i] = msg.data[i];
		}
		This->getInfo_finish = true;
		break;
	}
	return RX_RESET;
}

PSConInfo::PSConInfo(unsigned char _data[])
{
	for(int i = 0; i < 8;i++){
		this->data[i] = _data[i];
	}
}