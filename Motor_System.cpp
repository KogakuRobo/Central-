#include <math.h>
#include "Define.hpp"
#include "ENUM.hpp"
#include "Motor_System.hpp"
#include "Class.hpp"
#include "Class_PID.hpp"

extern Flag_Class f_s_c;
extern Velocity_Class vel[4];		//���x�ݒ�Ɋւ���N���X
extern Bezier_Class bez[4];
extern pid_class pid_X;			//�@�̂�X����PID����Ɋւ���N���X
extern pid_class pid_Y;			//�@�̂�Y����PID����Ɋւ���N���X
extern Run_No_Class r_n;
extern Flag_Class f_Run_Permission;	//running_flag ���s�����̔��f
extern My_Position_Class m_p;		//���Ȉʒu�N���X

//���x�o�̓R���g���[��
void velocity_contorole(void){
	double speed;
	//����
	if (f_s_c.R_flag() == 0) {speed = S_change(vel[r_n.R_run_no()].R_accel_time());}
	//����
	else if (f_s_c.R_flag() == 1){speed = S_change(vel[r_n.R_run_no()].R_arrival_time() - 2 * vel[r_n.R_run_no()].R_accel_time());}
	//����
	else if (f_s_c.R_flag() == 2){speed = S_change(vel[r_n.R_run_no()].R_accel_time());}
	//����
	else {
		vel[r_n.R_run_no()].Reset_distance();		//�ړI�ɓ���
//		m_p.W_Position(0,X);				//X���W������
//		m_p.W_Position(0,radian);			//�X��������
		f_Run_Permission.flag_up();			//���s�I���t���O���グ��
		f_s_c.flag_Reset();
		pid_X.pid_reset();				//�ϕ��l���Z�b�g
		vel[r_n.R_run_no()].W_ideal_velocity(0);
	}
	
	vel[r_n.R_run_no()].W_calculated_distance(speed*(1/cmt0));	//���_��̋����̏�������
//	vel[r_n.R_run_no()].W_ideal_vel(speed/(gear*L_wheel_radius));//�t�B�[�h�t�H���[�h�H
	
}

double S_change(double achievement_time) {
	static double a = 0;				//�����x
	float processing_time = 1 / cmt1;		//�������� CMT0
	static int processing_count = 0;		//������
	static double next_speed;			//�w�����x

	//��������x���Z�o
	if (processing_count == 0){a = 2 / achievement_time;}//�K�v�����x�̌v�Z
	//�������O��
	if (processing_count *processing_time < achievement_time / 2) {					
		next_speed = a * processing_time* processing_count * processing_time* processing_count / achievement_time;
		processing_count++;
	}
	//�������㔼
	else if (processing_count *processing_time >= achievement_time / 2 && processing_count *processing_time <= achievement_time - 2 * processing_time) {
		next_speed = 1 - a * (achievement_time - processing_time*processing_count)*(achievement_time - processing_time*processing_count) / achievement_time;
		processing_count++;
	}
	//�����I��
	else {			
		processing_count = 0;
		f_s_c.flag_count_up();
	}
	return next_speed;
}