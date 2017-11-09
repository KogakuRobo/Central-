
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "localization.hpp"
#include "MotorClass.hpp"
#include "RotaryClass.hpp"
#include "can_bus_setting.hpp"
#include "MotorSystem_Control.hpp"

class Robot{
	Localization *loca;
	MotorSystem* motora;
	MotorSystem* motorb;
	MotorSystem* motorc;
	MotorSystem* motord;
public:
	Robot(Localization *_l,
	MotorSystem* _motora,
	MotorSystem* _motorb,
	MotorSystem* _motorc,
	MotorSystem* _motord
	) : loca(_l), motora(_motora), motorb(_motorb), motorc(_motorc), motord(_motord)
	{
	}
	
	void Safe(void){
		motora->SetVelocity(100);
		motorb->SetVelocity(100);
		motorc->SetVelocity(100);
		motord->SetVelocity(100);
	}
};

class adj_Localization :public Localization{
public:
	float GetX(void){
		return -1 * Get_d().X * 1000 /*- 244 * sin(Get_d().yaw)*/ - 50 * cos(Get_d().yaw) + 50;
	}
	float GetY(void){
		return Get_d().Y*1000 + 50 * sin(Get_d().yaw) /*+ 244 * cos(Get_d().yaw)*/;
	}
	
};

void main(void)
{
	//SCI0のopenとノンバッファ処理
	FILE *fp = fopen("E1","w");
	if(fp == NULL){
		printf("LKK");
	}
	setvbuf(fp,(char*)fp->_Buf,_IONBF,1);
	
	fprintf(fp,"Program Start\n\r");
	
	PORTA.DDR.BIT.B0 = 1;
	PORTA.DDR.BIT.B1 = 1;
	
	extern long kernel_time;
	
	_rx621_CAN_bus can_bus;
	can_bus_driver(&can_bus);
	
	MotorSystem* motora = new MotorSystem(&can_bus,0x01);
	MotorSystem* motorb = new MotorSystem(&can_bus,0x02);
	MotorSystem* motorc = new MotorSystem(&can_bus,0x04);
	MotorSystem* motord = new MotorSystem(&can_bus,0x08);
	
	Localization loca;
	
	msleep(2000);
	//* 自己位置推定
	fprintf(fp,"Average:,%d\n\r",loca.Get_d().ave);
	fprintf(fp,"Deviation:,%d\n\r",loca.Get_d().devia);
	fprintf(fp,"duty,speed\n\r");
	
	Robot robo(&loca,motora,motorb,motorc,motord);
	//*/
	
	/*
	servo_d servo;
	//*/
	for(float i = 0.0;;i = i + 1.0){
		static float duty = 0;
		robo.Safe();
	msleep(500);
	}
	while(1);
}

extern "C"{
void user_abort(void){
}
}