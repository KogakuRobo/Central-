#ifndef _RotaryClass_HPP_
#define _RotaryClass_HPP_

#include <stddef.h>
#include "RotaryA.hpp"

class Rotary{
	int rotary_dev;
	const char *name;
	
public:
	Rotary(const char * = NULL);
	virtual int SetTGIA(interrupt_stc);
	virtual long GetCount(void);
};

#endif