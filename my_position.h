#ifndef _MY_POSITION_H_		//���Ȉʒu����p
#define _MY_POSITION_H_

#include "localization.hpp"

class my_position {
	double m_posi[3];	//x,y,��
	Localization *loca_my;
public:
	my_position(Localization *ploca_my);		//�R���X�g���N�^
	void m_posi_give(double M_POSI[3]);		//���Ȉʒu�n��
	double m_posi_angle_give(void);		//���Ȉʒu�n�� �p�x
	double m_posi_x_give(void);		//���Ȉʒu�n�� x��
	double m_posi_y_give(void);		//���Ȉʒu�n�� y��
};

#endif
