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
extern Run_No_Class r_n;	//���s�o�H�����߂�N���X
extern Velocity_Class vel[4];
extern Target_Point_Class t_p[4];	//�ړI���W�Ɋւ���N���X
extern TIME_Class time;		//���Ԍv���N���X


void Feed_Foward(double t, Robot *robot){
	static double Px[3] = {0,0,0};
	static double Py[3] = {0,0,0};
	double time = (t - t_start[r_n.R_run_no()])/vel[r_n.R_run_no()].R_arrival_time();//0 <= time < 1
	double ideal_vel_x;
	double ideal_vel_y;
	double ideal_p_x;
	double ideal_p_y;
//	double ideal_vel;	//�u�ԑ��x���z�l
	
	bez[r_n.R_run_no()].W_time(time + 1 / (vel[r_n.R_run_no()].R_arrival_time() * 100));//�x�W�G�Ȑ���t�v�Z�i�O�`�P�j

	if(bez[r_n.R_run_no()].R_time()<0)	//�����O�����̂Ƃ�
		bez[r_n.R_run_no()].W_time(0);
	else if(bez[r_n.R_run_no()].R_time()>1)	// t���P���傫���Ƃ�
		bez[r_n.R_run_no()].W_time(1);
	
	bez[r_n.R_run_no()].W_p();//���݂̂P��̎���
//	printf("time:%lf�b\n", bez[r_n.R_run_no()].R_time());

	Px[0] = Px[1];
	Px[1] = bez[r_n.R_run_no()].R_p(PX);
	Px[2] = Px[1]-Px[0];
	
	Py[0] = Py[1];
	Py[1] = bez[r_n.R_run_no()].R_p(PY);
	Py[2] = Py[1]-Py[0];

//	printf("Px[1]:%lf  Py[1]%lf  \n", Px[1], Py[1]);
	
	ideal_vel_x = Px[2]/(vel[r_n.R_run_no()].R_arrival_time() * 100);
	ideal_vel_y = Py[2]/(vel[r_n.R_run_no()].R_arrival_time() * 100);
	
	robot->SetTergetVelocity(ideal_vel_x, ideal_vel_y, 0);//���x�ڕW�l�i���^�h���ɓ����j
	
	ideal_p_x = Px[1];
	ideal_p_y = Py[1];
	robot->SetTergetPoint(ideal_p_x, ideal_p_y, 0);//���W�ڕW�l�i���^�h���ɓ����j
//	printf("v_x:%lf  V_y:%lf  \n", ideal_vel_x, ideal_vel_y);
//	printf("%lf,%lf\n", ideal_p_x, ideal_p_y);
	
	
}