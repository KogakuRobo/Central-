#ifndef _cmt1_HPP_
#define _cmt1_HPP_

#include "thread.h"

#define CMT1_NUMBER	16

void CMT1_CMTI1(void);

class cmt1_timer{
	
	friend void CMT1_CMI1(void);
	static thread_t thread;
	static long priority;
	static void*(*f)(thread_t*,void*);
	static void* attr;
	static int use;
public:
	cmt1_timer(void);
	thread_t* set_timer(long usec,long priority,void*(*)(thread_t*,void*),void*);
	void stop_timer(void);
	
	static bool is_action(void);
};

#endif