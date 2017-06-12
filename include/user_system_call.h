#ifndef _user_system_call_H_
#define _user_system_call_H_


#include"thread.h"

int user_syscall(int,void*);

extern "C"{
long open(const char *name,long mode,long flg);
long write(long fileno,const unsigned char *buf,long count);
long read(long fileno,unsigned char* buf,long count);
long close(long fileno);
long ioctl(long fileno,unsigned long request,void *argp);
}

int sys_init(void);

int msleep(unsigned long);

#endif