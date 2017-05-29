#ifndef _task_control_block_H_
#define _task_control_block_H_

#include "thread.h"

#define NUMBER_OF_MAX_TASK 	6
#define SIZE_OF_USER_STACK	0x400
#define SIZE_OF_KERNEL_STACK	0x400

typedef enum{
	TASK_NON,		//未初期化タスク
	TASK_READY,		//初期化終了タスク
	TASK_RUN,		//実行（スケジュール待ち）タスク
	TASK_WAIT,		//イベント待ちタスク
}TASK_STATE;

typedef struct esc_reg{
	unsigned long ISP;
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
	
	int num;
}_task_control_block;

#endif