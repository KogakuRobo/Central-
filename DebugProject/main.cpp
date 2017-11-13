
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "localization.hpp"
#include "MotorClass.hpp"
#include "RotaryClass.hpp"
#include "can_bus_setting.hpp"
#include "MotorSystem_Control.hpp"
#include "PID.hpp"

class Robot{
	Localization *loca;
	MotorSystem* motora;
	MotorSystem* motorb;
	MotorSystem* motorc;
	MotorSystem* motord;
	
	PID<float> x_pid;
	PID<float> y_pid;
	PID<float> yaw_pid;
	
	float PI;
public:
	Robot(Localization *_l,
	MotorSystem* _motora,
	MotorSystem* _motorb,
	MotorSystem* _motorc,
	MotorSystem* _motord
	) : loca(_l), motora(_motora), motorb(_motorb), motorc(_motorc), motord(_motord),
	x_pid(1000,10000000,0,0.01),
	y_pid(1000,10000000,0,0.01),
	yaw_pid(7000.0,10000000,0,0.01)
	{
		PI = 3.1415926535;
	}
	
	void Safe(void){
		//*/
		float yaw_ref = 0;
		float yaw = loca->GetYaw();
		float terget = yaw_pid.Run(yaw,yaw_ref);
		
		float x_ref = 0;
		float x = loca->GetX();
		float x_terget = x_pid.Run(x,x_ref);
		
		float y_ref = 0;
		float y = loca->GetY();
		float y_terget = y_pid.Run(y,y_ref);
		//*/
		motora->SetVelocity(- x_terget * sin(       PI / 4  - yaw) + y_terget * cos(       PI / 4  - yaw) - terget);
		motorb->SetVelocity(- x_terget * sin(   3 * PI / 4  - yaw) + y_terget * cos(   3 * PI / 4  - yaw) - terget);
		motorc->SetVelocity(- x_terget * sin( - 3 * PI / 4  - yaw) + y_terget * cos( - 3 * PI / 4  - yaw) - terget);
		motord->SetVelocity(- x_terget * sin( - 1 * PI / 4  - yaw) + y_terget * cos( - 1 * PI / 4  - yaw) - terget);
		//*/
		
		//printf("X,%f,Y,%f,yaw,%f\n\r",loca->GetX(),loca->GetY(),loca->GetYaw());
		printf("X,%f,Y,%f,yaw,%f\n\r",x_terget,y_terget,terget);
		
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
	
	MotorSystem motora(&can_bus,0x01);
	MotorSystem motorb(&can_bus,0x02);
	MotorSystem motorc(&can_bus,0x04);
	MotorSystem motord(&can_bus,0x08);
	
	Localization loca;
	
	msleep(2000);
	//* 自己位置推定
	fprintf(fp,"Average:,%d\n\r",loca.Get_d().ave);
	fprintf(fp,"Deviation:,%d\n\r",loca.Get_d().devia);
	fprintf(fp,"duty,speed\n\r");
	
	Robot robo(&loca,&motora,&motorb,&motorc,&motord);
	//*/
	
	/*
	servo_d servo;
	//*/
	for(float i = 0.0;;i = i + 1.0){
		static float duty = 0;
		robo.Safe();
		msleep(10);
	}
	while(1);
}

extern "C"{
void user_abort(void){
}
}