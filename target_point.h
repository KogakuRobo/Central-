#ifndef _TARGET_POINT_H_		//�ڕW���W�p
#define _TARGET_POINT_H_

#include"Bezier_curve.h"


class target_point {
	double t_posi[2];
	//�ڕW�ʒu//
	double i_posi[2];
	double s_point[2];
	double g_posi[2];
	double time;
	//�ŏ��̖ڕW�ʒu//
	double F_i_posi[2];
	double F_s_point[2];
	double F_g_posi[2];
	double F_time;

	//2�̖ڕW�ʒu//
	double S_i_posi[2];
	double S_s_point[2];
	double S_g_posi[2];
	double S_time;

	//3�̖ڕW�ʒu//
	double T_i_posi[2];
	double T_s_point[2];
	double T_g_posi[2];
	double T_time;

	//S���������p�̎���t//
	double Sji_p[4];
	double Sji_time;
	bezier_curve bezi;
	//bezier_curve pbezi;
public:
	target_point(void);
	void t_posi_set(int flog);
	float sji_time(void);
	void t_posi_give(double T_POSI[2]);
	void Sji_get(double S1, double S2, double S3, double S4, double S_t);
	void t_posi_get(double I_POSI_x, double I_POSI_y, double S_POSI_x, double S_POSI_y, double G_POSI_x, double G_POSI_y);
	void t_posi_st_get(double I_POSI_x, double I_POSI_y, double G_POSI_x, double G_POSI_y);



};
#endif
