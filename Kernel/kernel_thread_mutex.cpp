#include "CentralLibrary.h"
#include "kernel_thread_mutex.hpp"
#include "thread_control.hpp"
#include "task_que.hpp"

long kernel_thread_mutex_init(void *attr)
{
	mutex_init_t *init_st = (mutex_init_t*)attr;
	
	init_st->mid->_lock = 0;
	return 0;
}

long kernel_thread_mutex_lock(void *attr)
{
	mutex_t *mid = (mutex_t*)attr;
	set_atomic();
	
	unset_atomic();
}

long kernel_thread_mutex_unlock(void *attr)
{
	mutex_t *mid = (mutex_t*)attr;
	set_atomic();
	unset_atomic();
}
