#ifndef _RotaryA_HPP_
#define _RotaryA_HPP_

#include "CentralLibrary.h"

typedef enum{
	ROTARYA_SET_CLOCKRATE,
	
	ROTARYA_BEGIN,
	
	ROTARYA_GET_COUNT,
	
	ROTARYA_SET_TGIA,
};

typedef struct{
	void* (*function)(thread_t*,void*);
	long attr;
	void *argp;
}interrupt_stc;

#endif