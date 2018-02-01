#ifndef _ENUM_HPP_
#define _ENUM_HPP_

	enum rotary { LEFT, RIGHT };
	enum Position { X, Y, radian };
	enum Position_Rotary {X_Rotary, Y_Rotary, radian_Rotary };
//	enum Position_RGB {X_RGB=0,Y_RGB=1,radian_RGB=2,High=0,Low=1 };
	enum Run_Phase	{WAIT, START, AT_RUNNING, THROUGHING, CHANGE, TEST};//スタート→左折→スローイングゾーンへ→停止
	enum Root{TZ_1, TZ_2, TZ_3};	//各ＴＺに行くルート取り
	enum Bezier_Co{P0, P1, P2, P3};	//制御点（ベジェ）
	enum Bezier_Po{PX, PY};		//時刻tでの座標（ベジェ）
	enum dir {RO1, RO2, RO3, RO4};
	enum Direction{TO_TZ, TO_SZ};	//多分使わない（ＴＺに行くかＳＺに戻るか）
	
#endif