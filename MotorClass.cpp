#include"MotorClass.hpp"
#include"Motor.hpp"

Motor::Motor(const char *_n){
	if(_n != NULL){
		name = _n;
		motor_dev = open(name,0,0);
	}
	else {
		motor_dev = -1;
	}
}

int Motor::SetDuty(float d){
	if(motor_dev == -1){
		return -1;
	}
	
	return ioctl(motor_dev,MOTOR_SET_DUTY,(void*)&d);
}