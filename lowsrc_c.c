#include"user_system_call.h"
#include"syscall_table.h"

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