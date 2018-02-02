#ifndef _thread_mutex_HPP_
#define _thread_mutex_HPP_

#include "CentralLibrary.h"

class mutex{
	
	int _lock;

public:
	typedef enum{
	}attr_type;
private:
	attr_type attr;

public:
	typedef enum{
	}lock_return_code;
	
	typedef enum{
	}unlock_return_code;
	
public:
	mutex(void);
	lock_return_code 	lock(void);
	unlock_return_code 	unlock(void);
};

#endif