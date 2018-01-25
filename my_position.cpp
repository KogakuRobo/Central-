#include<iostream>
#include"my_position.h"
#include"ENUM.h"

my_position::my_position(Localization *ploca_my) {
	m_posi[0] = 0, m_posi[1] = 0, m_posi[2] = 0;
	loca_my=ploca_my;
}

/*void my_position::m_posi_set(void) {
	m_posi[X] = loca_my->GetX();
	m_posi[Y] = loca_my->GetY();
	m_posi[ANGLE] = loca_my->GetYaw();
}*/

void my_position::m_posi_give(double M_POSI[3]) {
	M_POSI[X] = loca_my->GetX();
	M_POSI[Y] = loca_my->GetY();
	M_POSI[ANGLE] = loca_my->GetYaw();
}

double my_position::m_posi_angle_give(void) {
	return loca_my->GetYaw();
}

double my_position::m_posi_x_give(void){
	return loca_my->GetX();
}

double my_position::m_posi_y_give(void){
	return loca_my->GetY();
}