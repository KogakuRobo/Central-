#ifndef _localization_HPP_
#define _localization_HPP_

#include "CentralLibrary.h"
//#include "Gyro.hpp"
#include "L3G4200D.hpp"

//自己位置推定用プログラム

//仕様モジュール
//CMT1	(ジャイロセンサのデータ取得周期生成）
//RSPI1　（ジャイロセンサーとの通信）

typedef struct{
	long ave;
	long devia;
	float yaw;
	
	long count_A;
	
	long count_B;
	
	long time;
	
	float X,Y;
}loca_data;

extern void* localization_init(void);
extern void *localization(thread_t*,void*);

class Localization
{
private:
	/*
	SPI_Bus *spi_bus;
	Gyro *yaw_gyro;

	int rotary_a;
	int rotary_b;
	*/
public:
	Localization(void);
	~Localization(void);
	//int Begin(void);
	
	virtual float GetX(void);			//[m]
	virtual float GetY(void);			//[m]
	virtual float GetYaw(void);			//[rad]
	
	volatile loca_data &Get_d(void);
};

#endif