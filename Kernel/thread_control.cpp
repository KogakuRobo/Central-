#include <stddef.h>
#include <machine.h>
#include "task_control_block.h"
#include "task_que.hpp"
#include "syscall_table.h"

static thread_t* current_thread;

static thread_t thread_main;
static thread_t thread_idle;

task_list ready_list;

void user_kernel_des(void);
int kernel_ready_thread(thread_t *);
void* idle_task(thread_t*,void*);

struct{
	long su_addr;
	long si_addr;
	struct{
		unsigned char use:1;
	}flags;
}stack_addr[NUMBER_OF_MAX_TASK];

int init(void){

	for(int i = 0;i < NUMBER_OF_MAX_TASK;i++){
		stack_addr[i].su_addr = (long)(__secend("SU")) - SIZE_OF_USER_STACK * i - 4;
		stack_addr[i].si_addr = (long)(__secend("SI")) - SIZE_OF_KERNEL_STACK * i;
		stack_addr[i].flags.use = 0;
	}
	
//メインタスク用初期設定
//すでに動作しているこのタスク
//メインタスクのregは退避するときはじめて初期化されます。
	thread_main.Flags.use = 1;
	thread_main.state = TASK_RUN;				//メインスレッド
	thread_main.priority = 0x0f;
	thread_main.stack_num = 0;
	stack_addr[0].flags.use = 1;
	
	current_thread = &thread_main;
	
//アイドルタスク生成
//スリープモードに移行するために、タスクはスーパーバイザーモードで起動
	thread_create(&thread_idle,CT_PRIORITY_MIN,idle_task,NULL);
	thread_idle.reg.PSW = 0x00000000;

	return 0;
}

int kernel_create_thread(CreateThreadStruct *cts){

	int free_stack;
	_task_control_block *terget = cts->tid;
	for(free_stack = 0;free_stack < NUMBER_OF_MAX_TASK;free_stack++){
		if(stack_addr[free_stack].flags.use == 0)break;
	}
	if(free_stack == NUMBER_OF_MAX_TASK)return -1;
	stack_addr[free_stack].flags.use = 1;
	
	terget->reg.PC 			= (unsigned long)cts->function;
	terget->reg.USP 		= stack_addr[free_stack].su_addr;
	terget->reg.ISP 		= stack_addr[free_stack].si_addr;
	*(long *)(terget->reg.USP) 	= (long)user_kernel_des;
	terget->reg.PSW			= 0x00130000;
	terget->reg.FPSW		= 0x00000100;
	
	terget->reg.GREG[0] 		= (long)cts->tid;
	terget->reg.GREG[1] 		= (long)cts->arg;
	
	terget->function 		= cts->function;
	terget->arg 			= cts->arg;
	terget->priority 		= cts->attr.BIT.priority;
	terget->Flags.use 		= 1;
	terget->stack_num 		= free_stack;
	
	switch(cts->attr.BIT.create_mode){
	case (CT_READY & 0x00000f00) >> 8:
		kernel_ready_thread(cts->tid);
		terget->state = TASK_READY;
		return -1;
		break;
	case (CT_WAIT & 0x00000f00) >> 8:
		terget->state = TASK_WAIT;
		break;
	case CT_RUN:
		break;
	}
	
	return 0;
}

int kernel_ready_thread(thread_t *tid){

	ready_list.priority_push(tid);
	
	tid->state 			= TASK_READY;

	return 0;
}

int kernel_suspend_thread(thread_t *tid){
	if(tid == NULL)tid = current_thread;
	tid->state = TASK_WAIT;
	
	return 0;
}

int kernel_resume_thread(thread_t *tid){
	if(tid == NULL)tid = current_thread;
	if(tid->state == TASK_WAIT)
		kernel_ready_thread(tid);
	return 0;
}

int kernel_destroy_thread(thread_t *tid){
	if(tid == NULL)tid = current_thread;
	tid->state = TASK_NON;
	tid->Flags.use = 0;
	stack_addr[tid->stack_num].flags.use = 0;
	tid->stack_num = 0;
	return 0;
}

void user_kernel_des(void){
	thread_destroy(NULL);
}

int context_switch(thread_t*,thread_t*);

int schedule(void)
{
	
	if(current_thread->state != TASK_RUN);				//現在実行中のスレッドが休止であれば、必ず次にスレッドを実行
	else if(current_thread->priority < (*(ready_list.begin()))->priority){	//現在実行中のスレッドのほうが優先度が高ければ無視
		return 0;
	}
	
	thread_t *new_tid = ready_list.pop_front();
	context_switch(current_thread,new_tid);
	
	return 0;
}
extern "C"
extern int _switch_to(_task_control_block *prev,_task_control_block *next);

int context_switch(thread_t* old_tid,thread_t* new_tid){
	switch(old_tid->state){		//前のカーネルが今どのような状況であるかによって、退避方法を変える
	case TASK_NON:
		break;
	case TASK_WAIT:
		break;
	case TASK_RUN:
		kernel_ready_thread(old_tid);
		break;
	}
	
	new_tid->state = TASK_RUN;
	
	current_thread = new_tid;
	
	return _switch_to(old_tid,new_tid);
}

thread_t* get_tid(void){
	return current_thread;
}

void set_atomic(void){
	clrpsw_i();
}

void unset_atomic(void)
{
	setpsw_i();
}
