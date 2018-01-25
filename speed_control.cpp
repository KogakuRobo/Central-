#include<iostream>
#include<math.h>
#include<stdio.h>
#include"speed_control.h"

speed_control::speed_control(my_position*pmy_posi, target_point*pt_point) :pid_x(100, 0, 0, 0.01), pid_y(100, 0, 0, 0.01), pid_yaw(1400, 0, 90, 0.01) {
	for (int i = 0; i <= 2; i++) {
		t_posi[i];
		speed_F[i];
		speed_B[i];
		slope[i];
		poor[i];
		poor2[i];
		MY_POSI[i];
	}
	quad = 0;
	quad_B = 0;
	angular_velocity_F= 0;
	angular_velocity_ad = 0;
	practical_angular_velocity = 0;
	theta_F = 0;
	angular_velocity_B=0;
	ideal_robot_theta = 0;
	theta_now_ta = 0;
	theta_be_ta = 0;
	theta_B = 0;
	wheel = 0;
	my_posi = pmy_posi;
	t_point = pt_point;
	BEFORE_TARGET_POSI[0]=0;
	BEFORE_TARGET_POSI[1]=0;
	TARGET_POSI[0]=0;
	TARGET_POSI[1]=0;
	TARGET_V[0]=0;
	TARGET_V[1]=0;
	theta[0]=0;
	theta[1]=0;
	V_max_X=0;
	V_max_Y=0;
	count=0;
}

void speed_control::angular_velocity_F_input(char move_type,double ini_posi_X,double ini_posi_Y,double goal_posi_X,double goal_posi_Y,float time,char flag) {
	if(0==flag%2) count=time+1;
        count+=0.01;
	BEFORE_TARGET_POSI[X] = TARGET_POSI[X];
	BEFORE_TARGET_POSI[Y] = TARGET_POSI[Y];
	//t_point->t_posi_set(move_type);
	//t_point->t_posi_give(TARGET_POSI);
	//printf("%f\n",time);
	if(count<time/3){
		//printf("%f\n",goal_posi_Y);
		sji_kasoku(time,goal_posi_X-ini_posi_X,goal_posi_Y-ini_posi_Y,0);
		t_point->t_posi_set(move_type);
		t_point->t_posi_give(TARGET_V);
	
		//sji_kasoku(time,goal_posi_Y-ini_poso_Y);
		//t_point->t_posi_set(move_type);
		//t_point->t_posi_give(TARGET_V);
	//printf("%f\n",TARGET_V[Y]);
	}
	else if(count>=time/3&&count<2*time/3){
		
	}
	else if(count>=2*time/3&&count<=time){
		sji_kasoku(time,goal_posi_X-ini_posi_X,goal_posi_Y-ini_posi_Y,2);
		t_point->t_posi_set(move_type);
		t_point->t_posi_give(TARGET_V);
		//TARGET_V[Y]=1*TARGET_V[Y];
		//printf("%f\n",TARGET_V[Y]);
	}
	else{
		count=0;
		//TARGET_V[X]=0;
		//TARGET_V[Y]=0;
	}
	TARGET_POSI[X]+=TARGET_V[X]*0.01;
		poor[X] = TARGET_POSI[X] - BEFORE_TARGET_POSI[X];
	TARGET_POSI[Y]+=TARGET_V[Y]*0.01;	
		poor[Y] = TARGET_POSI[Y] - BEFORE_TARGET_POSI[Y];
		
		//printf("%f,%f\n",TARGET_POSI[Y] , BEFORE_TARGET_POSI[Y]);
		poor[R] = sqrt(poor[X] * poor[X] + poor[Y] * poor[Y]);
		theta_now_ta = atan2(poor[X], poor[Y]) ;	//rad
		if(poor[X]==0&&poor[Y]==0)theta_now_ta=0;
		//printf("%f\n",theta_F);
		theta_F = theta_now_ta - theta_be_ta;
		//printf("%f\n",theta_F);
		speed_F[Y] = (2 * poor[R]) / (M_W_diameter*cmt);	//ω＝2πｆ　ｆ＝距離/タイヤの円周×ｃｍｔ0
		//printf("%f\n",TARGET_V[Y]);
		speed_F[ANGLE] =2*L_wheel_distance * 0 / M_W_diameter*pi*cmt;	//2*pi*回転する円弧の長さ（距離）/タイヤの円周×ｃｍｔ１

		Quadrant_F();
		//angular_velocity_F = Angular_Velocity_F();
		//angular_velocity = speed[Y] + speed[ANGLE];
		theta_be_ta = theta_now_ta;
		//BEFORE_TARGET_POSI[X] = TARGET_POSI[X];
		//BEFORE_TARGET_POSI[Y] = TARGET_POSI[Y];
}

