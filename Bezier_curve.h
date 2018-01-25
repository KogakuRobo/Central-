#ifndef _Bezier_curve_H_		//ベジェ曲線
#define _Bezier_curve_H_

#include "ENUM.h"

class bezier_curve {
	double p_x[4], p_y[4];	//初期位置、制御点、制御点２、最終目標位置
	double t;	//時間　0<t<1
	double point[4];//目標位置x,y
public:
	bezier_curve(void);		//コンストラクタ
	void root_set_x(double i_posi, double s_point, double g_posi, double time);	//機体経路の設定 x座標
	void root_set_y(double i_posi, double s_point, double g_posi, double time);	//機体経路の設定 y座標
	void root_set_r(double i_posi, double s_point, double g_posi, double time);	//機体経路の設定 yaw角 微分
	float Sji(double(&sji_p)[4], float max_t);	//ｓ字曲線用　上記のtime部分
	void root_set(double(&i_posi)[2], double(&s_point)[2], double(&g_posi)[2], double time);	//機体経路の設定 
	void root_give(double POINT[4]);	//機体経路の呼び出し
	void root_st_set(double(&i_posi)[2], double(&g_posi)[2], double time);
	void root_st_give(double POINT[4]);
};
#endif
