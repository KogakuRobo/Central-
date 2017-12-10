
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
	FILE *fout = fopen("SCI0","w");
	if(fout == NULL){
		printf("LKK");
	}
	setvbuf(fout,(char*)fout->_Buf,_IONBF,1);
	
	//SCI0のopenとノンバッファ処理
	FILE *fin = fopen("SCI0","r");
	if(fin == NULL){
		printf("LKK");
	}
	setvbuf(fin,(char*)fin->_Buf,_IONBF,1);
	
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
	//*/
	Localization loca;
	loca.Begin();
	
	msleep(2000);
	//*/
	Robot robo(&loca,&motora,&motorb,&motorc,&motord);
	//*/
	//robo.Begin();
	/*
	servo_d servo;
	//*/
	//fprintf(fout,"ProgramStart\n\r");
	
	for(float i = 0.0;;i = i + 1.0){
		int cmd;
		static int count = 0;
		//robo.Safe();
		//printf("%f\n",duty);
		//if(printf("DD:%d",scanf("%f",&duty)))fflush(stdin);
		//cmd = fgetc(fin);
		fscanf(fin,"%c\n",cmd);
		printf("GET:%d[%d]\n",cmd,count);
		switch(cmd){
		case '0':
			fprintf(fout,"%10.4e\n",loca.GetX());
			break;
		case '1':
			fprintf(fout,"%10.4e\n",loca.GetY());
			break;
		case '2':
			fprintf(fout,"%10.4e\n",loca.GetYaw());
			break;
		case '3':
			float x,y,yaw;
			fscanf(fin,"%f\n",&x);
			fscanf(fin,"%f\n",&y);
			fscanf(fin,"%f\n",&yaw);
			printf("x,%f,y,%f,yaw,%f\n",x,y,yaw);
			robo.SetPostionNode(x,y,yaw,0,0,0);
			fprintf(fout,"3\n");
			break;
		case '4':
			float temp[3];
			fscanf(fin,"%f",&temp[0]);
			fgetc(fin);
			fscanf(fin,"%f",&temp[1]);
			fgetc(fin);
			fscanf(fin,"%f",&temp[2]);
			fgetc(fin);
			robo.GetXPID().SetK(temp[0]);
			robo.GetXPID().SetTi(temp[1]);
			robo.GetXPID().SetTd(temp[2]);
			fprintf(fout,"4\n");
			break;
		case '5':
			fscanf(fin,"%f",&temp[0]);
			fgetc(fin);
			fscanf(fin,"%f",&temp[1]);
			fgetc(fin);
			fscanf(fin,"%f",&temp[2]);
			fgetc(fin);
			robo.GetYPID().SetK(temp[0]);
			robo.GetYPID().SetTi(temp[1]);
			robo.GetYPID().SetTd(temp[2]);
			fprintf(fout,"5\n");
			break;
		case '6':
			fscanf(fin,"%f",&temp[0]);
			fgetc(fin);
			fscanf(fin,"%f",&temp[1]);
			fgetc(fin);
			fscanf(fin,"%f",&temp[2]);
			fgetc(fin);
			robo.GetYawPID().SetK(temp[0]);
			robo.GetYawPID().SetTi(temp[1]);
			robo.GetYawPID().SetTd(temp[2]);
			fprintf(fout,"6\n");
			break;
		case '8':
			robo.Stop();
			fprintf(fout,"8\n");
			break;
		case '9':
			robo.Begin();
			fprintf(fout,"9\n");
			break;
		default:
			printf("Un Set command\n\r");
		}
	}
	while(1);
}

extern "C"{
void user_abort(void){
}
}