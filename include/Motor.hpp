#ifndef _Motor_HPP_
#define _Motor_HPP_

typedef enum{
	MOTOR_SET_FREQUENCY,
	MOTOR_SET_DUTY,
	MOTOR_BEGIN,
	MOTOR_STOP,
	MOTOR_IOCTL_LAST,
}motor_ioctl_cmd;

#define MOTOR_DEFAULT_FREQUENCY		20	//[kHz]

#endif