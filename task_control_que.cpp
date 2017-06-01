#include "task_control_que.h"

int enque(TaskQueHead *head,TaskQue *que)
{
	head->prev->next = que;
	que->next = head;
	que->prev = head->prev;
	head->prev = que;
	
	return 0;
}

int priority_enque(TaskQueHead *head,TaskQue *que)
{
	_task_control_block *terget_tcb;
	TaskQue *node 			= head->next;
	
	terget_tcb = (_task_control_block *)que->tid->p_TCB;
	
	while(((_task_control_block *)(node->tid->p_TCB))->priority < (terget_tcb->priority)){
		node = node->next;
		if(node == head)break;
	}
	node->prev->next 	= que;
	que->next 		= node;
	que->prev 		= node->prev;
	node->prev 		= que;
	
	return 0;
}

int deque(TaskQueHead *head,TaskQue *que)
{
	
}

