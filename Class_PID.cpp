#include <stdio.h>
#include <stdlib.h>
#include <iodefine.h>
#include "Class_PID.hpp"
#include "Motion.hpp"
#include "Define.hpp"
#include "UART.h"
#include "System.hpp"
#include "ENUM.hpp"

extern pid_class g_pid;
extern Rotary_class g_rotary;
double V_now = 0;

#ifdef __cplusplus
extern "C"{
	int VNOW();
}
#endif


double pid_class::dt;
pid_class::pid_class(){
	k = 0;
	kp = 0;
	ki = 0;
	kd = 0;
	dt = 0.01;
	inte = 0;
	posi = 0;
	adj = 0;
	pid_adj = 0;
	diff[0] = 0;	diff[1] = 0;
}

void pid_class::pid_gein(double p, double i, double d, double K){
	k = K;
	kp = p;
	ki = i;
	kd = d;
}

double pid_class::pid_ctr(double adj, double pos){
	diff[0] = diff[1];
	diff[1] = adj - pos;
//	printf("dif1:%f  dif2:%f\n", diff[0], diff[1]);
	inte += ((diff[1] + diff[0]) / 2) * dt;

	pid_adj = k * (kp * diff[1] + ki * inte + kd * ((diff[0] - diff[1]) / dt));
	posi = pid_adj;

	return pid_adj;
}

double pid_class::pid_ctr(double adj){
	diff[0] = diff[1];
	diff[1] = adj;
//	printf("dif1:%f  dif2:%f\n", diff[0], diff[1]);
	
	inte += ((diff[1] + diff[0]) / 2) * dt;
//	printf("%f\n", inte);

	pid_adj = kp * diff[1] + ki * inte + kd * ((diff[1] - diff[0]) / dt);
//	posi = pid_adj;

	return pid_adj;
}

void pid_class::pid_time(double t){
	dt = 1/t;
}

void pid_class::pid_print(){
	sci_printf("%lf\r\n", pid_ctr(adj, posi));
}

void pid_class::pid_reset(void){
	inte = 0;
}




Rotary_class::Rotary_class(){
	hensa[0] = 0;
	hensa[1] = 0;
	V_now = 0;
}

/*void Rotary_class::MOTOR_DUTY(void){
	hensa[0] = hensa[1];
	hensa[1] = ROTARY(ro_1);
	V_now = (hensa[1] - hensa[0]) * 100 * 2 * 3.1416 / 2000;
	
	if(g_rotary.pid_ctr(30, V_now) >= 0){
		MOTOR_F();
		DUTY_1 = g_rotary.pid_ctr(30, V_now);
	}
	
	else{
		MOTOR_B();
		DUTY_1 = g_rotary.pid_ctr(30, V_now);
	}
	
}*/

void Rotary_class::MOTOR_DUTY(void){
	DUTY_1 = pid_ctr(30, V_now) * 10;
}

int VNOW(void){	//速度算出、速度の値帰すだけ
	static int hensa[2];
	
	hensa[0] = hensa[1];
	hensa[1] = ROTARY(ro_1);
	V_now = (hensa[1] - hensa[0]) * 100 * 2 * 3.1416 / 2000;
	
	return V_now;
	
/*	if(g_rotary.pid_ctr(30, V_now) >= 0){
	//	MOTOR_F();
		return V_now;
		
	}
	
	else{
	//	MOTOR_B();
		return V_now;
	}*/
	
}

void Rotary_class::Rotary_print(int no){
	sci_printf("%d %d %d %d ", g_rotary_count[RO1], g_rotary_count[RO2], g_rotary_count[RO3], g_rotary_count[RO4]);
}