#ifndef _MotorClass_HPP_
#define _MotorClass_HPP_

#include <stddef.h>

class Motor{
	int motor_dev;
	
	const char *name;
public:
//コンストラクタ：デバイス起動と初期化を行います。
//引数
//	_n : デバイスを選択します。
/*2017/08/26　現在
	"MOTOR_A"
	"MOTOR_B"
	"MOTOR_C"
	"MOTOR_D"
*/
	Motor(const char *_n = NULL);
	virtual int SetDuty(float);
};

#endif