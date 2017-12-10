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
	
	//x,y,yaw��pid����N���X
	PID<float> x_pid;
	PID<float> y_pid;
	PID<float> yaw_pid;
	
	float Vx_ref;
	float Vy_ref;
	float Vyaw_ref;
	
	float x_ref;
	float y_ref;
	float yaw_ref;
	
	//�~����
	float PI;
	
	//Safe�X���b�h
	thread_t th_control;
	
	enum{
		INIT,
		RUNNING,
		STOP,
	}state;
public:
	Robot(Localization *_l,
	MotorSystem* _motora,
	MotorSystem* _motorb,
	MotorSystem* _motorc,
	MotorSystem* _motord
	);
	
	//Robot�N���X�̊J�n�B���[�^�V�X�e���̃C�j�V�����C�Y��Safe�X���b�h�J�n
	void Begin(long position_period = 10);
	void Stop(void);
	int SetPostionNode(float x,float y,float yaw,float vx,float vy,float vyaw);
	PID<float>& GetXPID(void);
	PID<float>& GetYPID(void);
	PID<float>& GetYawPID(void);
private :
	void Safe(void);
	static void *thread_handle(thread_t *t,void *attr);
};

#endif