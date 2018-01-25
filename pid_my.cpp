#include<iostream>
#include "PID_my.h"
//コンストラクタ//
PID_my::PID_my(float kp, float ki, float kd, float time) {
	KP = kp;
	KI = ki;
	KD = kd;
	TIME = time;
	integral = 0;
	diff[0] = 0;
	diff[1] = 0;
	adjustment = 0;
}
//PID_my実行//
double PID_my::PID_act(double target, double output) {
	diff[0] = diff[1];
	//printf("%f,%f\n",target,output);
	diff[1] = target - output;
	integral += ((diff[1] + diff[0]) / 2) *TIME;
	adjustment = KP * diff[1] + KI * integral + KD * (diff[1] - diff[0]) / TIME;
	if (diff[0] * diff[1]<0) { integral = 0; }

	return adjustment;
}

double PID_my::PID_act2(double poor) {
	diff[0] = diff[1];
	diff[1] = poor;
	integral += ((diff[1] + diff[0]) / 2) *TIME;
	adjustment = KP * diff[1] + KI * integral + KD * (diff[1] - diff[0]) / TIME;
	if (diff[0] * diff[1]<0) { integral = 0; }

	return adjustment;
}
//積分値のリセット//
void PID_my::PID_Reset(void) {

	integral = 0;
}