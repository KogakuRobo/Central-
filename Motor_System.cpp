#include <math.h>
#include "Define.hpp"
#include "ENUM.hpp"
#include "Motor_System.hpp"
#include "Class.hpp"
#include "Class_PID.hpp"

extern Flag_Class f_s_c;
extern Velocity_Class vel[4];		//速度設定に関するクラス
extern Bezier_Class bez[4];
extern pid_class pid_X;			//機体のX軸のPID制御に関するクラス
extern pid_class pid_Y;			//機体のY軸のPID制御に関するクラス
extern Run_No_Class r_n;
extern Flag_Class f_Run_Permission;	//running_flag 走行中かの判断
extern My_Position_Class m_p;		//自己位置クラス

//速度出力コントロール
void velocity_contorole(void){
	double speed;
	//加速
	if (f_s_c.R_flag() == 0) {speed = S_change(vel[r_n.R_run_no()].R_accel_time());}
	//等速
	else if (f_s_c.R_flag() == 1){speed = S_change(vel[r_n.R_run_no()].R_arrival_time() - 2 * vel[r_n.R_run_no()].R_accel_time());}
	//減速
	else if (f_s_c.R_flag() == 2){speed = S_change(vel[r_n.R_run_no()].R_accel_time());}
	//到着
	else {
		vel[r_n.R_run_no()].Reset_distance();		//目的に到着
//		m_p.W_Position(0,X);				//X座標初期化
//		m_p.W_Position(0,radian);			//傾き初期化
		f_Run_Permission.flag_up();			//走行終了フラグを上げる
		f_s_c.flag_Reset();
		pid_X.pid_reset();				//積分値リセット
		vel[r_n.R_run_no()].W_ideal_velocity(0);
	}
	
	vel[r_n.R_run_no()].W_calculated_distance(speed*(1/cmt0));	//理論上の距離の書き込み
//	vel[r_n.R_run_no()].W_ideal_vel(speed/(gear*L_wheel_radius));//フィードフォワード？
	
}

double S_change(double achievement_time) {
	static double a = 0;				//加速度
	float processing_time = 1 / cmt1;		//処理周期 CMT0
	static int processing_count = 0;		//処理回数
	static double next_speed;			//指示速度

	//初回加速度を算出
	if (processing_count == 0){a = 2 / achievement_time;}//必要加速度の計算
	//加減速前半
	if (processing_count *processing_time < achievement_time / 2) {					
		next_speed = a * processing_time* processing_count * processing_time* processing_count / achievement_time;
		processing_count++;
	}
	//加減速後半
	else if (processing_count *processing_time >= achievement_time / 2 && processing_count *processing_time <= achievement_time - 2 * processing_time) {
		next_speed = 1 - a * (achievement_time - processing_time*processing_count)*(achievement_time - processing_time*processing_count) / achievement_time;
		processing_count++;
	}
	//加速終了
	else {			
		processing_count = 0;
		f_s_c.flag_count_up();
	}
	return next_speed;
}