#include <math.h>
//#include <iodefine.h>
#include "Motion.hpp"
#include "Define.hpp"
#include "UART.h"
#include "ENUM.hpp"
#include "Class_PID.hpp"
#include "Class.hpp"
#include "System.hpp"
#include "Motor_System.hpp"

#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif

extern double t_start[4];
extern Bezier_Class bez[4];
extern Run_No_Class r_n;	//走行経路を決めるクラス
extern Velocity_Class vel[4];
extern Target_Point_Class t_p[4];	//目的座標に関するクラス
extern TIME_Class time;		//時間計測クラス


void Feed_Foward(double t, Robot *robot){
	static double Px[3] = {0,0,0};
	static double Py[3] = {0,0,0};
	double time = (t - t_start[r_n.R_run_no()])/vel[r_n.R_run_no()].R_arrival_time();//0 <= time < 1
	double ideal_vel_x;
	double ideal_vel_y;
	double ideal_p_x;
	double ideal_p_y;
//	double ideal_vel;	//瞬間速度理想値
	
	bez[r_n.R_run_no()].W_time(time + 1 / (vel[r_n.R_run_no()].R_arrival_time() * 100));//ベジエ曲線のt計算（０～１）

	if(bez[r_n.R_run_no()].R_time()<0)	//ｔが０未満のとき
		bez[r_n.R_run_no()].W_time(0);
	else if(bez[r_n.R_run_no()].R_time()>1)	// tが１より大きいとき
		bez[r_n.R_run_no()].W_time(1);
	
	bez[r_n.R_run_no()].W_p();//現在の１つ後の時間
//	printf("time:%lf秒\n", bez[r_n.R_run_no()].R_time());

	Px[0] = Px[1];
	Px[1] = bez[r_n.R_run_no()].R_p(PX);
	Px[2] = Px[1]-Px[0];
	
	Py[0] = Py[1];
	Py[1] = bez[r_n.R_run_no()].R_p(PY);
	Py[2] = Py[1]-Py[0];

//	printf("Px[1]:%lf  Py[1]%lf  \n", Px[1], Py[1]);
	
	ideal_vel_x = Px[2]/(vel[r_n.R_run_no()].R_arrival_time() * 100);
	ideal_vel_y = Py[2]/(vel[r_n.R_run_no()].R_arrival_time() * 100);
	
	robot->SetTergetVelocity(ideal_vel_x, ideal_vel_y, 0);//速度目標値（モタドラに入れる）
	
	ideal_p_x = Px[1];
	ideal_p_y = Py[1];
	robot->SetTergetPoint(ideal_p_x, ideal_p_y, 0);//座標目標値（モタドラに入れる）
//	printf("v_x:%lf  V_y:%lf  \n", ideal_vel_x, ideal_vel_y);
//	printf("%lf,%lf\n", ideal_p_x, ideal_p_y);
	
	
}