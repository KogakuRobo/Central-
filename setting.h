#ifndef _SETTING_H_		//defineによる定義
#define _SETIING_H_

#define pi 3.14159265359 
#define squa_two 1.41421356237
#define M_W_diameter 0.12 //メインタイヤの直径
#define S_W_diameter 0.05 //ロータリー用タイヤの直径[m]
#define L_wheel_distance 0.4				//xロータリから左右の主タイヤの間隔[m]
#define L_wheel_distance_front 0.83058			//xロータリから前左右タイヤの中心まで0.76[m] 
#define cmt 0.01 //周期
#define cmt1 1
#define cmt2 1


enum {
	x, y, r
};
///ロータリー///
#define pulse  (500*4)		
#define R_per_p  ((2*pi)/pulse)			//ラジアン　/　パルス
#define S_wheel_distance 1				//ロータリー用タイヤの間隔(mm)
#define S_W_mileage_per_p (S_W_diameter*pi/ pulse)		//1pulseあたりの走行距離(mm)

///入出力用///

#endif 

