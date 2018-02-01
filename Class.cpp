#include "Class.hpp"
#include "Define.hpp"
#include "ENUM.hpp"
#include "UART.h"
#include <stdio.h>
#include <stdlib.h>


//****�^�C���E���[�^�̃N���X****//
//�R���X�g���N�^//
Wheel_Class::Wheel_Class(void) {
	M_out = 0;
	M_out_last = 0;
	angular_velocity = 0;
	observation_angular_velocity=0;
}

//��������//
void Wheel_Class::W_Motor_out(int m_out) {
	M_out_last = M_out;
	M_out = m_out;
}
void Wheel_Class::W_Wheel_a_v(double w_a_v) {
	angular_velocity = w_a_v;

}
void Wheel_Class::W_Wheel_o_a_v(double w_o_a_v) {
	observation_angular_velocity = w_o_a_v;

}

//�ǂݏo��//
double Wheel_Class::R_Wheel_a_v(void) {return angular_velocity;}
double Wheel_Class::R_Wheel_o_a_v(void) {return observation_angular_velocity;}
double Wheel_Class::R_Motor_out(void) {return M_out;}
double Wheel_Class::R_Motor_out_last(void) {return M_out_last;}



//****���x����̃N���X****//
double Velocity_Class::calculated_distance;

//�R���X�g���N�^//
Velocity_Class::Velocity_Class(void) {

	arrival_time = 0;
	accel_time = 0;
	ratio = 0;
	MAX_velocity = 0;
	calculated_distance = 0;
	K=0;

}

//���B���Ԃ̏�������
void Velocity_Class::W_arrival_time(double a_t){
	arrival_time = a_t;
}
//���������Ԃ̎Z�o
void Velocity_Class::C_accel_time(void) {
	accel_time = arrival_time/(2+ratio);
}
//�������Ԃ̊����̏�������(�l)
void Velocity_Class::W_ratio(double r) {
	ratio = r;
}
//�ō����x�̎Z�o
void Velocity_Class::C_MAX_velocity(double x) {
	MAX_velocity =  ((2 + ratio)*x) / ((1 + ratio)*arrival_time);
}
//�v�Z��̋����̏������݁i�l�j
void Velocity_Class::W_calculated_distance(double c_d) {
	calculated_distance += c_d;
}
//�v�Z��̋����̏������݁i�l�j
void Velocity_Class::W_New_calculated_distance(double n_c_d) {
	calculated_distance = n_c_d;
}
//�o�̓X�s�[�h�̌W���������݁i�l�j
void Velocity_Class::W_coefficient(double k){
	K=k;
}
//���z���x�̏�������
void Velocity_Class::W_ideal_velocity(double i_v) {
	ideal_velocity = i_v;
}


//���B���Ԃ̓ǂݏo��
double Velocity_Class::R_arrival_time(void) {
	return arrival_time;
}
//���������Ԃ̓ǂݏo��()
double Velocity_Class::R_accel_time(void) {
	return accel_time;
}
//���B���Ԃ̊����̓ǂݏo��
double Velocity_Class::R_ratio(void) {
	return ratio;
}
//�ō����x�̓ǂݏo��
double Velocity_Class::R_MAX_velocity(void) {
	return MAX_velocity;
}
//�v�Z��̋����̓ǂݏo��()
double Velocity_Class::R_calculated_distance(void) {
	return calculated_distance;
}
//�o�̓X�s�[�h�̏C���v�Z(���_��̋���,���H��̋���)
double Velocity_Class::revise_speed(double theory_d, double practice_d) {
	return K*(theory_d - practice_d);
}
//calculated_distance�̃��Z�b�g
void Velocity_Class::Reset_distance(void) {
	calculated_distance = 0;
}
//���z���x�̓ǂݏo��
double Velocity_Class::R_ideal_velocity(void) {
	return ideal_velocity ;
}



//****�ڕW���W�̃N���X****//
//�R���X�g���N�^//
double Target_Point_Class::last_point_X;
double Target_Point_Class::last_point_Y;

Target_Point_Class::Target_Point_Class(void) {

	point[X]=0;
	point[Y]=0;
	point[radian]=0;
	target_distance=0;		//�ڕW�܂ł̋���

}
//�ڕW�_�y�ъp�x�̏�������
void Target_Point_Class::W_point(double x, double y, double r) {

	point[X] = x;
	point[Y] = y;
	point[radian] = r;
}
//�ړI�_�܂ł̋����̏�������
void Target_Point_Class::W_Target_Distance(double t_d) {

	target_distance = t_d;
}

