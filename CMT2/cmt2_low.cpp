#include "CentralLibrary.h"
#include "cmt2.hpp"

thread_t cmt2_timer::thread;
long cmt2_timer::priority;
void *(*cmt2_timer::f)(thread_t*,void*);
void *cmt2_timer::attr;
int cmt2_timer::use;

cmt2_timer::cmt2_timer(void){
	MSTP(CMT2) = 0;
	use = 0;
}

thread_t* cmt2_timer::set_timer(long usec,long priority,void* (*f)(thread_t*,void*),void *attr)
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
	
	CMT2.CMCR.BIT.CKS = i;
	CMT2.CMCR.BIT.CMIE = 1;
	
	thread_create(&this->thread,CT_WAIT | priority,f,attr);
	
	
	IPR(CMT2,CMI2) = 13;
	IEN(CMT2,CMI2) = 1;
	
	CMT2.CMCOR = temp - 1;
	
	CMT.CMSTR1.BIT.STR2 = 1;
	
	return &thread;
}

void cmt2_timer::stop_timer(void){
	IEN(CMT2,CMI2) = 0;
	CMT.CMSTR1.BIT.STR2 = 0;
}

bool cmt2_timer::is_action(void){
	return use == 1;
}

#pragma interrupt CMT2_CMI2(vect = VECT(CMT2,CMI2))
void CMT2_CMI2(void)
{
	if(cmt2_timer::thread.state == TASK_NON){
		thread_create(&cmt2_timer::thread,cmt2_timer::priority,cmt2_timer::f,cmt2_timer::attr);
	}
	else if(cmt2_timer::thread.state == TASK_WAIT){
		thread_resume(&cmt2_timer::thread);
	}
}
