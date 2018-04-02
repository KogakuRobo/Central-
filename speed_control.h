#ifndef SPEED_CONTROL_H
#define SPEED_CONTROL_H

#include"target_point.h"
#include"setting.h"
#include"pid_my.h"
#include"my_position.h"

class speed_control {
	double output;	//�o��
	double output_B;	//�o��
	double theta;	//�ڕW�@�̂̊p�x
	double my_theta;
	char wheel;	//�^�C���̔z�u
	double V[3];	//�@�̂̑��x
	double V_B[4];	//�@�̂̑��x
	double poor[3];
	double TARGET_POSI[3];
	double BEFORE_TARGET_POSI[3];
	double MY_POSI[3];
	double TARGET_V[3];
	double V_max_X;
	double V_max_Y;
	float count;
	float time;
	double BEFORE_MYPOSI;
	target_point*t_posi;
	PID_my pid_x;
	PID_my pid_y;
	PID_my pid_yaw;
	PID_my pid_V;
	my_position*my_posi;
public:
	speed_control(target_point*pt_posi, my_position*pmy_posi);//WHEEL=�^�C���̔z�u�@1=����,2=����,3=�E��,4=�E��
	double output_F_get(int WHEEL);	//�ڕW���W�Ɗp���x(�^�C���ԍ��ƖڕW�̋@�̌X���j
	double output_B_get(int WHEEL_B);	//�ڕW���W�Ɗp���x
	void robot_speed_F_set(char move_type,double ini_posi_X,double ini_posi_Y,double goal_posi_X,double goal_posi_Y,double goal_posi_ANGLE,float op_time,char flag);		//�o�͗^����
	void robot_speed_B_set(void);		//�o�͗^����
	double robot_speed__theta_B(void);
	double robot_speed__X_B(void);
	double robot_speed__Y_B(void);
	void output_F_give(void);	
	void output_B_give(void);
	void sji_kasoku(double distance_X,double distance_Y,char flag);
	double verocity(int flag,double angle);
	void pid_gain_change(char p_flag);
};

#endif