#ifndef _MY_POSITION_H_		//自己位置推定用
#define _MY_POSITION_H_

#include "localization.hpp"

class my_position {
	double m_posi[3];	//x,y,θ
	Localization *loca_my;
public:
	my_position(Localization *ploca_my);		//コンストラクタ
	void m_posi_give(double M_POSI[3]);		//自己位置渡し
	double m_posi_angle_give(void);		//自己位置渡し 角度
	double m_posi_x_give(void);		//自己位置渡し x軸
	double m_posi_y_give(void);		//自己位置渡し y軸
};

#endif
