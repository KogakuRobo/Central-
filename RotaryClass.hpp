#ifndef _RotaryClass_HPP_
#define _RotaryClass_HPP_

#include <stddef.h>

class Rotary{
	int rotary_dev;
	const char *name;
	
public:
	Rotary(const char * = NULL);
	virtual signed long GetCount(void);
};

#endif