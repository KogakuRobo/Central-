#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include"target_point.h"
#include"setting.h"

target_point::target_point(void) {
	for (int i = 0; i <= 1; i++) {
		t_posi[i] = 0;
		i_posi[i]=0;
		s_point[i]=0;
		g_posi[i]=0;
	}
	time = 0;
	Sji_p[0]=0;
	Sji_p[1]=0;
	Sji_p[2]=0;
	Sji_p[3]=0;
	Sji_time=0;
}

void target_point::t_posi_set(int flog) {
	switch (flog) {
	case 0:
		bezi.root_st_set(i_posi, g_posi, sji_time());
		bezi.root_st_give(t_posi);
		//printf("%f\n",t_posi[Y]);
		break;
	case 1:
		bezi.root_set(i_posi,s_point, g_posi, sji_time());
		bezi.root_give(t_posi);
		break;
	case 2:
		break;
	}
}

float target_point::sji_time(void) {
	//if (Sji_time < 1.00) {
		//printf("%ftt\n",Sji_time);
		//Sji_time += cmt0;
		
	/*}
	else {
		Sji_time = 1.00;
	}*/
	time=bezi.Sji(Sji_p, Sji_time);
	//printf("%foo\n",time);
	return time;

}
void target_point::t_posi_give(double T_POSI[2]) {
	T_POSI[X] = t_posi[X];
	//printf("%f\n",T_POSI[X]);
	T_POSI[Y] = t_posi[Y];
}

void target_point::t_posi_get(double I_POSI_x, double I_POSI_y, double S_POSI_x, double S_POSI_y, double G_POSI_x, double G_POSI_y) {
	i_posi[X] = I_POSI_x;
	i_posi[Y] = I_POSI_y;
	s_point[X] = S_POSI_x;
	s_point[Y] = S_POSI_y;
	g_posi[X] = G_POSI_x;
	g_posi[Y] = G_POSI_y;
	
}

void target_point::t_posi_st_get(double I_POSI_x, double I_POSI_y, double G_POSI_x, double G_POSI_y) {
	i_posi[X] = I_POSI_x;
	i_posi[Y] = I_POSI_y;
	//printf("%f\n",g_posi[X]);
	g_posi[X] = G_POSI_x;
	g_posi[Y] = G_POSI_y;
	
}

void target_point::Sji_get(double S1, double S2, double S3,double S4,double S_t) {
	Sji_p[0] = S1;
	Sji_p[1] = S2;
	Sji_p[2] = S3;
	Sji_p[3] = S4;
	Sji_time=S_t;
	//printf("%f\n",Sji_time);
}