//�I�����_��X���W�̏�������
void Target_Point_Class::W_last_point_X(double l_x){
	
	last_point_X=l_x;
}

//�I�����_��Y���W�̏�������
void Target_Point_Class::W_last_point_Y(double l_y){
	
	last_point_Y=l_y;
}



//�ڕW�_�y�ъp�x�̓ǂݏo��
double Target_Point_Class::R_point(int no) {

	switch (no)
	{
	case X:return point[X]; break;
	case Y:return point[Y]; break;
	case radian:return point[radian];break;
	default: return -1;break;
	}

}
//�ړI�_�܂ł̋����̓ǂݏo��
double Target_Point_Class::R_Target_Distance(void) {

	return target_distance;
}

//�I�����_��X���W�̓ǂݏo��
double Target_Point_Class::R_last_point_X(void){
	
	return last_point_X;
}

//�I�����_��Y���W�̓ǂݏo��
double Target_Point_Class::R_last_point_Y(void){
	
	return last_point_Y;
}



//*******���Ȉʒu����̃N���X*******//
double My_Position_Class::position[4];
double My_Position_Class::position_Rotary[4];
//double My_Position_Class::position_PSD[3];
//double My_Position_Class::position_RGB[3][2];
double My_Position_Class::position_Rotary_0[4];
//�R���X�g���N�^//
My_Position_Class::My_Position_Class(void) {
	int  i;
	for(i=0;i<4;i++){
		position[i]=0;
		position_Rotary[i]=0;
	//	position_PSD[i]=0;
		position_Rotary_0[i]=0;
	//	for(j=0;j<2;j++){position_RGB[i][j]=0;}
	}
}

//���Ȉʒu���W�̏�������
void My_Position_Class::W_Position(double posi,int no) {
	
	switch (no)
	{
	case X:position[X] = posi;break;
	case Y:position[Y] = posi;break;
	case radian:position[radian] = posi;break;
	default://ERROR_STOP 
		break;
	}
}

//���Ȉʒu���W�̏�������
void My_Position_Class::W_Position(double x, double y, double r) {

	position[X] = x;
	position[Y] = y;
	position[radian] = r;

}

