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

int enque(TaskQueHead*,TaskQue*);		//����G���L���[
int priority_enque(TaskQueHead*,TaskQue*);	//�D��x�Q�ƌ^�G���L���[
int deque(TaskQueHead*,TaskQue*);

int get_que(TaskQueHead*,int num,TaskQue*);

#endif