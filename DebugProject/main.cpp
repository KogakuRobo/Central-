
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "MotorClass.hpp"
#include "RotaryClass.hpp"
#include "can_bus_setting.hpp"

#include "Robot.hpp"
#include "Define.hpp"
#include "Motion.hpp"
#include "Class.hpp"
#include "ENUM.hpp"
#include "Start_Set.hpp"

Localization loca;

double t_start[4];//t=0のとき（ベジェ用）
Flag_Class f_s_c(0);
Flag_Class f_Run_Permission(0);	//走行許可、不許可のフラグ
Flag_Class f_settle(0);
Rotary_Class rot;		//ロータリーの自己位置推定に関するクラス
Run_No_Class r_n;		//走行経路を決めるクラス
My_Position_Class m_p;		//自己位置クラス
My_Position_Class t_m_p;	//目的座標に関するクラス
Velocity_Class vel[4];
Bezier_Class bez[4];		//ベジェ曲線クラス
Bezier_Class bez_ref[4];	//ベジェ理想値
TIME_Class time;		//時間計測クラス
Ref refer;			//目標値に関するクラス（ＭＤに入れる用）
//Robot g_robo;

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

int is_arrive(void/*Localization *local*/){//目的地に着いたかの判定
//	printf("t_p(%lf, %lf)", bez[r_n.R_run_no()].R_point(X), bez[r_n.R_run_no()].R_point(Y));
	if(bez[r_n.R_run_no()].R_point(X)-0.1<loca.GetX() && loca.GetX()<bez[r_n.R_run_no()].R_point(X)+0.1){//目標点についたとき
		if(bez[r_n.R_run_no()].R_point(Y)-0.1<loca.GetY() && loca.GetY()<bez[r_n.R_run_no()].R_point(Y)+0.1)
			return 1;
		else
			return 0;
	}
	else
		return 0;
}

void Motor(Robot *rob){
	enum Run_Phase run_phase;
	run_phase = START;
	double t_0=0;//cmt0用時間計測
//	double t_start[4];//t=0のとき（ベジェ用）
	printf("Motor:START\n");
	while(1){
		switch(run_phase){
		case WAIT://～シャトル受け取り
			if(1/*シャトルを受け取ったとき*/){
				f_Run_Permission.flag_up();
				run_phase = START;
			}
			break;
		case START:
			time.W_HZ(100.0);
			time.Count();
			
			run_phase = AT_RUNNING;
				
			f_Run_Permission.flag_up();	//走行許可
			r_n.W_run_no(0);
			Terget_Point_TZ0();
			t_start[r_n.R_run_no()]=time.R_TIME();//スタート時の時間t
		//	printf("t_p(%lf, %lf)", t_p[r_n.R_run_no()].R_point(X), t_p[r_n.R_run_no()].R_point(Y));
		//	printf("no:%d\n", r_n.R_run_no());
			break;
		
		case AT_RUNNING://自動走行（ＳＺ～ＴＺ）
			Feed_Foward(time.R_TIME(), rob);//時刻tでの速度・座標理想値算出
			if(is_arrive()==1){
				f_Run_Permission.flag_down();
				run_phase = CHANGE;		//走行モード変更フェイズへ
			}
		//	printf("loc: %.3f %.3f\n", loca.GetX(), loca.GetY());
		//	rob->Print_Robot();
			break;
		
		case THROUGHING:
		switch(r_n.R_run_no()){
			case 0:
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			default: break;
		}
			break;
			
		case CHANGE:
		printf("CHANGE now\n");
		//ＴＺ１（２or３）に到着した時
		
		//ＴＺ１から投げたシャトルがリングを通過した時 
		switch(r_n.R_run_no()){
			case 0:	rob->Motor_STOP();
				msleep(2000);
				run_phase = AT_RUNNING;
				r_n.W_run_no(1);
				Terget_Point_TZ1();
				t_start[r_n.R_run_no()]=time.R_TIME();//スタート時の時間t
				f_Run_Permission.flag_up();
				printf("Run number 1\n");
				break;
			case 1:	msleep(2000);
				run_phase = AT_RUNNING;
				r_n.W_run_no(3);
				Terget_Point_TZ2();
				t_start[r_n.R_run_no()]=time.R_TIME();//スタート時の時間t
				f_Run_Permission.flag_up();
				printf("Run number 2\n");
				break;
			case 2:	msleep(1000);
				run_phase = AT_RUNNING;
				r_n.W_run_no(3);
				Terget_Point_TZ3();
				t_start[r_n.R_run_no()]=time.R_TIME();//スタート時の時間t
				printf("Run number 3\n");
				break;
			case 3: printf("STOP\n");
				break;
			default:printf("Error\n");
				break;
		}
			break;
		
		case TEST:
			Feed_Foward(time.R_TIME(), rob);//時刻tでの速度・座標理想値算出
			if(is_arrive()==1){
				f_Run_Permission.flag_down();
				run_phase = CHANGE;		//走行モード変更フェイズへ
			}
		//	printf("loc: %.3f %.3f\n", loca.GetX(), loca.GetY());
		//	rob->Print_Robot();
			break;
		default:
			break;
		
		}
		printf("loc: %.3f %.3f\n", loca.GetX(), loca.GetY());
//		printf("time:%lf\n", time.R_TIME());
		time.Count();		   //タイム計測開始（ベジエ用）
		msleep(10);
	}
}



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
	
//	Localization loca;
	loca.Begin();
	
	msleep(2000);
	
	Robot robo(&loca,&motora,&motorb,&motorc,&motord);
	//*/
	robo.Begin();
	/*
	servo_d servo;
	//*/
/*	for(float i = 0.0;;i = i + 1.0){
		static float duty = 0;
		//robo.Safe();
		//printf("%f\n",duty);
		//if(printf("DD:%d",scanf("%f",&duty)))fflush(stdin);
		msleep(10);
	}*/
		Motor(&robo);		   //走行モード変更など
}

extern "C"{
void user_abort(void){
}
}