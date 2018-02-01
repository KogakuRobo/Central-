#ifndef _CLASS_HPP_
#define _CLASS_HPP_



//****�^�C���E���[�^�̃N���X****//
class Wheel_Class {

private:
	double M_out;				//���[�^�[�o��
	double M_out_last;			//�P�O�̃��[�^�[�o��
	double angular_velocity;		//�v������^�C���̊p���x
	double observation_angular_velocity;	//�����l�̃^�C���̊p���x

public:
	Wheel_Class(void);			//�R���X�g���N�^
	void W_Motor_out(int m_r_out);		//���[�^�[�̏o��(��������)
	void W_Wheel_a_v(double w_r_a_v);	//�v������^�C���̊p���x(��������)
	void W_Wheel_o_a_v(double w_o_r_a_v);	//�����l�̃^�C���̊p���x(��������)
	double R_Wheel_a_v(void);		//�v������^�C���̊p���x(�ǂݏo��)
	double R_Wheel_o_a_v(void);		//�����l�̃^�C���̊p���x(�ǂݏo��)
	double R_Motor_out(void);		//���[�^�[�̏o��(�ǂݏo��)
	double R_Motor_out_last(void);		//�P�O�̃��[�^�[�̏o��(�ǂݏo��)
};



class Velocity_Class {//���x����i2017NHK���v���O�������p�j
private:
	double arrival_time;			//���B����
	double accel_time;			//����������
	double ratio;				//�@�������s���ԁ���������
	double MAX_velocity;
	static double calculated_distance;
	double K;				//�o�̓X�s�[�h�̒萔
	double ideal_velocity;			//���z���x


public:
	//�R���X�g���N�^//
	Velocity_Class(void);
	//���B���Ԃ̏�������(�l)
	void W_arrival_time(double ar_t);
	//���������Ԃ̎Z�o
	//arrival_time��ratio�̓��͌�ɓ���
	void C_accel_time(void);
	//�������Ԃ̊����̏�������(�l)
	//�������s���ԁ����B����
	void W_ratio(double r);
	//�ō����x�̏�������(�l)
	void C_MAX_velocity(double x);
	//�v�Z��̋����̏������݁i�l�j
	void W_calculated_distance(double c_d);
	//�v�Z��̋����C���l�������݁i�l�j
	void W_New_calculated_distance(double n_c_d);	
	//�o�̓X�s�[�h�̌W���������݁i�l�j
	void W_coefficient(double k);
	//���z���x�̏�������
	void W_ideal_velocity(double i_v);

	//���B���Ԃ̓ǂݏo��()
	double R_arrival_time(void);
	//���������Ԃ̓ǂݏo��()
	double R_accel_time(void);
	//���B���Ԃ̊����̓ǂݏo��()
	double R_ratio(void);
	//�ō����x�̓ǂݏo��()
	double R_MAX_velocity(void);
	//�v�Z��̋����̓ǂݏo��()
	double R_calculated_distance(void);

	//�o�̓X�s�[�h�̏C���v�Z(���_��̋���,���H��̋���)
	double revise_speed(double theory_d,double practice_d);
	//calculated_distance�̃��Z�b�g
	void Reset_distance(void);
	//���z���x�̓ǂݏo��
	double R_ideal_velocity(void);

};

class Target_Point_Class { //�ڕW�_�֌W�̃N���X�i�Q�O�P�V�N�v���O����藬�p�j
private :
	double point[3];
	double target_distance;		//�ڕW�܂ł̋���
	static double last_point_X;
	static double last_point_Y;
	

public:
	//�R���X�g���N�^//
	Target_Point_Class(void);
	//�ڕW�_�y�ъp�x�̏�������
	void W_point(double x, double y, double r);
	//�ڕW�����̏������݁i�l�j
	void W_Target_Distance(double t_d);
	//�I�����_��X���W�̏�������
	void W_last_point_X(double l_x);
	//�I�����_��Y���W�̏�������
	void W_last_point_Y(double l_y);
	


	//�ڕW�_�y�ъp�x�̓ǂݏo��()
	double R_point(int no);
	//�ڕW�����̓ǂݏo���i�j
	double R_Target_Distance(void);
	//�I�����_��X���W�̓ǂݏo��
	double R_last_point_X(void);
	//�I�����_��Y���W�̓ǂݏo��
	double R_last_point_Y(void);

};

class My_Position_Class {	//���Ȉʒu
protected :
	static double position[4];		//���Ȉʒu����oX,Y,radian�p
	static double position_Rotary[4];	//Rotary�ɂ�鎩�Ȉʒu����{X_Rotary,Y_Rotary,radian_Rotary}
	static double position_Rotary_0[4];
	
											
public:
	//�R���X�g���N�^//
	My_Position_Class(void);

