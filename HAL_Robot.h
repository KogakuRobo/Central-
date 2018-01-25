#ifndef HAL_ROBOT_H_
#define HAL_ROBOT_H_

#include "Robot.hpp"
#include "localization.hpp"
#include "MotorSystem_Control.hpp"
#include"my_position.h"
#include"target_point.h"
#include"speed_control.h"

class HAL_Robot :public Robot {
	float operating_time;
	char flag;
	my_position* my_posi;
	target_point* t_point;
	speed_control* s_con;
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
	void Begin(void);
private:
	void Safe(void);
};

#endif
