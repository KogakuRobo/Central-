/*#include<iostream>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include"speed_control.h"

speed_control::speed_control(target_point*pt_posi,my_position*pmy_posi):pid_x(10, 0, 0, 0.01), pid_y(10, 0, 0, 0.01),pid_yaw(80, 0, 8, 0.01) {
	wheel =0;
	output = 0;
	output_B = 0;
	theta = 0;
	my_theta=0;
	for (int i = 0; i < 2; i++) {
		V[i]=0;	//機体の速度
		V_B[i]=0;	//機体の速度
		poor[i]=0;
		TARGET_POSI[i]=0;
		BEFORE_TARGET_POSI[i]=0;
		MY_POSI[i]=0;
	}
	V_B[2]=0;
	MY_POSI[2] = 0;
	t_posi = pt_posi;
	my_posi = pmy_posi;
}

double speed_control::output_F_get(int WHEEL ,double omega) {
	wheel=WHEEL;
	switch(wheel) {
		case 1:
		//printf("%f\n",V[X]);
		//printf("%f\n",V[X]);
		//	output = -1*(- V[X]*sin(theta+pi/4)+V[Y]*sin(theta-pi/4) + L_wheel_distance*omega); //左上
			output = -1*(- V[X]/sin(theta+pi/4)+V[Y]/sin(theta-pi/4) + L_wheel_distance*omega)*5;
			break;
		case 4:
			//output =-1*( V[X]*sin(theta +3* pi / 4) - V[Y]*sin(theta + pi / 4) + L_wheel_distance*omega); //左下
			output =-1*( V[X]/sin(theta +3* pi / 4) - V[Y]/sin(theta + pi / 4) + L_wheel_distance*omega)*5;
			break;
		case 3:
			//output =-1*( V[X]*sin(theta + pi / 4) + V[Y]*sin(theta +3* pi / 4) + L_wheel_distance*omega); //右下
			output =-1*( V[X]/sin(theta + pi / 4) + V[Y]/sin(theta +3* pi / 4) + L_wheel_distance*omega)*5; 
			break;
		case 2:
			//output = -1*(V[X]*sin(theta - pi / 4) + V[Y]*sin(theta + pi / 4) + L_wheel_distance*omega); //右上
			output = -1*(V[X]/sin(theta - pi / 4) + V[Y]/sin(theta + pi / 4) + L_wheel_distance*omega)*5;
			break;
	}
	//if(output*2.0 / M_W_diameter>80)output_B=0;
	return output*2.0 / M_W_diameter;
}

double speed_control::output_B_get(int WHEEL_B) {
	wheel = WHEEL_B;
	my_theta = my_posi->m_posi_angle_give();
	switch (wheel) {
	case 1:
		output_B = V_B[X]*sin(-my_theta + pi / 4) - V_B[Y]*sin(-my_theta - pi / 4) - L_wheel_distance*V_B[ANGLE]; //左上
		break;
	case 4:
		output_B =-V_B[X]*sin(-my_theta + 3 * pi / 4) + V_B[Y]*sin(-my_theta + pi / 4) - L_wheel_distance* V_B[ANGLE]; //左下
		break;
	case 3:
		output_B = -V_B[X]*sin(-my_theta + pi / 4) - V_B[Y]*sin(-my_theta + 3 * pi / 4) - L_wheel_distance* V_B[ANGLE]; //右下
		break;
	case 2:
		output_B = -V_B[X]*sin(-my_theta - pi / 4) - V_B[Y]*sin(-my_theta + pi / 4) - L_wheel_distance* V_B[ANGLE]; //右上
		break;
	}
	//printf("%f\n",output_B);
	
	return output_B*2.0 / M_W_diameter;
}

void speed_control::robot_speed_F_set(char move_type) {
	BEFORE_TARGET_POSI[X] = TARGET_POSI[X];
	BEFORE_TARGET_POSI[Y] = TARGET_POSI[Y];
	t_posi->t_posi_set(move_type);
	t_posi->t_posi_give(TARGET_POSI);
	//printf("%f  %f\n",TARGET_POSI[X],TARGET_POSI[Y]);
	poor[X] = TARGET_POSI[X] - BEFORE_TARGET_POSI[X];
	poor[Y] = TARGET_POSI[Y] - BEFORE_TARGET_POSI[Y];
	//printf("%f\n",poor[X]);
	V[X] = poor[X] / cmt;
	V[Y] = poor[Y] / cmt;
	//BEFORE_TARGET_POSI[X] = TARGET_POSI[X];
	//BEFORE_TARGET_POSI[Y] = TARGET_POSI[Y];
}

void speed_control::robot_speed_B_set(void) {*/
	/*my_posi->m_posi_give(MY_POSI);
	poor[X] = BEFORE_TARGET_POSI[X] - MY_POSI[X];
	poor[Y] = BEFORE_TARGET_POSI[Y] - MY_POSI[Y];
	poor[ANGLE]=0- MY_POSI[ANGLE];*/
	//printf("TEST6\n");
	//printf("%f,%f,%f\n",my_posi->m_posi_x_give(),my_posi->m_posi_y_give(), my_posi->m_posi_angle_give());
/*	V_B[X] = pid_x.PID_act(BEFORE_TARGET_POSI[X], my_posi->m_posi_x_give());
	V_B[Y] = pid_y.PID_act(BEFORE_TARGET_POSI[Y], my_posi->m_posi_y_give());
	V_B[ANGLE]= pid_yaw.PID_act(0, my_posi->m_posi_angle_give());
	//printf("%f,%f\n",BEFORE_TARGET_POSI[X],BEFORE_TARGET_POSI[Y]);
	//printf("TEST7\n");

}

void speed_control::output_F_give(void) {
	robot_speed_F_set(1);
	//output_F_get(0);
	//printf("%f\n",output*2 / M_W_diameter);
	//return output*2.0 / M_W_diameter;
} 

void speed_control::output_B_give(void) {
	robot_speed_B_set();
	/*output_B_get();
	return output_B*2 / M_W_diameter;*/
/*}

double speed_control::robot_speed__theta_B(void) {
	return pid_yaw.PID_act(0, my_posi->m_posi_angle_give());
}

double speed_control::robot_speed__X_B(void) {
	return pid_x.PID_act(BEFORE_TARGET_POSI[X], my_posi->m_posi_x_give());
}

double speed_control::robot_speed__Y_B(void) {
	return pid_y.PID_act(BEFORE_TARGET_POSI[Y], my_posi->m_posi_y_give());
}
*/
/*double speed_control::set_velocity(void) {
	robot_speed_F();
	robot_speed_B();
	output_F_get(0);
	leg_motora->SetVelocity(-x_terget * sin(PI / 4 - yaw) + y_terget * cos(PI / 4 - yaw) - terget);

}*/