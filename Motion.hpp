#ifndef _MOTION_HPP_
#define _MOTION_HPP_

//#include "Class.hpp"
#include "Robot.hpp"

//モーター制御//
void Feed_Foward(double t, Robot *robot);
void AT_running_control(void);

void accel(void);

//Flag_Class f_s_c;

#endif