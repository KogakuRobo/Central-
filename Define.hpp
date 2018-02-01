#ifndef _DEFINE_HPP_
#define _DEFINE_HPP_

//#include <iodefine.h>


//割り込みとか
#define cmt0 /*((48000000 / (8 * pow(4, CMT0.CMCR.BIT.CKS))) / CMT0.CMCOR)*/100
#define pulse  (500*4)		
#define R_per_p  ((2*pi)/pulse)					//ラジアン　/　パルス
#define pi 3.14159265359
#define S_wheel_radius	0.01//ロータリー用タイヤ半径
#define S_wheel_distance 0.07	//タイヤ間距離
#define arrive_time	0//目標座標への到達時間

//速度リミッター//
#define RIMIT 12

//デューティ//
#define DUTY_1		MTU6.TGRB
#define DUTY_2		MTU4.TGRB
#define DUTY_3		MTU9.TGRB
#define DUTY_4		MTU10.TGRB

//ロータリーエンコーダ//
#define RO_1 		MTU8.TCNT//ロータリーカウントに使用
#define RO_2 		MTU7.TCNT
#define RO_3 		MTU2.TCNT
#define RO_4 		MTU1.TCNT

//ＰＩＤ制御//
#define KP	0.1
#define KI	0
#define KD	0
#define DELTA_T	0.01

//割り込み//
#define cmt1	100

enum root{str, right, left, back, s_right, s_left, b_right, b_left, stop};
enum {ro_1, ro_2, ro_3, ro_4};

//グローバル関数//
extern int g_rotary_count[4];//ロータリーカウント
extern int  g_check[4];//
extern double V_now;


#endif