void speed_control::angular_velocity_B_input(void) {
	my_posi->m_posi_give(MY_POSI);
	poor2[X] = BEFORE_TARGET_POSI[X] - MY_POSI[X];
	poor2[Y] = BEFORE_TARGET_POSI[Y] - MY_POSI[Y];
	//printf("%f,%f\n",poor2[X] , poor2[Y]);
	poor2[R] = sqrt(poor2[X] * poor2[X] + poor2[Y] * poor2[Y]);
	if(poor2[Y]<0) poor2[R]=poor2[R]*-1;
	//theta[0] = atan2(poor2[X], poor2[Y]);
	if(poor2[X]==0)theta[0]=0;
	else theta[0] = atan2(poor2[X], poor2[Y]);
	//printf("%f\n" ,theta[0]);
	Quadrant_B();
	//theta_B = theta[1]- MY_POSI[ANGLE];	//rad
	//theta_B = ideal_robot_theta - (MY_POSI2[ANGLE]);		//rad 多分いける
	//speed_B[Y] =pid_y.PID_act2( 2 * poor2[R] / (M_W_diameter* cmt));
	//speed_B[ANGLE] = pid_yaw.PID_act2(2* L_wheel_distance * theta_B / M_W_diameter*cmt);	//目標位置と現在地の差から機体が移動する角度＋現在の機体の
	speed_B[Y] =pid_y.PID_act2(poor2[R]);
	//printf("%f,%f\n",theta[1] ,MY_POSI[ANGLE] );
	speed_B[ANGLE]=pid_yaw.PID_act(0,MY_POSI[ANGLE]);
	//printf("%f,%f\n",speed_B[Y] ,speed_B[ANGLE]);
	//angular_velocity_B = Angular_Velocity_B();
	//angular_velocity_ad = speed[Y] + speed[ANGLE];

}

double speed_control::angular_velocity_output(void) {
	return angular_velocity_F + angular_velocity_B;
}

void speed_control::Quadrant_F(void) {
	if (poor[X] >= 0 && poor[Y] >= 0) { quad = 1; }	//第一象限
	else if (poor[X] < 0 && poor[Y] >= 0) { quad = 2; }		//第ニ象限
	else if (poor[X] < 0 && poor[Y] < 0) { quad = 3; }	//第三象限
	else { quad = 4; }								//第四象限
}

void speed_control::Quadrant_B(void) {
	//printf("%d,",quad_B);
	if(poor2[X]==0&&poor2[Y]==0){
		quad_B=0;
		theta[1]=0;
	}
	else if (poor2[X] > 0 && poor2[Y] >= 0) { 
		quad_B = 1;
		theta[1] = 2 * pi - theta[0];
		if (theta[0] == 0) { theta[1] = 3 * pi / 2; }
	}	//第一象限
	else if (poor2[X] <= 0 && poor2[Y] > 0) { 
		quad_B = 2;
		theta[1] = -theta[0];
	}		//第ニ象限
	else if (poor2[X] < 0 && poor2[Y] <= 0) { 
		quad_B = 3;
		theta[1] = pi - theta[0];
		if (theta[0] == 0) { theta[1] = pi / 2; }
	}	//第三象限
	else { 
		quad_B = 4;
		theta[1] = pi - theta[0];
		if (theta[0] == 0) { theta[1] = pi; }
	}								//第四象限
}


