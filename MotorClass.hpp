#ifndef _MotorClass_HPP_
#define _MotorClass_HPP_

#include <stddef.h>

class Motor{
	int motor_dev;
	
	const char *name;
public:
//�R���X�g���N�^�F�f�o�C�X�N���Ə��������s���܂��B
//����
//	_n : �f�o�C�X��I�����܂��B
/*2017/08/26�@����
	"MOTOR_A"
	"MOTOR_B"
	"MOTOR_C"
	"MOTOR_D"
*/
	Motor(const char *_n = NULL);
	virtual int SetDuty(float);
};

#endif