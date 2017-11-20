
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "MotorClass.hpp"
#include "RotaryClass.hpp"
#include "can_bus_setting.hpp"

#include "Robot.hpp"

class adj_Localization :public Localization{
	float adj_x;
	float adj_y;
	
public:
	float GetX(void){
		return -1 * this->Localization::GetX() * 1000 /*- 244 * sin(Get_d().yaw)*/ - 50 * cos(this->Localization::GetYaw()) + 50;
	}
	float GetY(void){
		return this->Localization::GetY()*1000 + 50 * sin(this->Localization::GetYaw()) /*+ 244 * cos(Get_d().yaw)*/;
	}
	
	void SetAbjX(float x){
		adj_x = x;
	}
	void SetAdjY(float y){
		adj_y = y;
	}
	
	
};

void main(void)
{
	//SCI0のopenとノンバッファ処理
	FILE *fp = fopen("E1","rw");
	if(fp == NULL){
		printf("LKK");
	}
	setvbuf(fp,(char*)fp->_Buf,_IONBF,1);
	
	printf("Program Start\n\r");
	
	PORTA.DDR.BIT.B0 = 1;
	PORTA.DDR.BIT.B1 = 1;
	
	extern long kernel_time;
	//*/
	_rx621_CAN_bus can_bus;
	can_bus_driver(&can_bus);
	
	MotorSystem motora(&can_bus,0x01);
	MotorSystem motorb(&can_bus,0x02);
	MotorSystem motorc(&can_bus,0x04);
	MotorSystem motord(&can_bus,0x08);
	
	Localization loca;
	loca.Begin();
	
	msleep(2000);
	
	Robot robo(&loca,&motora,&motorb,&motorc,&motord);
	//*/
	robo.Begin();
	/*
	servo_d servo;
	//*/
	for(float i = 0.0;;i = i + 1.0){
		static float duty = 0;
		//robo.Safe();
		//printf("%f\n",duty);
		//if(printf("DD:%d",scanf("%f",&duty)))fflush(stdin);
		msleep(10);
	}
	while(1);
}

extern "C"{
void user_abort(void){
}
}