#ifndef _RotaryA_HPP_
#define _RotaryA_HPP_

//セントラルボード用　ロータリーエンコーダA　デバイスドライバヘッダ

//ドライバ名　"ROTARY_A"

#include "CentralLibrary.h"
#include "Rotary.hpp"

typedef enum{
	//TGIA割り込み時に作成するスレッドを設定します。
	ROTARYA_SET_TGIA = ROTARY_IOCTL_LAST,
};

typedef struct{
	void* (*function)(thread_t*,void*);
	long attr;
	void *argp;
}interrupt_stc;

#endif