//���Ȉʒu���W�̓ǂݏo��
double My_Position_Class::R_Position(int no) {

	switch (no)
	{
	case X:return position[X];
	case Y:return position[Y];
	case radian:return position[radian];
	default://ERROR_STOP
		break;
	}
}
//���Ȉʒu���W�̓ǂݏo���i[X_Rotary,Y_Rotary,radian_Rotary]�j
double My_Position_Class::R_Position_Rotary(int no) {

	switch (no)
	{
	case X:return position_Rotary[X_Rotary];
	case Y:return position_Rotary[Y_Rotary];
	case radian:return position_Rotary[radian_Rotary];
	default://ERROR_STOP 
		break;
	}
}
//���Ȉʒu���W�̓ǂݏo���i[X_PSD,Y_PSD,radian_PSD]�j
/*double My_Position_Class::R_Position_PSD(int no) {

	switch (no)
	{
	case X:ERROR_STOP break;
	case Y:return position_PSD[Y_PSD];
	case radian:return position_PSD[radian_PSD];
	default:ERROR_STOP break;
	}
}
//���Ȉʒu���胊�~�b�g�̓ǂݏo���i[X_RGB or Y_RGB or radian_RGB],[High�@or Low]�j
double My_Position_Class::R_Position_RGB(int no,int hl){
	
	if((no==X_RGB||no==Y_RGB||no==radian_RGB)&&(hl==High||hl==Low)){
		return position_RGB[no][hl];
	}
	else {ERROR_STOP}
}*/
//���Ȉʒu�̓���
void My_Position_Class::position_Fusion(void){
	
	position_Rotary_0[X_Rotary]=position_Rotary[X_Rotary];
	position_Rotary_0[Y_Rotary]=position_Rotary[Y_Rotary];
	position_Rotary_0[radian_Rotary]=position_Rotary[radian_Rotary];
	

	
//////X���W�̓���//////
/*	if(position_Rotary[X_Rotary]>position_RGB[X_RGB][High]){
		if(position_Rotary[X_Rotary]-position_RGB[X_RGB][High]>0.05){	//�e�Z���T�̌X���̍���5cm�ȏ�Ȃ�O��l
			position[X]=position_Rotary[X_Rotary];
		}
		else{
			position[X]=position_RGB[X_RGB][High];
			position_Rotary[X_Rotary]=position_RGB[X_RGB][High];	
		}
	}
	else if(position_Rotary[X_Rotary]<position_RGB[X_RGB][Low]){
		if(position_Rotary[X_Rotary]-position_RGB[X_RGB][Low]<-0.05){	//�e�Z���T�̌X���̍���5cm�ȉ��Ȃ�O��l
			position[X]=position_Rotary[X_Rotary];
		}
		else{
			position[X]=position_RGB[X_RGB][Low];
			position_Rotary[X_Rotary]=position_RGB[X_RGB][Low];
		}
	}
	else {*/
		position[X]=position_Rotary[X_Rotary];
//	}	
//////Y���W�̓���//////	
/*	if(position_Rotary[Y_Rotary]>position_RGB[Y_RGB][High]){
		if(position_Rotary[Y_Rotary]-position_RGB[Y_RGB][High]>0.1){	//�e�Z���T�̌X���̍���1cm�ȏ�Ȃ�O��l
			position[Y]=position_Rotary[Y_Rotary];
		}
		else{
			position[Y]=position_RGB[Y_RGB][High];
			position_Rotary[Y_Rotary]=position_RGB[Y_RGB][High];	
		}
	}
	else if(position_Rotary[Y_Rotary]<position_RGB[Y_RGB][Low]){
		if(position_Rotary[Y_Rotary]-position_RGB[Y_RGB][Low]<-0.1){	//�e�Z���T�̌X���̍���1cm�ȉ��Ȃ�O��l
			position[Y]=position_Rotary[Y_Rotary];
		}
		else{
			position[Y]=position_RGB[Y_RGB][Low];
			position_Rotary[Y_Rotary]=position_RGB[Y_RGB][Low];
		}
	}
	else {*/
		position[Y]=position_Rotary[Y_Rotary];
//	}		
	
//////radian�̓���//////	
/*	if(position_Rotary[radian_Rotary]>position_RGB[radian_RGB][High]){
		if(position_Rotary[radian_Rotary]-position_RGB[radian_RGB][High]>0.087){	//�e�Z���T�̌X���̍���0.5�x�ȏ�Ȃ�O��l
			position[radian]=position_Rotary[radian_Rotary];
		}
		else{
			position[radian]=position_RGB[radian_RGB][High];
			position_Rotary[radian_Rotary]=position_RGB[radian_RGB][High];	
		}
	}
	else if(position_Rotary[radian_Rotary]<position_RGB[radian_RGB][Low]){
		if(position_Rotary[radian_Rotary]-position_RGB[radian_RGB][Low]<-0.087){	//�e�Z���T�̌X���̍���0.5�x�ȉ��Ȃ�O��l
			position[radian]=position_Rotary[radian_Rotary];
		}
		else{
			position[radian]=position_RGB[radian_RGB][Low];
			position_Rotary[radian_Rotary]=position_RGB[radian_RGB][Low];
		}
	}
	else {*/
		position[radian]=position_Rotary[radian_Rotary];
//	}
}

void My_Position_Class::position_pri(void){
		
//	printf("%f",position[radian]);
//	printf("%f;%f;%f",position[Y],position_RGB[Y_RGB][High],position_RGB[Y_RGB][Low]);
	printf("X:%f;  Y:%f;  rad:%f;\n\r ",position[X],position[Y],position[radian]);
//	printf("%f;%f;%f;%f;%f;%f",position_Rotary[Y_Rotary],position_RGB[Y_RGB][High],position_RGB[Y_RGB][Low],position_Rotary[radian_Rotary],position_RGB[radian_RGB][High],position_RGB[radian_RGB][Low]);	
//	printf("%f;%f;%f;",position_Rotary[X_Rotary],position_Rotary[Y_Rotary],position_Rotary[radian_Rotary]);
//	printf("%f;%f;%f;",position_Rotary_0[X_Rotary],position_Rotary_0[Y_Rotary],position_Rotary_0[radian_Rotary]);
//	printf("%f;%f;%f;",position_Rotary[radian_Rotary],position_RGB[radian_RGB][High],position_RGB[radian_RGB][Low]);
//	printf("%f;%f;%f;%f",position_RGB[Y_RGB][High],position_RGB[Y_RGB][Low],position_RGB[radian_RGB][High],position_RGB[radian_RGB][Low]);	
//	printf("%f;%f;",position_PSD[Y_PSD],position_PSD[radian_PSD]);
//	printf("\n");

}



