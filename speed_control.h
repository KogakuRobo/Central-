#ifndef _SPEED_CONTROL_H_		//���x����p
#define _SPEED_CONTROL_H_

#include"my_position.h"
#include"target_point.h"
#include"PID_my.h"
#include"setting.h"


class speed_control {
	//double m_posi[3];	//���Ȉʒu(x,y,r)
	double t_posi[3];	//�ڕW���W(x,y,r)
	char quad;		//���ی����̊m�F
	char wheel;			//�^�C���̔ԍ�
	double ideal_robot_theta;		//���z�̋@�̊p�x
	double MY_POSI[3];
	double BEFORE_TARGET_POSI[2];
	double TARGET_POSI[2];
	double V_max_X;
	double V_max_Y;
	double TARGET_V[2];
	float count;
	//�i�t�B�[�h�t�H���[�h�p�j//
	double speed_F[3];	//x,y,r
	double slope[3];	//���Ȉʒu�ƖڕW�̌X���ƑO��̖ڕW�̌X��
	double angular_velocity_F;		//�ڕW�p���x�@�i�t�B�[�h�t�H���[�h�j
	double angular_velocity_ad;		//�ʒu�����p���x�@(�t�B�[�h�o�b�N�j
	double practical_angular_velocity;	//�����l�̊p���x�@
	double poor[3];		    //�ڕW�l�ƌ��ڕW�l�̍� (x,y,r)
	float theta_F;			//�ڕW�l�ƌ��ڕW�l�̊p�x��
	double theta_now_ta;	//theta_F���������߂̂���
	double theta_be_ta;
	//(�t�B�[�h�o�b�N�p�j//
	double speed_B[3];
	double angular_velocity_B;		//�ʒu�����p���x�@(�t�B�[�h�o�b�N�j
	double poor2[3];		//���ݒn�ƖڕW�l�̍� (x,y,r)
	float theta_B;		//���ݒn�ƖڕW�l�̊p�x��
	float theta[2];		//theta_B���������߂̂���
	char quad_B;
	my_position*my_posi;
	target_point*t_point;
	PID_my pid_x;
	PID_my pid_y;
	PID_my pid_yaw;
public:
	speed_control( my_position*pmy_posi, target_point*pt_point);		//�R���X�g���N�^
	void angular_velocity_F_input(char move_type,double ini_posi_X,double ini_posi_Y,double goal_posi_X,double goal_posi_Y,float time,char flag);	//�^�C���̖ڕW�p���x���� (�t�B�[�h�t�H���[�h) (x,y)
	void angular_velocity_B_input(void);		//�ʒu���꒲���p���x���� (�t�B�[�h�o�b�N) (x,y,r)
	double angular_velocity_output(void);		//�^�C���̊p���x�n��
	double angular_velocity_stop(void);		//���~�b�g�z�����ꍇ
	void Quadrant_F(void);		//�ی��m�F
	void Quadrant_B(void);		//�ی��m�F
	double Angular_Velocity_F(char WHEEL);		//�^�C���̔ԍ��Əی��ɂ��p���x
	double Angular_Velocity_B(char WHEEL);
	void sji_kasoku(float time,double distance_X,double distance_Y,char flag);
};

#endif