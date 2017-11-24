#ifndef _cmt3_HPP_
#define _cmt3_HPP_

#include "thread.h"

#define CMT3_NUMBER 16

void CMT3_CMTIt(void);

class cmt3_timer{
	friend void CMT3_CMI3(void);
	
	static thread_t thread;
	static long priority;
	static void*(*f)(thread_t*,void*);
	static void* attr;
	static int use;
public:
	cmt3_timer(void);
	thread_t* set_timer(long usec,long priority,void*(*)(thread_t*,void*),void*);
	void stop_timer(void);
	
	static bool is_action(void);
};

#endif