#include <iostream>
#include"Bezier_curve.h"
#include<stdio.h>
#include<stdlib.h>

bezier_curve::bezier_curve(void) {
	for (int i = 0; i <= 3; i++) {
		p_x[i] = 0;
		p_y[i] = 0;
		point[i] = 0;
	}
	t = 0;
}

void bezier_curve::root_set_x(double i_posi, double s_point, double g_posi, double time) {
	p_x[0] = i_posi;
	p_x[1] = s_point;
	p_x[2] = g_posi;
	t = time;
	point[X] = (1 - t)*(1 - t)*p_x[0] + 2 * (1 - t)*t*p_x[1] + t*t*p_x[2];
}

void bezier_curve::root_set_y(double i_posi, double s_point, double g_posi, double time) {
	p_y[0] = i_posi;
	p_y[1] = s_point;
	p_y[2] = g_posi;
	t = time;
	point[Y] = (1 - t)*(1 - t)*p_y[0] + 2 * (1 - t)*t*p_y[1] + t*t*p_y[2];
}

void bezier_curve::root_set_r(double i_posi, double s_point, double g_posi, double time) {
	p_y[0] = i_posi;
	p_y[1] = s_point;
	p_y[2] = g_posi;
	t = time;
}


void bezier_curve::root_set(double(&i_posi)[2], double(&s_point)[2], double(&g_posi)[2], double time) {
	p_x[0] = i_posi[X];
	p_x[1] = s_point[X];
	p_x[2] = g_posi[X];
	p_y[0] = i_posi[Y];
	p_y[1] = s_point[Y];
	p_y[2] = g_posi[Y];
	t = time;
	point[X] = (1 - t)*(1 - t)*p_x[0] + 2 * (1 - t)*t*p_x[1] + t*t*p_x[2];
	point[Y] = (1 - t)*(1 - t)*p_y[0] + 2 * (1 - t)*t*p_y[1] + t*t*p_y[2];
	//printf("%f,%f\n",point[X],point[Y]);
}



void bezier_curve::root_give(double POINT[4]) { 
	POINT[X] = point[X];
	POINT[Y] = point[Y];
}

float bezier_curve::Sji(double(&sji_p)[4], float time) {
	p_y[0] = sji_p[0];
	p_y[1] = sji_p[1];
	p_y[2] = sji_p[2];
	p_y[3] = sji_p[3];
	t = time;
	point[Y] = (1 - t)*(1 - t)*(1 - t)*p_y[0] + 3 * (1 - t)*(1 - t)*t*p_y[1] + 3 * (1 - t)*t*t*p_y[2] + t*t*t*p_y[3];
	return point[Y];
}

void bezier_curve::root_st_set(double(&i_posi)[2], double(&g_posi)[2], double time) {
	p_x[0] = i_posi[X];
	p_x[1] = g_posi[X];
	p_y[0] = i_posi[Y];
	p_y[1] = g_posi[Y];
	t = time;
	//printf("%f\n",g_posi[1]);
	//printf("%f\n",t);
	point[X] = (1 - t)*p_x[0] + t*p_x[1];
	point[Y] = (1 - t)*p_y[0] + t*p_y[1];
	//printf("%f",t);

}

void bezier_curve::root_st_give(double POINT[4]) {
	POINT[X] = point[X];
	//printf("%f",POINT[X]);
	//printf("%f\n",point[X]);
	POINT[Y] = point[Y];
}

