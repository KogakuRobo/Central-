#include "RotaryClass.hpp"
#include "Rotary.hpp"
#include"CentralLibrary.h"

Rotary::Rotary(const char *_n){
	name = _n;
	if(this->name != NULL){
		rotary_dev = open(name,0,0);
		ioctl(rotary_dev,ROTARY_BEGIN,NULL);
	}
	else {
		rotary_dev = -1;
	}
}

signed long Rotary::GetCount(void){
	return (signed long)ioctl(rotary_dev,ROTARY_GET_COUNT,NULL);
}