#include<stddef.h>
#include"task_control_block.h"
#include"syscall_table.h"

extern int now_tcb_number;
extern _task_control_block TCBs[NUMBER_OF_MAX_TASK];

typedef struct _task_que{
	struct _task_que *prev,*next,*head,*end;
	_task_control_block *tcb;
	int num;
}TaskQue;

TaskQue ReadyQueHead;
TaskQue Que[NUMBER_OF_MAX_TASK];

void user_kernel_des(void);
int kernel_ready_thread(thread_t *);

int init(void){
	int tcb_node;
	static thread_t main;
	
	for(tcb_node = 0;tcb_node < NUMBER_OF_MAX_TASK;tcb_node++){
		TCBs[tcb_node].Flags.use = 0;
		TCBs[tcb_node].reg.USP = (long)(__secend("SU")) - SIZE_OF_STACK * tcb_node - 4;
		*(long *)(TCBs[tcb_node].reg.USP) = (long)user_kernel_des;
		TCBs[tcb_node].reg.PSW = 0x00130000;
		TCBs[tcb_node].reg.FPSW = 0x00000100;
		TCBs[tcb_node].arg = NULL;
		TCBs[tcb_node].state = TASK_NON;
		TCBs[tcb_node].priority = 0;
	}
	
//メインタスク用初期設定
//すでに動作しているこのタスク
	TCBs[0].Flags.use = 1;
	TCBs[0].state = TASK_RUN;				//メインスレッド
	TCBs[0].priority = 255;
	main.num = 0;
	TCBs[0].tid = &main;
	
//レディキューなし
	ReadyQueHead.prev = ReadyQueHead.next = ReadyQueHead.end = ReadyQueHead.head = &ReadyQueHead;
	return 0;
}

int kernel_create_thread(CreateThreadStruct *cts){
	_task_control_block *terget;
	int free_tcb;
	
	for(free_tcb = 0;free_tcb < NUMBER_OF_MAX_TASK;free_tcb++){
		if(TCBs[free_tcb].Flags.use == 0)break;
	}
	if(free_tcb == NUMBER_OF_MAX_TASK)return -1;
	
	
	terget = &TCBs[free_tcb];
	terget->function = cts->function;
	terget->reg.PC = (unsigned long)terget->function;
	terget->arg = cts->arg;
	terget->priority = (cts->attr & 0x000000ff);
	terget->Flags.use = 1;
	terget->tid = cts->tid;
	cts->tid->num = free_tcb;
	
	terget->reg.GREG[0] = (long)cts->tid;
	terget->reg.GREG[1] = (long)cts->arg;
	kernel_ready_thread(cts->tid);
	
	Que[free_tcb].tcb = &TCBs[free_tcb];
	TCBs[free_tcb].Flags.use = 1;
	return 0;
}

int kernel_ready_thread(thread_t *tid){
	int num = tid->num;
	 TaskQue *node = ReadyQueHead.next;
	
	while(node->tcb->priority < TCBs[num].priority){
		node = node->next;
		if(node == &ReadyQueHead)break;
	}
	TCBs[num].state = TASK_READY;
	
	node->prev->next = &Que[num];
	Que[num].next = node;
	Que[num].prev = node->prev;
	node->prev = &Que[num];
	Que[num].num = num;
	return 0;
}

int kernel_destroy_thread(thread_t *tid){
	int num = tid->num;
	TCBs[num].state = TASK_NON;
	TCBs[num].Flags.use = 0;
	return 0;
}

void user_kernel_des(void){
	thread_destroy(NULL);
}

int schedule(void)
{
	
	if(Que[now_tcb_number].tcb->state != TASK_RUN){
		if(ReadyQueHead.next == &ReadyQueHead){
			//スリープモードへ移行
		}
		else{
			return ReadyQueHead.next->num;
		}
	}
	
	else if(TCBs[now_tcb_number].priority > ReadyQueHead.next->tcb->priority)return ReadyQueHead.next->num;
	
	else return now_tcb_number;
	return -1;
}

//SWINT割り込み時に退避されたCPUレジスタ。low_swint.src参照
extern p_esc_reg swint_esc_reg;

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
	TCBs[now_tcb_number].reg = *swint_esc_reg;
	
	*swint_esc_reg = TCBs[num].reg;
	Que[num].prev->next = Que[num].next;
	Que[num].next->prev = Que[num].prev;
	TCBs[num].state = TASK_RUN;
	
	now_tcb_number = num;
	
	return 0;
}