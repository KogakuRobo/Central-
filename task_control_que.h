#ifndef _task_control_que_H_
#define _task_control_que_H_

#include "task_control_block.h"

typedef struct _task_que{
	struct _task_que *prev,*next;
	thread_t *tid;
	struct{
		unsigned char use:1;
		
	}Flags;
}TaskQue;

typedef TaskQue TaskQueHead;

int enque(TaskQueHead*,TaskQue*);		//後方エンキュー
int priority_enque(TaskQueHead*,TaskQue*);	//優先度参照型エンキュー
int deque(TaskQueHead*,TaskQue*);

int get_que(TaskQueHead*,int num,TaskQue*);

#endif