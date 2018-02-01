#include <math.h>
#include <stdio.h>
#include "Start_Set.hpp"
#include "Class.hpp"
#include "Define.hpp"
#include "ENUM.hpp"

extern Run_No_Class r_n;
extern My_Position_Class m_p;		//���Ȉʒu�Ɋւ���N���X
extern My_Position_Class t_m_p;		//�ݐώ��Ȉʒu�Ɋւ���N���X
//extern Target_Point_Class t_p[4];	//�ړI���W�Ɋւ���N���X
//extern RC_filter_Class RC_f[2];	//�f�W�^���t�B���^�̃N���X
extern Velocity_Class vel[4];		//���x�ݒ�Ɋւ���N���X
//extern PSD_Class psd;			//PSD�ԊO���Z���T�̎��Ȉʒu����Ɋւ���N���X
//extern Shoot_Parameter C_param[3]; 	//���˕��̃p�����[�^�[�Ɋւ���N���X{XXX,YYY,RRR}
extern Bezier_Class bez[4];
extern Wheel_Class w_1;		//��^�C���Ɋւ���N���X
extern Wheel_Class w_2;		//��^�C���Ɋւ���N���X
extern Wheel_Class w_3;		//��^�C���Ɋւ���N���X
extern Wheel_Class w_4;		//��^�C���Ɋւ���N���X
//extern HardWare g_hw;		//MD�ʐM�p�N���X




//�����ʒu�̓���
void First_Position(void) {

//	m_p.W_Position(0.75, 0, 0);
	m_p.W_Position(0, 0, 0);
	t_m_p.W_Position(0, 0, 0);

}

void Terget_Point_TZ0(void){//�s�y����s�y�֍s���ꍇ
			   //���b�N���Ǝ󂯎���Ăr�y�ɖ߂�K�v���Ȃ��Ɖ���

	//�Y�����crun_number�N���X
	//�����c�ˏo�������
	//�����c
/*	t_p[0].W_point(3.22, -4.5, 0);
	vel[0].W_ratio(0.25);
	vel[0].C_accel_time();
	vel[0].W_coefficient(0);
	vel[0].W_arrival_time(0.75);
	bez[0].W_p_x(0, 0.0, 0.7, 3.22);
	bez[0].W_p_y(0,-3.7,-4.1,-4.5);*/
	
	////�P���r�f�I�p////
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
	
	//TZ1����TZ2
/*	t_p[1].W_point(3.22, -6.5, 0);
	vel[1].W_ratio(0.25);
	vel[1].C_accel_time();
	vel[1].W_coefficient(0);
	vel[1].W_arrival_time(1);
	bez[1].W_p_x( 3.22, 0.5, 0.5,3.22);
	bez[1].W_p_y(-4.5 ,-4.2,-6.8,-6.5);*/
	
	////�P���r�f�I�B�e�p////
	bez[1].W_point(0, 0, 0);
	vel[1].W_ratio(0.25);
	vel[1].C_accel_time();
	vel[1].W_coefficient(0);
	vel[1].W_arrival_time(5);
	bez[1].W_p_x( 0, 0, 0,0);
	bez[1].W_p_y(3.3 ,2.2,1.1,0);
}

void Terget_Point_TZ2(void){
	//TZ2����TZ3
/*	t_p[2].W_point(6.58, -6.5, 0);
	vel[2].W_ratio(0.25);
	vel[2].C_accel_time();
	vel[2].W_coefficient(0);
	vel[2].W_arrival_time(1);
	bez[2].W_p_x(3.22,4.00,5.7,6.48);
	bez[2].W_p_y(-6.5,-6.5,-6.5,-6.5);*/
	
	////�P���r�f�I�B�e�p////
	bez[2].W_point(-2, 0, 0);
	vel[2].W_ratio(0.25);
	vel[2].C_accel_time();
	vel[2].W_coefficient(0);
	vel[2].W_arrival_time(1);
	bez[2].W_p_x(0,-0.66,-1.32,-2);
	bez[2].W_p_y(0,-2.7,-2.7,0);
}

void Terget_Point_TZ3(void){
	//�e�X�g�p
	bez[3].W_point(-2, 3.2, 0);
	vel[3].W_ratio(0.5);
	vel[3].C_accel_time();
	vel[3].W_coefficient(0);
	vel[3].W_arrival_time(10);
	bez[3].W_p_x(-2,-2,-2,-2);
	bez[3].W_p_y(0,1,2.2,3.2);
}