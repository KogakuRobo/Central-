#include <math.h>
#include "Robot.hpp"

void Set_MotorSystemCGain(MotorSystem *ms,float K,float Ti,float Td);
void Set_MotorSystemVGain(MotorSystem *ms,float K,float Ti,float Td);

Robot::Robot(Localization *_l,
	MotorSystem* _motora,
	MotorSystem* _motorb,
	MotorSystem* _motorc,
	MotorSystem* _motord
) : loca(_l), leg_motora(_motora), leg_motorb(_motorb), leg_motorc(_motorc), leg_motord(_motord),
x_pid(1200,5,0.0001,0.01),
y_pid(1200,5,0.0001,0.01),
yaw_pid(5000.0,0,0,0.01)
{
	PI = 3.1415926535;
	
	x_ref = 0;
	y_ref = 0;
	yaw_ref = 0;
	
	Vx_ref = 0;
	Vy_ref= 0;
	Vyaw_ref= 0;
}

void Robot::Begin(long position_period){
	leg_motora->SetVcc(12);
	leg_motorb->SetVcc(12);
	leg_motorc->SetVcc(12);
	leg_motord->SetVcc(12);
	
	Set_MotorSystemCGain(leg_motora,4.0,0.3,0.0);
	Set_MotorSystemCGain(leg_motorb,4.0,0.3,0.0);
	Set_MotorSystemCGain(leg_motorc,4.0,0.3,0.0);
	Set_MotorSystemCGain(leg_motord,4.0,0.3,0.0);
		
	Set_MotorSystemVGain(leg_motora,1.0,0.1,0.00001);
	Set_MotorSystemVGain(leg_motorb,1.0,0.1,0.00001);
	Set_MotorSystemVGain(leg_motorc,1.0,0.1,0.00001);
	Set_MotorSystemVGain(leg_motord,1.0,0.1,0.00001);
		
	thread_create(&th_control,CT_PRIORITY_MAX + 3,Robot::thread_handle,(void*)this);
}

void Robot::Safe(void){
	//*/
	float yaw = loca->GetYaw();
	float terget = yaw_pid.Run(yaw,yaw_ref) + Vyaw_ref;
	
	float x = loca->GetX();
	float x_terget = x_pid.Run(x,x_ref) + Vx_ref;

	float y = loca->GetY();
	float y_terget = y_pid.Run(y,y_ref) + Vy_ref;
	//*/
	leg_motora->SetVelocity(- x_terget * sin(       PI / 4  - yaw) + y_terget * cos(       PI / 4  - yaw) - terget);
	leg_motorb->SetVelocity(- x_terget * sin(   3 * PI / 4  - yaw) + y_terget * cos(   3 * PI / 4  - yaw) - terget);
	leg_motorc->SetVelocity(- x_terget * sin( - 3 * PI / 4  - yaw) + y_terget * cos( - 3 * PI / 4  - yaw) - terget);
	leg_motord->SetVelocity(- x_terget * sin( - 1 * PI / 4  - yaw) + y_terget * cos( - 1 * PI / 4  - yaw) - terget);
	//*/
	
	//printf("X,%f,Y,%f,yaw,%f\n\r",loca->GetX(),loca->GetY(),loca->GetYaw());
	//printf("X,%f,Y,%f,yaw,%f\n\r",x_terget,y_terget,terget);
	
}

void *Robot::thread_handle(thread_t *t,void *attr){
	Robot *This = (Robot *)attr;
	while(1){
		This->Safe();
		msleep(10);
	}
	return NULL;
}


int Robot::SetPostionNode(float x,float y,float yaw,float vx,float vy,float vyaw){
	this->x_ref = x;
	this->y_ref = y;
	this->yaw_ref = yaw;
	
	this->Vx_ref = vx;
	this->Vy_ref = vy;
	this->Vyaw_ref = vyaw;
	
	return 0;
}

void Set_MotorSystemCGain(MotorSystem *ms,float K,float Ti,float Td){
	ms->SetCGain_P(K);
	ms->SetCGain_I(Ti);
	ms->SetCGain_D(Td);
}


void Set_MotorSystemVGain(MotorSystem *ms,float K,float Ti,float Td){
	ms->SetVGain_P(K);
	ms->SetVGain_I(Ti);
	ms->SetVGain_D(Td);
}