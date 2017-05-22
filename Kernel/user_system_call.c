#include"user_system_call.h"
#include"syscall_table.h"
#include<stddef.h>

extern int now_tcb_number;

int thread_create(thread_t *tid,long attr,void*(*function)(thread_t *,void*),void* arg)
{
	CreateThreadStruct cts ={.tid = tid,.attr = attr,.function=function,.arg=arg};
	return user_syscall(SYSCALL_CREATE_THREAD,&cts);
	
}

int thread_destroy(thread_t *tid){
	if(tid == NULL)tid->num = now_tcb_number;
	return user_syscall(SYSCALL_DESTROY_THREAD,tid);
}

int sys_init(void){
	return user_syscall(SYSCALL_INIT,NULL);
}