double speed_control::Angular_Velocity_F(char WHEEL) {
	wheel=WHEEL;
	//printf("%d,",quad);
	if (wheel == RIGHT) {
		if (quad == 1) {
			//printf("%f,%f\n",speed_F[Y], speed_F[ANGLE]);
			angular_velocity_F = speed_F[Y] - speed_F[ANGLE];
		}
		else if (quad == 2) {
			angular_velocity_F = speed_F[Y] + speed_F[ANGLE];
		}
		else if (quad == 3) {
			angular_velocity_F = -speed_F[Y] + speed_F[ANGLE];
		}
		else if (quad == 4) {
			angular_velocity_F = -speed_F[Y] - speed_F[ANGLE];
		}
		else {
			angular_velocity_F = 0;
		}
	}
	else if (wheel == LEFT) {
		if (quad == 1) {
			//printf("%f,%f\n",speed_F[Y], speed_F[ANGLE]);
			angular_velocity_F = -1*(speed_F[Y] + speed_F[ANGLE]);
		}
		else if (quad == 2) {
			angular_velocity_F = -1*(speed_F[Y] - speed_F[ANGLE]);
		}
		else if (quad == 3) {
			angular_velocity_F =-1*( -1 * speed_F[Y] - speed_F[ANGLE]);
		}
		else if (quad == 4) {
			angular_velocity_F =-1*( -1 * speed_F[Y] + speed_F[ANGLE]);
		}
		else {
			angular_velocity_F = 0;
		}
	}
	//printf("%f,%f\n",speed_F[Y], speed_F[ANGLE]);
	//printf("%f,%f\n",speed_F[Y], speed_F[ANGLE]);
	return 4.3*angular_velocity_F;
}

double speed_control::Angular_Velocity_B(char WHEEL) {
	wheel=WHEEL;
	//printf("%d,",quad_B);
	//printf("%d\n,",speed_B[ANGLE]);
	quad_B=1;
	if (wheel == RIGHT) {
		if (quad_B == 1) {
			return speed_B[Y] + speed_B[ANGLE];
		}
		else if (quad_B == 2) {
			return speed_B[Y] + speed_B[ANGLE];
		}
		else if (quad_B == 3) {
			return -1 * speed_B[Y] + speed_B[ANGLE];
		}
		else if (quad_B == 4) {
			return -1 * speed_B[Y] + speed_B[ANGLE];
		}
		else {
			return 0;
		}
	}
	else if (wheel == LEFT) {
		if (quad_B == 1) {
			return -1*(speed_B[Y] - speed_B[ANGLE]);
		}
		else if (quad_B == 2) {
			return -1*(speed_B[Y] - speed_B[ANGLE]);
		}
		else if (quad_B == 3) {
			return -1*(-speed_B[Y] - speed_B[ANGLE]);
		}
		else if (quad_B == 4) {
			return -1*(-speed_B[Y] - speed_B[ANGLE]);
		}
		else {
			return 0;
		}
	}
	return 0;
}

double speed_control::angular_velocity_stop(void) {
	return 0;
}

void speed_control::sji_kasoku(float time,double distance_X,double distance_Y,char flag){
	V_max_X=distance_X*2/(time/3+time);
	V_max_Y=distance_Y*2/(time/3+time);
	if(flag==0){
	//distance_R=sqrt(distance_X*distance_X+distance_Y*distance_Y);
	
		t_point->t_posi_st_get(0,0,V_max_X,V_max_Y);	//目標位置設定(初期x,y,目標x,y)
	}
	else if(flag==1){
		
	}
	else if(flag==2){
		t_point->t_posi_st_get(V_max_X,V_max_Y,0,0);
	}
		
	//count+=0.005;
	
}
	