//*******�x�W�F�Ȑ��̃N���X*******//
double Bezier_Class::p_x;
double Bezier_Class::p_y;
double Bezier_Class::t_px;
double Bezier_Class::t_py;
double Bezier_Class::t_rad;
Bezier_Class::Bezier_Class(void){
	t = 0;
	p0_x=0; p1_x=0; p2_x=0; p3_x=0;	
	p0_y=0; p1_y=0; p2_y=0; p3_y=0;	
	ac_rate = 0;	
	vel_rate = 0;
	p_x=0; p_y=0;
	t_px=0;	t_py=0; t_rad=0;
	
	Px[0]=0; Px[1]=0; Px[2]=0;
	Py[0]=0; Py[1]=0; Py[2]=0;
	ideal_vel = 0;
}
	
void Bezier_Class::W_time(double time){
	t = time;
}//���ԏ����o��

void Bezier_Class::W_p_x(double p0, double p1, double p2, double p3){
	p0_x = p0;
	p1_x = p1;
	p2_x = p2;
	p3_x = p3;
}//����_�i���������j�����o��

void Bezier_Class::W_p_y(double p0, double p1, double p2, double p3){
	p0_y = p0;
	p1_y = p1;
	p2_y = p2;
	p3_y = p3;
}//����_�i���������j�����o��

void Bezier_Class::W_p(){
//	p_x = point_x;
//	p_y = point_y;
	p_x = (1-t)*(1-t)*(1-t)*p0_x + 3*(1-t)*(1-t)*t*p1_x + 3*(1-t)*t*t*p2_x + t*t*t*p3_x;
	p_y = (1-t)*(1-t)*(1-t)*p0_y + 3*(1-t)*(1-t)*t*p1_y + 3*(1-t)*t*t*p2_y + t*t*t*p3_y;
}//���Ԃ��Ƃ̍��W�����o��(���Ƃ��v�Z)

void Bezier_Class::W_Vel_rate(double v_rate){
	vel_rate = v_rate;
}

void Bezier_Class::W_Ac_rate(double a_rate){
	ac_rate = a_rate;
}
void Bezier_Class::W_ideal_vel(double i_v){
	ideal_vel = i_v;
}

void Bezier_Class::W_point(double x, double y, double r) {

	t_px = x;
	t_py = y;
	t_rad = r;
}


double Bezier_Class::R_time(void){ return t; }
double Bezier_Class::R_p_x(int no){
	switch (no)
		{
		case P0:return p0_x;
		break;
		case P1:return p1_x;
		break;
		case P2:return p2_x;
		break;
		case P3:return p3_x;
		break;
		default://ERROR_STOP
			break;
		}
}
double Bezier_Class::R_p_y(int no){
	switch (no)
		{
		case P0:return p0_y;
		break;
		case P1:return p1_y;
		break;
		case P2:return p2_y;
		break;
		case P3:return p3_y;
		break;
		default://ERROR_STOP
			break;
		}
}
double Bezier_Class::R_p(int no){
	switch (no)
		{
		case PX:return p_x;
		break;
		case PY:return p_y;
		break;
		default://ERROR_STOP
			break;
		}
}

double Bezier_Class::R_Vel_rate(void){
	return vel_rate;
}
double Bezier_Class::R_Ac_rate(void){
	return ac_rate;
}

double Bezier_Class::R_ideal_vel(void){
	return ideal_vel;
}

double Bezier_Class::R_point(int no) {

	switch (no)
	{
	case X:return t_px; break;
	case Y:return t_py; break;
	case radian:return t_rad;break;
	default: return -1;break;
	}

}


//*******���[�^���[�G���R�[�_�̃N���X*******//
//�R���X�g���N�^//
Rotary_Class::Rotary_Class(void) {

	angular_velocity_self[0]=0;
	angular_velocity_self[1]=0;
	angular_velocity_self[2]=0;
	angular_velocity_self[3]=0;
	Velocity=0;
}

//�@�̂̑��x�̏������݁i�l�j
void Rotary_Class::W_Maschine_Velocity(double m_v){Velocity=m_v;}

