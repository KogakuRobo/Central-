#include <stddef.h>
#include "task_control_block.h"
#include "task_control_que.h"
#include "syscall_table.h"

#define NUMBER_OF_MAX_QUE 5

extern int now_tcb_number;
extern _task_control_block TCBs[NUMBER_OF_MAX_TASK];

TaskQueHead ReadyQueHead;
TaskQue Que[NUMBER_OF_MAX_QUE];


void user_kernel_des(void);
int kernel_ready_thread(thread_t *);
void* idle_task(thread_t*,void*);

int init(void){
	const int MAIN_TCB = 0;
	const int IDLE_TCB = 1;
	static thread_t main;
	static thread_t idle;
	
	for(int que_node = 0;que_node < NUMBER_OF_MAX_QUE;que_node++){
		Que[que_node].Flags.use = 0;Que[que_node].tid = NULL;
	}

//レディキューなし
	ReadyQueHead.prev = ReadyQueHead.next = &ReadyQueHead;
	
//メインタスク用初期設定
//すでに動作しているこのタスク
//メインタスクのregは退避するときはじめて初期化されます。
	TCBs[MAIN_TCB].Flags.use = 1;
	TCBs[MAIN_TCB].state = TASK_RUN;				//メインスレッド
	TCBs[MAIN_TCB].priority = 0x0f;
	
	TCBs[MAIN_TCB].tid = &main;
	main.p_TCB = &TCBs[MAIN_TCB];

//アイドルタスク生成
//スリープモードに移行するために、タスクはスーパーバイザーモードで起動
	thread_create(&idle,CT_PRIORITY_MIN,idle_task,NULL);
	((_task_control_block*)idle.p_TCB)->reg.PSW = 0x00000000;

	return 0;
}

int kernel_create_thread(CreateThreadStruct *cts){
	_task_control_block *terget;
	int free_tcb;
	
	for(free_tcb = 0;free_tcb < NUMBER_OF_MAX_TASK;free_tcb++){
		if(TCBs[free_tcb].Flags.use == 0)break;
	}
	if(free_tcb == NUMBER_OF_MAX_TASK)return -1;
	
	terget 				= &TCBs[free_tcb];
	
	terget->reg.PC 			= (unsigned long)cts->function;
	terget->reg.USP 		= (long)(__secend("SU")) - SIZE_OF_USER_STACK * free_tcb - 4;
	terget->reg.ISP 		= (long)(__secend("SI")) - SIZE_OF_KERNEL_STACK * free_tcb;
	*(long *)(terget->reg.USP) 	= (long)user_kernel_des;
	terget->reg.PSW			= 0x00130000;
	
	terget->reg.GREG[0] 		= (long)cts->tid;
	terget->reg.GREG[1] 		= (long)cts->arg;
	
	terget->function 		= cts->function;
	terget->arg 			= cts->arg;
	terget->priority 		= (cts->attr & 0x000000ff);
	terget->state 			= TASK_READY;
	terget->tid 			= cts->tid;
	terget->Flags.use 		= 1;
	terget->num 			= free_tcb;
	
	cts->tid->p_TCB 		= terget;
	
	if(kernel_ready_thread(cts->tid) != 0)return -1;
	
	return 0;
}

int kernel_ready_thread(thread_t *tid){
	int free_que;
	
	TaskQue *node 			= ReadyQueHead.next;
	_task_control_block *terget 	= (_task_control_block *)tid->p_TCB;
	 
	for(free_que = 0;free_que < NUMBER_OF_MAX_QUE;free_que++){
		if(Que[free_que].Flags.use == 0)break;
	}
	if(free_que == NUMBER_OF_MAX_QUE)return -1;
	
	Que[free_que].Flags.use 	= 1;
	
	while(((_task_control_block *)(node->tid->p_TCB))->priority < (terget->priority)){
		node = node->next;
		if(node == &ReadyQueHead)break;
	}
	
	node->prev->next 	= &Que[free_que];
	Que[free_que].next 	= node;
	Que[free_que].prev 	= node->prev;
	node->prev 		= &Que[free_que];
	
	terget->state 			= TASK_READY;
	
	tid->p_Que = &Que[free_que];
	Que[free_que].tid = tid;
	
	return 0;
}

int kernel_suspend_thread(thread_t *tid){
	if(tid == NULL)tid = TCBs[now_tcb_number].tid;
	((_task_control_block *)(tid->p_TCB))->state = TASK_WAIT;
	
	return 0;
}

int kernel_resume_thread(thread_t *tid){
	if(tid == NULL)tid = TCBs[now_tcb_number].tid;
	kernel_ready_thread(tid);
	return 0;
}

int kernel_destroy_thread(thread_t *tid){
	if(tid == NULL)tid = TCBs[now_tcb_number].tid;
	((_task_control_block *)(tid->p_TCB))->state = TASK_NON;
	((_task_control_block *)(tid->p_TCB))->Flags.use = 0;
	return 0;
}

void user_kernel_des(void){
	thread_destroy(NULL);
}

int schedule(void)
{
	
	if(TCBs[now_tcb_number].state != TASK_RUN){
		if(ReadyQueHead.next == &ReadyQueHead){
			//スリープモードへ移行
		}
		else{
			return ((_task_control_block *)(ReadyQueHead.next->tid->p_TCB))->num;
		}
	}
	
	else if(TCBs[now_tcb_number].priority > ((_task_control_block *)(ReadyQueHead.next->tid->p_TCB))->priority)
		return ((_task_control_block *)(ReadyQueHead.next->tid->p_TCB))->num;
	
	else return now_tcb_number;
	return -1;
}

extern int _switch_to(_task_control_block *prev,_task_control_block *next);

int context_switch(int num){
	if(now_tcb_number == num)return 0;
	//タスクのCPU情報退避
	switch(TCBs[now_tcb_number].state){		//前のカーネルが今どのような状況であるかによって、退避方法を変える
	case TASK_NON:
		break;
	case TASK_WAIT:
		break;
	case TASK_RUN:
		kernel_ready_thread(TCBs[now_tcb_number].tid);
		break;
	}
	
	TaskQue *run = (TaskQue*)TCBs[num].tid->p_Que;
	run->prev->next = run->next;
	run->next->prev = run->prev;
	run->Flags.use = 0;
	TCBs[num].state = TASK_RUN;
	
	int befor_tcb_num = now_tcb_number;
	now_tcb_number = num;
	
	return _switch_to(&TCBs[befor_tcb_num],&TCBs[now_tcb_number]);
}

int get_tid(thread_t **_tid){
	*_tid = TCBs[now_tcb_number].tid;
	return 0;
}
