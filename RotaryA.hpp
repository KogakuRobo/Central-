#ifndef _RotaryA_HPP_
#define _RotaryA_HPP_

//セントラルボード用　ロータリーエンコーダA　デバイスドライバヘッダ

//ドライバ名　"ROTARY_A"

#include "CentralLibrary.h"

typedef enum{
	//クロックレートを選択します。
	ROTARYA_SET_CLOCKRATE,
	
	//モジュールの動作を開始します。
	ROTARYA_BEGIN,
	
	//戻り値にカウントを取得します。
	ROTARYA_GET_COUNT,
	
	//TGIA割り込み時に作成するスレッドを設定します。
	ROTARYA_SET_TGIA,
}rotary_a_ioctl_cmd;

typedef struct{
	void* (*function)(thread_t*,void*);
	long attr;
	void *argp;
}interrupt_stc;

#endif