#ifndef _CLASS_PID_HPP_
#define _CLASS_PID_HPP_

//ＰＩＤ関係
class pid_class{
	double kp, ki, kd, k;
	static double dt;
	double adj, posi;
	double inte;
	double diff[2];	//0:before  1:now
	double pid_adj;
public:
	pid_class();
	void pid_gein(double p, double i, double d, double k);
	double pid_ctr(double adj, double pos);
	double pid_ctr(double adj);
	void pid_time(double t);
	void pid_print();
	
	void pid_reset(void);
};

//ロータリー関係
class Rotary_class : public pid_class{
	int hensa[2];	//ロータリーカウント数
//	double V_now;
public:
	Rotary_class();
	void MOTOR_DUTY();
	void Rotary_print(int ro);
};

#endif