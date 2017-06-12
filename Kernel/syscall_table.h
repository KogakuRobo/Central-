#ifndef _syscall_table_H_
#define _syscall_table_H_

#include "thread.h"
#include"user_system_call.h"

typedef enum{
	SYSCALL_CREATE_THREAD = 0,
	SYSCALL_DESTROY_THREAD,
	SYSCALL_OPEN,
	SYSCALL_WRITE,
	SYSCALL_READ,
	SYSCALL_CLOSE,
	SYSCALL_IOCTL,
	
	SYSCALL_SUSPEND_THREAD,
	SYSCALL_RESUME_THREAD,
	
	SYSCALL_TIMER_MSLEEP,
	
	SYSCALL_INIT,
}syscall_table;

//SYS_CREATE_THREADéûÇÃà¯êîç\ë¢ëÃ
struct __create_thread_struct{
	thread_t *tid;
	CreateAttribute	attr;
	void *(*function)(thread_t *,void*);
	void *arg;
};

typedef struct{
	const char *name;
	long mode;
	long flg;
}open_stc;

typedef struct{
	long fileno;
	const unsigned char *buf;
	long count;
}write_stc;

typedef struct{
	long fileno;
	unsigned char *buf;
	long count;
}read_stc;

typedef struct{
	long fileno;
}close_stc;

typedef struct{
	long fileno;
	unsigned long request;
	void *argp;
}ioctl_stc;

typedef struct __create_thread_struct CreateThreadStruct;

#endif