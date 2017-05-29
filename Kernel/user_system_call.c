#include"user_system_call.h"
#include"syscall_table.h"
#include<stddef.h>

long open(const char *name,long mode,long flg)
{
	open_stc stc ={.name = name,.mode = mode,.flg = flg};
	return user_syscall(SYSCALL_OPEN,&stc);
}

long write(long fileno,const unsigned char *buf,long count){
	write_stc stc = {.fileno = fileno,.buf = buf,.count = count};
	return user_syscall(SYSCALL_WRITE,&stc);
}

long read(long fileno,unsigned char* buf,long count){
	read_stc stc = {.fileno=fileno,.buf = buf,.count = count};
	return user_syscall(SYSCALL_READ,&stc);
}

long close(long fileno){
	close_stc stc = {.fileno = fileno};
	return user_syscall(SYSCALL_CLOSE,&stc);
}

int thread_create(thread_t *tid,long attr,void*(*function)(thread_t *,void*),void* arg)
{
	CreateThreadStruct cts ={.tid = tid,.attr = attr,.function=function,.arg=arg};
	return user_syscall(SYSCALL_CREATE_THREAD,&cts);
	
}

int thread_destroy(thread_t *tid){
	return user_syscall(SYSCALL_DESTROY_THREAD,tid);
}

int thread_suspend(thread_t *tid){
	return user_syscall(SYSCALL_SUSPEND_THREAD,tid);
}

int thread_resume(thread_t *tid){
	return user_syscall(SYSCALL_RESUME_THREAD,tid);
}

int sys_init(void){
	return user_syscall(SYSCALL_INIT,NULL);
}
