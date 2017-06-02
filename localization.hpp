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
}data;

extern void *localization(thread_t*,void*);

#endif