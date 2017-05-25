#ifndef _task_control_block_H_
#define _task_control_block_H_

#include "thread.h"

#define NUMBER_OF_MAX_TASK 16
#define SIZE_OF_STACK	0x400

typedef enum{
	TASK_NON,		//���������^�X�N
	TASK_READY,		//�������I���^�X�N
	TASK_RUN,		//���s�i�X�P�W���[���҂��j�^�X�N
	TASK_WAIT,		//�C�x���g�҂��^�X�N
}TASK_STATE;

typedef struct esc_reg{
	unsigned long GREG[15];
	unsigned long FPSW;
	unsigned long USP;
	unsigned long PC;
	unsigned long PSW;
}esc_reg,*p_esc_reg;

typedef struct __tcb{
	esc_reg reg;
	
	void *(*function)(thread_t *,void*);
	void *arg;

	unsigned char priority;
	TASK_STATE state;
	
	thread_t *tid;
	
	struct{
		unsigned char use:1;
		
	}Flags;
	
}_task_control_block;

#endif