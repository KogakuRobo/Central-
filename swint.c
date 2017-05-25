
#include<stddef.h>
#include<machine.h>
#include"iodefine.h"
#include"task_control_block.h"
#include"syscall_table.h"

int now_tcb_number = 0;
_task_control_block TCBs[NUMBER_OF_MAX_TASK];

//SWINT���荞�ݎ��ɑޔ����ꂽCPU���W�X�^�Blow_swint.src�Q��
p_esc_reg swint_esc_reg;

//�������������Ȃ����ƂŁASWINT���荞�݂ɂ��̂܂܈�����n�����Ƃ��\�ł��B
int user_syscall(int num,void *attr)
{
	int_exception(VECT(ICU,SWINT));
	return 0;
}

extern int kernel_create_thread(CreateThreadStruct*);
extern int kernel_destroy_thread(thread_t*);
extern int schedule(void);					//�X�P�W���[��
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
	case SYSCALL_OPEN:
		break;
	case SYSCALL_INIT:		//���������̂݃X�P�W���[���͌Ă΂Ȃ��B
		ret = init();
		return ret;
	default:
		ret = -1;
	}
	
	int numterget = schedule();
	ret = context_switch(numterget);
	
	return ret;
}
