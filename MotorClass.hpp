#ifndef _MotorClass_HPP_
#define _MotorClass_HPP_

#include <stddef.h>

class Motor{
	int motor_dev;
	
	const char *name;
public:
	Motor(const char *_n = NULL);
	virtual int SetDuty(float);
};

#endif