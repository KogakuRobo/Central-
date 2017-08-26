#ifndef _localization_HPP_
#define _localization_HPP_

#include "CentralLibrary.h"

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
}data;

//class Localization

extern volatile data d;

extern void* localization_init(void);
extern void *localization(thread_t*,void*);

#endif