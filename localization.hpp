#ifndef _localization_HPP_
#define _localization_HPP_

#include "CentralLibrary.h"
//#include "Gyro.hpp"
#include "L3G4200D.hpp"
#include "RotaryClass.hpp"

//自己位置推定用プログラム

//仕様モジュール
//CMT1	(ジャイロセンサのデータ取得周期生成）
//RSPI1　（ジャイロセンサーとの通信）

class Localization
{
private:
	
	SPI_Bus *spi_bus;
	L3G4200D *yaw_gyro;

	Rotary* rotaryc_a;
	Rotary* rotaryc_b;
	
	float yaw;
	long count_A;
	long count_B;
	
	float X;
	float Y;
	
	long encorder_ppr;
	
	//parameter_K :エンコーダのカウント差分から取り付けられたタイヤの接線速度を算出するための変数 ,1パルス当たりの距離「ｍ」
	//parameter_K = r * pi / ( 2 * PPR)
	//r: タイヤ半径[m]
	//pi : 円周率
	//PPR : エンコーダのパルス数[pulse per revolve]
	// = 0.024[m] * 3.1415926535 / ( 2 * 1024)
	float parameter_K;
	
	static void* localization(thread_t*,void*);
public:
	Localization(void);
	~Localization(void);
	int Begin(void);
	
	virtual float GetX(void);			//[m]
	virtual float GetY(void);			//[m]
	virtual float GetYaw(void);			//[rad]
	
	void set_encorder_ppr(long p);
};

#endif