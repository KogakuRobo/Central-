#include "CentralLibrary.h"
#include "cmt1.hpp"

thread_t cmt1_timer::thread;
long cmt1_timer::priority;
void *(*cmt1_timer::f)(thread_t*,void*);
void* cmt1_timer::attr;
int cmt1_timer::use;

cmt1_timer::cmt1_timer(void){
	MSTP(CMT1) = 0;
	use = 0;
}

thread_t* cmt1_timer::set_timer(long usec,long priority,void* (*f)(thread_t*,void*),void *attr)
{
	const int CKS[] = {8,32,128,512};
	unsigned short temp;
	int i;
	for (i = 0;i < 4;i++){
		temp = usec * PCK_CLOCK / CKS[i];
		if(temp < 65536)break;
	}
	
	if(i == 4)return NULL;
	
	this->priority = priority;
	this->f = f;
	this->attr = attr;
	use = 1;
	
	CMT1.CMCR.BIT.CKS = i;
	CMT1.CMCR.BIT.CMIE = 1;
	
	thread_create(&this->thread,CT_WAIT | priority,f,attr);
	
	
	IPR(CMT1,CMI1) = 13;
	IEN(CMT1,CMI1) = 1;
	
	CMT1.CMCOR = temp - 1;
	
	CMT.CMSTR0.BIT.STR1 = 1;
	
	return &thread;
}

void cmt1_timer::stop_timer(void){
	IEN(CMT1,CMI1) = 0;
	CMT.CMSTR0.BIT.STR1 = 0;
}

bool cmt1_timer::is_action(void){
	return use == 1;
}

#pragma interrupt CMT1_CMI1(vect = VECT(CMT1,CMI1))
void CMT1_CMI1(void)
{
	if(cmt1_timer::thread.state == TASK_NON){
		thread_create(&cmt1_timer::thread,cmt1_timer::priority,cmt1_timer::f,cmt1_timer::attr);
	}
	else if(cmt1_timer::thread.state == TASK_WAIT){
		thread_resume(&cmt1_timer::thread);
	}
}