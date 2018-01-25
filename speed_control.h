#ifndef _SPEED_CONTROL_H_		//速度制御用
#define _SPEED_CONTROL_H_

#include"my_position.h"
#include"target_point.h"
#include"PID_my.h"
#include"setting.h"


class speed_control {
	//double m_posi[3];	//自己位置(x,y,r)
	double t_posi[3];	//目標座標(x,y,r)
	char quad;		//何象限かの確認
	char wheel;			//タイヤの番号
	double ideal_robot_theta;		//理想の機体角度
	double MY_POSI[3];
	double BEFORE_TARGET_POSI[2];
	double TARGET_POSI[2];
	double V_max_X;
	double V_max_Y;
	double TARGET_V[2];
	float count;
	//（フィードフォワード用）//
	double speed_F[3];	//x,y,r
	double slope[3];	//自己位置と目標の傾きと前回の目標の傾き
	double angular_velocity_F;		//目標角速度　（フィードフォワード）
	double angular_velocity_ad;		//位置調整角速度　(フィードバック）
	double practical_angular_velocity;	//実測値の角速度　
	double poor[3];		    //目標値と元目標値の差 (x,y,r)
	float theta_F;			//目標値と元目標値の角度差
	double theta_now_ta;	//theta_Fをだすためのもの
	double theta_be_ta;
	//(フィードバック用）//
	double speed_B[3];
	double angular_velocity_B;		//位置調整角速度　(フィードバック）
	double poor2[3];		//現在地と目標値の差 (x,y,r)
	float theta_B;		//現在地と目標値の角度差
	float theta[2];		//theta_Bをだすためのもの
	char quad_B;
	my_position*my_posi;
	target_point*t_point;
	PID_my pid_x;
	PID_my pid_y;
	PID_my pid_yaw;
public:
	speed_control( my_position*pmy_posi, target_point*pt_point);		//コンストラクタ
	void angular_velocity_F_input(char move_type,double ini_posi_X,double ini_posi_Y,double goal_posi_X,double goal_posi_Y,float time,char flag);	//タイヤの目標角速度入手 (フィードフォワード) (x,y)
	void angular_velocity_B_input(void);		//位置ずれ調整角速度入手 (フィードバック) (x,y,r)
	double angular_velocity_output(void);		//タイヤの角速度渡す
	double angular_velocity_stop(void);		//リミット越した場合
	void Quadrant_F(void);		//象限確認
	void Quadrant_B(void);		//象限確認
	double Angular_Velocity_F(char WHEEL);		//タイヤの番号と象限による角速度
	double Angular_Velocity_B(char WHEEL);
	void sji_kasoku(float time,double distance_X,double distance_Y,char flag);
};

#endif