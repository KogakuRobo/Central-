
#include<stddef.h>
#include<machine.h>
#include"iodefine.h"
#include"task_control_block.h"
#include"syscall_table.h"

int now_tcb_number = 0;
_task_control_block TCBs[NUMBER_OF_MAX_TASK];

//引数を処理しないことで、SWINT割り込みにそのまま引数を渡すことが可能です。
int user_syscall(int num,void *attr)
{
	int_exception(VECT(ICU,SWINT));
	//ICU.SWINTR.BIT.SWINT = 1;
}

extern int kernel_create_thread(CreateThreadStruct*);
extern int kernel_destroy_thread(thread_t*);
extern int kernel_suspend_thread(thread_t*);
extern int kernel_resume_thread(thread_t*);

extern long kernel_open(const char*,long,long);
extern long kernel_write(long,const unsigned char*,long);
extern long kernel_read(long,unsigned char,long);
extern long kernel_close(long);

extern int schedule(void);					//スケジューラ
extern int context_switch(int);
extern int init(void);

int swint(int num,void *attr)
{
	int ret;
	
	switch(num){
	case SYSCALL_CREATE_THREAD:
		ret = kernel_create_thread(attr);
		break;
	case SYSCALL_DESTROY_THREAD:
		ret = kernel_destroy_thread(attr);
		break;
	case SYSCALL_SUSPEND_THREAD:
		ret = kernel_suspend_thread(attr);
		break;
	case SYSCALL_RESUME_THREAD:
		ret = kernel_resume_thread(attr);
		break;
	case SYSCALL_OPEN:
		open_stc *o_stc = attr;
		return kernel_open(o_stc->name,o_stc->mode,o_stc->flg);
	case SYSCALL_WRITE:
		write_stc *w_stc = attr;
		return kernel_write(w_stc->fileno,w_stc->buf,w_stc->count);
	case SYSCALL_READ:
		read_stc *r_stc = attr;
		return kernel_read(r_stc->fileno,r_stc->buf,r_stc->count);
	case SYSCALL_CLOSE:
		close_stc *c_stc = attr;
		return kernel_close(c_stc->fileno);
	case SYSCALL_INIT:		//初期化時スケジューラは呼ばない。
		ret = init();
		return ret;
	default:
		ret = -1;
	}
	
	int numterget = schedule();
	ret = context_switch(numterget);
	
	return now_tcb_number;
}
