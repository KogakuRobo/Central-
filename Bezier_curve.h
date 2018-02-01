#ifndef _Bezier_curve_H_		//�x�W�F�Ȑ�
#define _Bezier_curve_H_

#include "ENUM.h"

class bezier_curve {
	double p_x[4], p_y[4];	//�����ʒu�A����_�A����_�Q�A�ŏI�ڕW�ʒu
	double t;	//���ԁ@0<t<1
	double point[4];//�ڕW�ʒux,y
public:
	bezier_curve(void);		//�R���X�g���N�^
	void root_set_x(double i_posi, double s_point, double g_posi, double time);	//�@�̌o�H�̐ݒ� x���W
	void root_set_y(double i_posi, double s_point, double g_posi, double time);	//�@�̌o�H�̐ݒ� y���W
	void root_set_r(double i_posi, double s_point, double g_posi, double time);	//�@�̌o�H�̐ݒ� yaw�p ����
	float Sji(double(&sji_p)[4], float max_t);	//�����Ȑ��p�@��L��time����
	void root_set(double(&i_posi)[2], double(&s_point)[2], double(&g_posi)[2], double time);	//�@�̌o�H�̐ݒ� 
	void root_give(double POINT[4]);	//�@�̌o�H�̌Ăяo��
	void root_st_set(double(&i_posi)[2], double(&g_posi)[2], double time);
	void root_st_give(double POINT[4]);
};
#endif
