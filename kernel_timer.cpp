#include "CentralLibrary.h"
#include "que.hpp"
#include "kernel_timer.hpp"

typedef struct{
	thread_t* tid;
	unsigned long timer;
	struct{
		unsigned char use:1;
	}FLAGS;
}timer_task_node;

typedef timer_task_node* p_timer_task_node;

typedef list<p_timer_task_node,KERNEL_TIMER_TASK>::iterator timer_iterator;

timer_task_node time_node[KERNEL_TIMER_TASK];
unsigned long kernel_time = 0;

class timer_list:public list<p_timer_task_node,KERNEL_TIMER_TASK>{
public:
	void push(p_timer_task_node);
}t_list;

void timer_list::push(p_timer_task_node node){
	iterator ite = this->begin();
	
	if(this->empty()){
		this->push_front(node);
		return;
	}
	while((*ite)->timer < node->timer){
		++ite;
		if(ite == (this->begin())){
			this->push_back(node);
			return;
		}
	}
	
	this->insert(ite,node);
}

void cmt0_init(void){
	MSTP(CMT0) = 0;
	
	CMT0.CMCR.BIT.CKS = 0;
	CMT0.CMCR.BIT.CMIE = 1;
	
	IPR(CMT0,CMI0) = 13;
	IEN(CMT0,CMI0) = 1;
	
	CMT0.CMCOR = 6000 - 1;
	
	CMT.CMSTR0.BIT.STR0 = 1;
	for(int i = 0;i < KERNEL_TIMER_TASK;i++){
		time_node[i].FLAGS.use = 0;
	}
}

static int serch_free_node(void){
	for(int i = 0;i < KERNEL_TIMER_TASK;i++){
		if(time_node[i].FLAGS.use == 0){
			time_node[i].FLAGS.use = 1;
			return i;
		}
	}
	return -1;
}

int kernel_msleep(unsigned long t){
	int num = serch_free_node();
	thread_t* tid;
	
	if(num == -1)	return -1;
	
	tid = get_tid();
	time_node[num].timer = kernel_time + t;
	time_node[num].tid = tid;
	t_list.push(&time_node[num]);
	
	thread_suspend(tid);
	return 0;
}

#pragma interrupt CMT0_CMI0(vect = VECT(CMT0,CMI0),enable)
void CMT0_CMI0(void)
{
	timer_task_node* ttn;
	kernel_time++;
	
	do{
		if(t_list.empty())return;		//待ちノードがなければ割り込みから離脱（pop後になくなる可能性）
		timer_iterator ite = t_list.begin();	//次のノードを参照（なお、一回目も同様）
		if((*ite)->timer <= kernel_time){	//リスト登録のタイミングではカーネル時間より遅くなる。
			ttn = t_list.pop_front();	//取り出し
			ttn->FLAGS.use = 0;
			thread_resume(ttn->tid);
		}
		else return;
		
	}while(1);
}