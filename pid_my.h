#ifndef _PID_my_H_		//pid制御用
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
	PID_my(float kp, float ki, float kd, float time);	//コンストラクタ
	double PID_act(double target, double output);	//PID実行 目標値と現在地
	double PID_act2(double poor);	//PID実行２　直接diff[1]入力
	void PID_Reset(void);	//積分値のリセット
};

#endif