#include "CentralLibrary.h"
#include "cmt3.hpp"

thread_t cmt3_timer::thread;
long cmt3_timer::priority;
void *(*cmt3_timer::f)(thread_t*,void*);
void *cmt3_timer::attr;
int cmt3_timer::use;

cmt3_timer::cmt3_timer(void){
	MSTP(CMT3) = 0;
	use = 0;
}

thread_t* cmt3_timer::set_timer(long usec,long priority,void* (*f)(thread_t*,void*),void *attr)
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
	
	CMT3.CMCR.BIT.CKS = i;
	CMT3.CMCR.BIT.CMIE = 1;
	
	thread_create(&this->thread,CT_WAIT | priority,f,attr);
	
	
	IPR(CMT3,CMI3) = 13;
	IEN(CMT3,CMI3) = 1;
	
	CMT3.CMCOR = temp - 1;
	
	CMT.CMSTR1.BIT.STR3 = 1;
	
	return &thread;
}

void cmt3_timer::stop_timer(void){
	IEN(CMT3,CMI3) = 0;
	CMT.CMSTR1.BIT.STR3 = 0;
}

bool cmt3_timer::is_action(void){
	return use == 1;
}

#pragma interrupt CMT3_CMI3(vect = VECT(CMT3,CMI3))
void CMT3_CMI3(void)
{
	if(cmt3_timer::thread.state == TASK_NON){
		thread_create(&cmt3_timer::thread,cmt3_timer::priority,cmt3_timer::f,cmt3_timer::attr);
	}
	else if(cmt3_timer::thread.state == TASK_WAIT){
		thread_resume(&cmt3_timer::thread);
	}
}