	//���Ȉʒu���W�̏������݁i�l,[X,Y,radian]�j
	void W_Position(double posi, int no);	
	//���Ȉʒu���W�̏������݁i[X,Y,radian]�j
	void W_Position(double x, double y, double r);		
	//���Ȉʒu���W�̓ǂݏo���i[X,Y,radian]�j
	double R_Position(int no);
	//���Ȉʒu���W�̓ǂݏo���i[X_Rotary,Y_Rotary,radian_Rotary]�j
	double R_Position_Rotary(int no);
	//���Ȉʒu�̓���
	void position_Fusion(void);
	
	void position_pri(void);
};


/****�x�W�F�Ȑ��֌W****/
class Bezier_Class {
private :
	double t;	//�ڕW���W�܂ł̎��ԁi 0<=t<1 �j
	double p0_x, p1_x, p2_x, p3_x;	//����_�i�����j
	double p0_y, p1_y, p2_y, p3_y;	//����_�i�����j
	double vel_rate;//�����̎��Ԋ���(vel�N���X�ł���)
	double ac_rate;//�������̎��Ԋ���(vel�N���X�ł���)
	static double p_x, p_y;//����t�ł̍��W
	
	double Px[3];	//���݂̍��W�A�P���Ƃ̍��W�A��
	double Py[3];	//���݂̍��W�A�P���Ƃ̍��W�A��
	double ideal_vel;//�u�ԑ��x���z�l
	
	static double t_px, t_py, t_rad;
	
public :
	Bezier_Class(void);
	void W_time(double time);//���ԏ����o��
	void W_p_x(double p0, double p1, double p2, double p3);//����_�i���������j�����o��
	void W_p_y(double p0, double p1, double p2, double p3);//����_�i���������j�����o��
	void W_p();//���Ԃ��Ƃ̍��W�����o��
	void W_point(double x, double y, double r);
	void W_Vel_rate(double v_rate);
	void W_Ac_rate(double a_rate);
	void W_ideal_vel(double i_v);
	
	double R_time(void);
	double R_p_x(int no);
	double R_p_y(int no);
	double R_p(int no);
	double R_Vel_rate(void);
	double R_Ac_rate(void);
	double R_ideal_vel(void);
	double R_point(int no);
};


/****���[�^���[�G���R�[�_�̃N���X****/
class Rotary_Class : public My_Position_Class{
private :
	double angular_velocity_self[4];	//���Ȉʒu����p���[�^���[�̊p���x
	double Velocity;			//�@�̂̈ړ����x
public:
	//�R���X�g���N�^//
	Rotary_Class(void);
	//�@�̂̑��x�̏������݁i�l�j
	void W_Maschine_Velocity(double m_v);
	//���Ȉʒu���W�̏������݁i�l,[X_Rotary,Y_Rotary,radian_Rotary]�j
	void W_Position_Rotary(double posi, int no);	
	//���m�p�^�C���̊p���x�̏������݁i�l,[Right,Left]�j
	void W_Angular_Velocity_Self(double a_v, int no);
	//���m�p�^�C���̊p���x�̓ǂݏo���i�l[Right,Left]�j
	double R_Angular_Velocity_Self(int no);
	//�@�̂̑��x�̓ǂݏo���i�j
	double R_Maschine_Velocity(void);
	//���W�Z�o
	void calculation_coordinate_Rotary(void);
	//print�֐�
	void Rotary_pri(void);
};

class Flag_Class {//�t���O�֌W�i2017NHK�����j
private :
	int flag;
	bool dip_on_off[4];
public:
	//�R���X�g���N�^//
	Flag_Class(int f);
	//flag���グ��
	void flag_up(void);
	//flag��������
	void flag_down(void);
	//flag�̃J�E���g�A�b�v?
	void flag_count_up(void);
	//flag�̃��Z�b�g
	void flag_Reset(void);
	//�t���O�̏�������
	void W_flag(int f);
	//flag�̌Ăяo��
	int R_flag(void);
	
	void W_dip(bool s,int no);
	bool R_dip(int no);
};


//****��n���s�m�F�N���X****//
class Run_No_Class{
	private:
	int run_no;
	public:
	//�R���X�g���N�^//
	Run_No_Class(void);
	//�ǂݏo��
	int R_run_no(void);
	//��������
	void W_run_no(int n);
	
};



class TIME_Class {
private:
	double TIME;
	double Hz;
	
public:
	//�R���X�g���N�^//
	TIME_Class(void);
	//�J�E���g
	void Count(void);
	//�����̏�������
	void W_HZ(double hz);
	//���Ԃ̓ǂݏo��
	double R_TIME(void);
};

//���x�E���W�ڕW�l
class Ref{
	private:
	double X_ref, Y_ref, YAW_ref;
	double Vx_ref, Vy_ref, Vyaw_ref;
	
	public:
	void W_poji_ref(double xref, double yref, double yawref);
	void W_Vel_ref(double vx, double vy, double vyaw);
	double R_poji_ref(int num);
	double R_Vel_ref(int num);
};
#endif