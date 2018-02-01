#ifndef _PID_my_H_		//pid����p
#define _PID_my_H_

class PID_my {
	float KP;
	float KI;
	float KD;
	float TIME;
	double integral;
	double diff[2];
	double adjustment;
public:
	PID_my(float kp, float ki, float kd, float time);	//�R���X�g���N�^
	double PID_act(double target, double output);	//PID���s �ڕW�l�ƌ��ݒn
	double PID_act2(double poor);	//PID���s�Q�@����diff[1]����
	void PID_Reset(void);	//�ϕ��l�̃��Z�b�g
};

#endif