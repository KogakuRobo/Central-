
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "localization.hpp"
#include "MotorClass.hpp"
#include "RotaryClass.hpp"

class adj_Localization :public Localization{
public:
	float GetX(void){
		return Get_d().X*1000 - 244 * sin(Get_d().yaw) + 56 * cos(Get_d().yaw);
	}
	float GetY(void){
		return Get_d().Y*1000 + 56 * sin(Get_d().yaw) + 244 * cos(Get_d().yaw);
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
	
	adj_Localization loca;
	
	msleep(2000);
	
	fprintf(fp,"Average:,%d\n\r",loca.Get_d().ave);
	fprintf(fp,"Deviation:,%d\n\r",loca.Get_d().devia);
	fprintf(fp,"duty,speed\n\r");
	
	Rotary rotaryc("ROTARY_D");
	Motor motora("MOTOR_A");
	
	for(float i = 0.0;;i = i + 1.0){
		float duty = 0;
		//msleep(10);
		//fprintf(fp,"%d,%f\n\r",d.time,d.yaw);
		/*fprintf(fp,"%d,%f,%f,%f,%f,%f\n\r",kernel_time,d.X*1000,d.Y*1000,d.yaw,
			d.X*1000 - 244 * sin(d.yaw) + 56 * cos(d.yaw),
			d.Y*1000 + 56 * sin(d.yaw) + 244 * cos(d.yaw)
		);*/
		duty = 99.0*sin(i / 20);
		motora.SetDuty(duty);
		msleep(1000);
		int befor = rotaryc.GetCount();
		msleep(500);
		int after = rotaryc.GetCount();
		
		fprintf(fp,"%f,%d\n\r",duty,after - befor);

	}
	while(1);
}

extern "C"{
void user_abort(void){
}
}