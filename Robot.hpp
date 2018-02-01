#ifndef _Robot_HPP_
#define _Robot_HPP_

#include "localization.hpp"
#include "MotorSystem_Control.hpp"
#include "PID.hpp"
/*#include"my_position.h"
#include"target_point.h"
#include"speed_control.h"*/

class Robot{
	
	/*MotorSystem* leg_motora;
	MotorSystem* leg_motorb;
	MotorSystem* leg_motorc;
	MotorSystem* leg_motord;*/
	
	
	
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
	
	enum{
		INIT,
		RUNNING,
		STOP,
	}state;
public:

	/*target_point* t_point;
	my_position* my_posi;
	speed_control* s_con;*/
	Localization *loca;
	MotorSystem* leg_motora;
	MotorSystem* leg_motorb;
	MotorSystem* leg_motorc;
	MotorSystem* leg_motord;

	Robot(Localization *_l,
	MotorSystem* _motora,
	MotorSystem* _motorb,
	MotorSystem* _motorc,
	MotorSystem* _motord
	/*my_position* _my_posi,
	target_point* _t_point,
	speed_control* _s_con*/
	);
	
	//Robotクラスの開始。モータシステムのイニシャライズとSafeスレッド開始
	virtual void Begin(long position_period = 10);
	void Stop(void);
	int SetPostionNode(float x,float y,float yaw,float vx,float vy,float vyaw);
	PID<float>& GetXPID(void);
	PID<float>& GetYPID(void);
	PID<float>& GetYawPID(void);
private :
	virtual void Safe(void);
	static void *thread_handle(thread_t *t,void *attr);
};

/*class HAL_Robot : public Robot{
public:
HAL_Robot(Localization *_l,
	MotorSystem* _motora,
	MotorSystem* _motorb,
	MotorSystem* _motorc,
	MotorSystem* _motord,
	my_position* _my_posi,
	target_point* _t_point,
	speed_control* _s_con
	);
void safe(void);
void Begin(void);
};*/

#endif