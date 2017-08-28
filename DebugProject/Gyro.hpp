#ifndef _GYRO_H_
#define _GYRO_H_

class Gyro{
public:
	virtual int Begin(unsigned int) = 0;
	virtual float getYaw(void) = 0;
};

#endif