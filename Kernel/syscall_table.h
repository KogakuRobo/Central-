#ifndef _syscall_table_H_
#define _syscall_table_H_

#include"user_system_call.h"

typedef enum{
	SYSCALL_CREATE_THREAD = 0,
	SYSCALL_DESTROY_THREAD,
	SYSCALL_OPEN,
	SYSCALL_CLOSE,
	
	SYSCALL_INIT,
}syscall_table;

//SYS_CREATE_THREADéûÇÃà¯êîç\ë¢ëÃ
struct __create_thread_struct{
	thread_t *tid;
	long	attr;
	void *(*function)(thread_t *,void*);
	void *arg;
};

typedef struct __create_thread_struct CreateThreadStruct;

#endif