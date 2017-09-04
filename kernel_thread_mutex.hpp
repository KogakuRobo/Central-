#ifndef _kernel_thread_mutex_HPP_
#define _kernel_thread_mutex_HPP_

#include "task_que.hpp"

struct __thread_mutex{
	int _lock;
	task_list mutex_list;
};

typedef struct __thread_mutex mutex_t;

typedef struct {
	mutex_t *mid;
	long attr;
}mutex_init_t;	
long kernel_thread_mutex_init(void*);

long kernel_thread_mutex_lock(void*);
long kernel_thread_mutex_unlock(void*);

#endif