#ifndef _cmt2_HPP_
#define _cmt2_HPP_

#include "thread.h"

#define CMT2_NUMBER 16

void CMT2_CMTI2(void);

class cmt2_timer{
	friend void CMT2_CMI2(void);
	
	static thread_t thread;
	static long priority;
	static void*(*f)(thread_t*,void*);
	static void* attr;
	static int use;
public:
	cmt2_timer(void);
	thread_t* set_timer(long usec,long priority,void*(*)(thread_t*,void*),void*);
	void stop_timer(void);
	
	static bool is_action(void);
};

#endif