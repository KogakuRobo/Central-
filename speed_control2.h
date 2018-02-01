/*#ifndef SPEED_CONTROL_H
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
	double V[2];	//�@�̂̑��x
	double V_B[3];	//�@�̂̑��x
	double poor[2];
	double TARGET_POSI[2];
	double BEFORE_TARGET_POSI[2];
	double MY_POSI[3];
	target_point*t_posi;
	PID_my pid_x;
	PID_my pid_y;
	PID_my pid_yaw;
	my_position*my_posi;
public:
	speed_control(target_point*pt_posi, my_position*pmy_posi);//WHEEL=�^�C���̔z�u�@1=����,2=����,3=�E��,4=�E��
	double output_F_get(int WHEEL , double theta);	//�ڕW���W�Ɗp���x(�^�C���ԍ��ƖڕW�̋@�̌X���j
	double output_B_get(int WHEEL_B);	//�ڕW���W�Ɗp���x
	void robot_speed_F_set(char move_type);		//�o�͗^����
	void robot_speed_B_set(void);		//�o�͗^����
	double robot_speed__theta_B(void);
	double robot_speed__X_B(void);
	double robot_speed__Y_B(void);
	void output_F_give(void);	
	void output_B_give(void);	
};

#endif*/