//���Ȉʒu���W�̏������݁i[X_Rotary,Y_Rotary,radian_Rotary]�j
void Rotary_Class::W_Position_Rotary(double posi, int no){
	switch (no)
	{
	case X_Rotary: position_Rotary[X_Rotary]=posi;break;
	case Y_Rotary: position_Rotary[Y_Rotary]=posi;break;
	case radian_Rotary: position_Rotary[radian_Rotary]=posi;break;
	default: break;
	}
}
//���m�p�^�C���̊p���x�̏�������(�l,[Right,Left]�j
void Rotary_Class::W_Angular_Velocity_Self(double a_v, int no) {

	switch (no)
	{
	case RO1:angular_velocity_self[RO1] = a_v;break;
	case RO2:angular_velocity_self[RO2] = a_v;break;
	case RO3:angular_velocity_self[RO3] = a_v;break;
	case RO4:angular_velocity_self[RO4] = a_v;break;
	default: break;
	}
}


//���m�p�^�C���̊p���x�̓ǂݏo���i�l[Right,Left]�j
double Rotary_Class::R_Angular_Velocity_Self(int no){
	
	switch (no)
	{
	case RO1:return angular_velocity_self[RO1];
	break;
	case RO2:return angular_velocity_self[RO2];
	break;
	case RO3:return angular_velocity_self[RO3];
	break;
	case RO4:return angular_velocity_self[RO4];
	break;
	default: break;
	}	
}

//�@�̂̑��x�̓ǂݏo���i�j
double Rotary_Class::R_Maschine_Velocity(void){return Velocity;}

void Rotary_Class::Rotary_pri(void){
	//���[�^���[�J�E���g��
	sci_printf("RO1:%d  RO2:%d  RO3:%d  RO4:%d\n\r", rotary(RO1), rotary(RO2), rotary(RO3), rotary(RO4));
	//���[�^���[�̊p���x
	sci_printf("a_vel1:%lf  a_vel2:%lf  a_vel3:%lf  a_vel4:%lf\n\r", angular_velocity_self[RO1], angular_velocity_self[RO2], angular_velocity_self[RO3], angular_velocity_self[RO4]);
	//�@�̂̈ړ����x
	sci_printf("Vel:%lf", Velocity);
}



//****�t���O�̃N���X****//
//�R���X�g���N�^//
Flag_Class::Flag_Class(int f){
	flag=f;
	dip_on_off[0]=0;
	dip_on_off[1]=0;
	dip_on_off[2]=0;
	dip_on_off[3]=0;
}
//flag���グ��
void Flag_Class::flag_up(void){flag=1;}
//flag��������
void Flag_Class::flag_down(void){flag=0;}
//flag�̃J�E���g�A�b�v?
void Flag_Class::flag_count_up(void){flag++;}
//flag�̃��Z�b�g
void Flag_Class::flag_Reset(void){flag=0;}
//�t���O�̏�������
void Flag_Class::W_flag(int f){flag=f;}
//flag�̌Ăяo��
int Flag_Class::R_flag(void){return flag;}

void Flag_Class::W_dip(bool s,int no){
	dip_on_off[no-1]=s;
}

bool Flag_Class::R_dip(int no){
	return dip_on_off[no-1];
}



//****��n���s�m�F�N���X****//
//�R���X�g���N�^//
Run_No_Class::Run_No_Class(void){run_no=0;}
//�ǂݏo��
int Run_No_Class::R_run_no(void){return run_no;}
//��������
void Run_No_Class::W_run_no(int n){run_no=n;}



/*****���Ԃ̃N���X****/

TIME_Class::TIME_Class(void){
	
	Hz=0;
	TIME=0;
}

	//�J�E���g
void TIME_Class::Count(void){
	
	TIME+=1/Hz;	//�P��[�b]
}
//�����̏�������
void TIME_Class::W_HZ(double hz){
	
	Hz=hz;
}
	//���Ԃ̓ǂݏo��
double TIME_Class::R_TIME(void){
	return TIME;	
}




void Ref::W_poji_ref(double xref, double yref, double yawref){
	X_ref = xref;
	Y_ref = yref;
	YAW_ref = yawref;
}

void Ref::W_Vel_ref(double vx, double vy, double vyaw){
	Vx_ref = vx;
	Vy_ref = vy;
	Vyaw_ref = vyaw;
}

double Ref::R_poji_ref(int num){
	switch(num){
		case X:return X_ref;
		case Y:return Y_ref;
		case radian:return YAW_ref;
	}
}

double Ref::R_Vel_ref(int num){
	switch(num){
		case X:return Vx_ref;
		case Y:return Vy_ref;
		case radian:return Vyaw_ref;
	}
}