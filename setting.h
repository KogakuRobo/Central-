#ifndef _SETTING_H_		//defineによる定義
#define _SETIING_H_

#define pi 3.14159265359 
#define M_W_diameter 0.06 //メインタイヤの直径
#define S_W_diameter 0.15 //ロータリー用タイヤの直径[m]
#define L_wheel_distance 0.1908	//機体から主タイヤの間隔[m]
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

