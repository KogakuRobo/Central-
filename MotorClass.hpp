#ifndef _MotorClass_HPP_
#define _MotorClass_HPP_


#include"CentralLibrary.h"

class Motor{
	int motor_dev;
	
	const char *name;
public:
	Motor(const char *_n = NULL);
	int SetDuty(float);
};

#endif