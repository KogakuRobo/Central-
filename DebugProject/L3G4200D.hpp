#ifndef _L3G4200D_HPP_
#define _L3G4200D_HPP_


#include<stdio.h>

#include"Gyro.hpp"
#include"../SPI/SPI.hpp"

#include"L3G4200D_defined.hpp"

#define NUMBER_OF_CALIBRATION	500		//キャリブレーションに用いるサンプリング数。

class L3G4200D:public Gyro{
	
	unsigned char new_data;			//更新フラグ。下二つのデータが更新されたかどうかで使う。getYawでクリア
	float yaw;				//計算角度
	signed short z;				//取得Zデータ（生）
	
	SPI_Bus *spi_bus;			//通信バスクラス
	
	struct{
		signed short average;		//キャリブレーション中の平均値
		signed short deviation;		// ・・		　　　の偏差
		signed short max;		//			の最大値
		signed short min;		//			の最小値
		signed short* p_buffer;		//偏差算出のためのサンプリング値保持バッファへのアドレス。begin内で動的確保
	}gcs;
	
	enum {
		INITIALIZE,
		CALIBRATION,
		RUN,
	}mode;					//内部状態
	
	FILE *fp;
public:
	L3G4200D(SPI_Bus&);
	int Begin(unsigned int);
	
	float getYaw(void);
	signed short getZ(void);
	signed short getAverage(void);
	signed short getDeviation(void);
	
	void Processing(void);
	
	bool isCalibration(void);
	bool isNewData(void);
private:
	int RegisterWrite(unsigned char,unsigned char *,int);
	int RegisterRead(unsigned char,unsigned char*,int);
	
	unsigned char ReadWhoAmI(void);
	void WriteCTRL_REG1(unsigned char);
	void WriteCTRL_REG2(unsigned char);
	void WriteCTRL_REG3(unsigned char);
	void WriteCTRL_REG4(unsigned char);
	void WriteCTRL_REG5(unsigned char);
	unsigned short OUT_Z(void);
	
	bool Calibration(signed short);
	signed short Filting(signed short);
};

#endif