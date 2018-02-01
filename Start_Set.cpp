#include <math.h>
#include <stdio.h>
#include "Start_Set.hpp"
#include "Class.hpp"
#include "Define.hpp"
#include "ENUM.hpp"

extern Run_No_Class r_n;
extern My_Position_Class m_p;		//自己位置に関するクラス
extern My_Position_Class t_m_p;		//累積自己位置に関するクラス
//extern Target_Point_Class t_p[4];	//目的座標に関するクラス
//extern RC_filter_Class RC_f[2];	//デジタルフィルタのクラス
extern Velocity_Class vel[4];		//速度設定に関するクラス
//extern PSD_Class psd;			//PSD赤外線センサの自己位置推定に関するクラス
//extern Shoot_Parameter C_param[3]; 	//投射部のパラメーターに関するクラス{XXX,YYY,RRR}
extern Bezier_Class bez[4];
extern Wheel_Class w_1;		//主タイヤに関するクラス
extern Wheel_Class w_2;		//主タイヤに関するクラス
extern Wheel_Class w_3;		//主タイヤに関するクラス
extern Wheel_Class w_4;		//主タイヤに関するクラス
//extern HardWare g_hw;		//MD通信用クラス




//初期位置の入力
void First_Position(void) {

//	m_p.W_Position(0.75, 0, 0);
	m_p.W_Position(0, 0, 0);
	t_m_p.W_Position(0, 0, 0);

}

void Terget_Point_TZ0(void){//ＴＺからＴＺへ行く場合
			   //ラックごと受け取ってＳＺに戻る必要がないと仮定

	//添え字…run_numberクラス
	//ｘ軸…射出する方向
	//ｙ軸…
/*	t_p[0].W_point(3.22, -4.5, 0);
	vel[0].W_ratio(0.25);
	vel[0].C_accel_time();
	vel[0].W_coefficient(0);
	vel[0].W_arrival_time(0.75);
	bez[0].W_p_x(0, 0.0, 0.7, 3.22);
	bez[0].W_p_y(0,-3.7,-4.1,-4.5);*/
	
	////１次ビデオ用////
	bez[0].W_point(0, 3.3, 0);
//	printf("t_p(%lf, %lf)\n", t_p[r_n.R_run_no()].R_point(X), t_p[r_n.R_run_no()].R_point(Y));
	vel[0].W_ratio(0.25);
	vel[0].C_accel_time();
	vel[0].W_coefficient(0);
	vel[0].W_arrival_time(5);
	bez[0].W_p_x(0,0,0,0);
	bez[0].W_p_y(0,1.1,2.2,3.3);
//	printf("p_x:\n", t_p[r_n.R_run_no()].R_point());
}

void Terget_Point_TZ1(void){
	
	//TZ1からTZ2
/*	t_p[1].W_point(3.22, -6.5, 0);
	vel[1].W_ratio(0.25);
	vel[1].C_accel_time();
	vel[1].W_coefficient(0);
	vel[1].W_arrival_time(1);
	bez[1].W_p_x( 3.22, 0.5, 0.5,3.22);
	bez[1].W_p_y(-4.5 ,-4.2,-6.8,-6.5);*/
	
	////１次ビデオ撮影用////
	bez[1].W_point(0, 0, 0);
	vel[1].W_ratio(0.25);
	vel[1].C_accel_time();
	vel[1].W_coefficient(0);
	vel[1].W_arrival_time(5);
	bez[1].W_p_x( 0, 0, 0,0);
	bez[1].W_p_y(3.3 ,2.2,1.1,0);
}

void Terget_Point_TZ2(void){
	//TZ2からTZ3
/*	t_p[2].W_point(6.58, -6.5, 0);
	vel[2].W_ratio(0.25);
	vel[2].C_accel_time();
	vel[2].W_coefficient(0);
	vel[2].W_arrival_time(1);
	bez[2].W_p_x(3.22,4.00,5.7,6.48);
	bez[2].W_p_y(-6.5,-6.5,-6.5,-6.5);*/
	
	////１次ビデオ撮影用////
	bez[2].W_point(-2, 0, 0);
	vel[2].W_ratio(0.25);
	vel[2].C_accel_time();
	vel[2].W_coefficient(0);
	vel[2].W_arrival_time(1);
	bez[2].W_p_x(0,-0.66,-1.32,-2);
	bez[2].W_p_y(0,-2.7,-2.7,0);
}

void Terget_Point_TZ3(void){
	//テスト用
	bez[3].W_point(-2, 3.2, 0);
	vel[3].W_ratio(0.5);
	vel[3].C_accel_time();
	vel[3].W_coefficient(0);
	vel[3].W_arrival_time(10);
	bez[3].W_p_x(-2,-2,-2,-2);
	bez[3].W_p_y(0,1,2.2,3.2);
}