
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "MotorClass.hpp"
#include "RotaryClass.hpp"
#include "can_bus_setting.hpp"

#include "Robot.hpp"

#include "HAL_Robot.h"
#include "my_position.h"
#include "target_point.h"
#include "speed_control.h"

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
	my_position M_POSI(&loca);
	target_point T_POINT;
	speed_control s_con(&T_POINT,&M_POSI);
	HAL_Robot robo(&loca,&motora,&motorb,&motorc,&motord,&M_POSI,&T_POINT,&s_con);
	//*/
	//robo.Begin();
	/*
	servo_d servo;
	//*/
	//fprintf(fout,"ProgramStart\n\r");
	//static cmt2_timer timer2;
	//timer2.set_timer(2500,CT_PRIORITY_MAX,(void *(*)(thread_t*,void*))robo,NULL);
	robo.Begin();
	while(1){
		//printf("%f,%f,%f\n",M_POSI.m_posi_x_give(),M_POSI.m_posi_y_give(), M_POSI.m_posi_angle_give());
		//printf("%f,",i);
		//printf("%f,",t*0.01);
		//printf("%f,%f,%f,%f\n",s_con.output_F_get(1,0),s_con.output_F_get(2,0),s_con.output_F_get(3,0),s_con.output_F_get(4,0));
		//printf("%f,%f,%f,%f\n",s_con.output_B_get(1),s_con.output_B_get(2),s_con.output_B_get(3),s_con.output_B_get(4));
		//printf("%f,%f\n",s_con.output_B_get(1),loca.GetY());
		//printf("%f,%f,%f\n",loca.GetX(),loca.GetY(),loca.GetYaw());
		//printf("%f\n",s_con.output_F_get(1)+s_con.output_B_get(1));
		msleep(100);
	}
	/*for(float i = 0.0;;i = i + 1.0){
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
	}*/
	while(1);
}

extern "C"{
void user_abort(void){
}
}