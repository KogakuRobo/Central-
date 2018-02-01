#ifndef _Robot_HPP_
#define _Robot_HPP_

#include "localization.hpp"
#include "MotorSystem_Control.hpp"
#include "PID.hpp"

class Robot{
	Localization *loca;
	MotorSystem* leg_motora;
	MotorSystem* leg_motorb;
	MotorSystem* leg_motorc;
	MotorSystem* leg_motord;
	
	//x,y,yawのpid制御クラス
	PID<float> x_pid;
	PID<float> y_pid;
	PID<float> yaw_pid;
	
	float Vx_ref;
	float Vy_ref;
	float Vyaw_ref;
	
	float x_ref;
	float y_ref;
	float yaw_ref;
	
	//円周率
	float PI;
	
	//Safeスレッド
	thread_t th_control;
public:
	Robot(Localization *_l,
	MotorSystem* _motora,
	MotorSystem* _motorb,
	MotorSystem* _motorc,
	MotorSystem* _motord
	);
	
	//Robotクラスの開始。モータシステムのイニシャライズとSafeスレッド開始
	void Begin(long position_period = 10);
	
	int SetPostionNode(float x,float y,float yaw,float vx,float vy,float vyaw);
	
	void SetTergetPoint(float x,float y,float yaw){x_ref = x;y_ref = y;yaw_ref = yaw;}
	void SetTergetVelocity(float vx,float vy,float vyaw){Vx_ref = vx;Vy_ref = vy;Vyaw_ref = vyaw;}
	void Print_Robot(void);
	void Motor_STOP(void);
	
private :
	void Safe(void);
	static void *thread_handle(thread_t *t,void *attr);
};

#endif