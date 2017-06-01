
#include<stdio.h>
#include<stdlib.h>
#include"../lowsrc.h"
#include"CentralLibrary.h"
#include"machine.h"

volatile int g_r_time = 0;


thread_t *t_main;
thread_t t_test_a;

void cmt_init(void){
	MSTP(CMT0) = 0;
	CMT0.CMCR.BIT.CKS = 0;
	CMT0.CMCR.BIT.CMIE = 1;
	
	IPR(CMT0,CMI0) = 15;
	IEN(CMT0,CMI0) = 1;
	
	CMT0.CMCOR = 6000 - 1 ;
	
	CMT.CMSTR0.BIT.STR0 = 1;
}

#pragma interrupt CMT0_CMI0(vect = VECT(CMT0,CMI0))
void CMT0_CMI0(void)
{
	thread_resume(&t_test_a);
}


void *TestA(thread_t * tid,void *attr){
	while(1){
		thread_suspend(&t_test_a);
		g_r_time++;
	}
}

void main(void)
{
	FILE *fp = fopen("SCI0","w");

	setvbuf(fp,(char*)fp->_Buf,_IONBF,1);
	
	thread_create(&t_test_a,CT_PRIORITY_MAX,TestA,fp);
	
	cmt_init();
	
	PORTA.DDR.BIT.B0 = 0x01;
	PORTA.DDR.BIT.B1 = 0x01;
	while(1){
		static int count = 0;
		int ps = get_psw();
		fprintf(fp,"time[%d],%d\n\r",count++,g_r_time);
	}
	while(1);
}

extern "C"{
void user_abort(void){
}
}