#include"user_system_call.h"
#include"syscall_table.h"
#include<stddef.h>

extern "C"{
long open(const char *name,long mode,long flg)
{
	open_stc stc ={name,mode,flg};
	return user_syscall(SYSCALL_OPEN,&stc);
}

long write(long fileno,const unsigned char *buf,long count){
	write_stc stc = {fileno,buf,count};
	return user_syscall(SYSCALL_WRITE,&stc);
}

long read(long fileno,unsigned char* buf,long count){
	read_stc stc = {fileno,buf,count};
	return user_syscall(SYSCALL_READ,&stc);
}

long close(long fileno){
	close_stc stc = {fileno};
	return user_syscall(SYSCALL_CLOSE,&stc);
}

long ioctl(long fileno,unsigned long request,void *argp)
{
	ioctl_stc stc = {fileno,request,argp};
	return user_syscall(SYSCALL_IOCTL,&stc);
}
}

int thread_create(thread_t *tid,long attr,void*(*function)(thread_t *,void*),void* arg)
{
	CreateThreadStruct cts ={tid,attr,function,arg};
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

int msleep(unsigned long t){
	return user_syscall(SYSCALL_TIMER_MSLEEP,(void*)t);
}

int sys_init(void){
	return user_syscall(SYSCALL_INIT,NULL);
}
