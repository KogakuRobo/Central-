#ifndef _thread_H_
#define _thread_H_

#include "../que.hpp"

#define CT_PRIORITY_MAX 0x00000000
#define CT_PRIORITY_MIN 0x000000ff

#define CT_PRIORITY(x) 	x & 0x000000ff

#define CT_READY	0x00000000
#define CT_WAIT		0x00000100
#define CT_RUN		0x00000200

#define CT_DESTROY	0x00000000
#define CT_RESTART	0x00001000

typedef union{
	long L;
	struct{
		unsigned char priority:8;
		unsigned char create_mode:4;
		unsigned char end_function:4;
	}BIT;
}CreateAttribute;

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
	
	void *(*function)(struct __tcb *,void*);
	void *arg;

	int stack_num;
	
	unsigned char priority;
	TASK_STATE state;
	
	struct{
		unsigned char use:1;
	}Flags;
}_task_control_block;


typedef _task_control_block thread_t;

typedef list<thread_t*,3> thread_list;
typedef list<thread_t*,3>::iterator thread_iterator;

extern int thread_create(thread_t*,long,void*(*)(thread_t*,void*),void*);
extern int thread_wakeup(thread_t*);
extern int thread_destroy(thread_t*);
extern int thread_suspend(thread_t*);
extern int thread_resume(thread_t*);

extern thread_t* get_tid(void);

#endif