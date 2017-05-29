
#include<stdio.h>
#include<stdlib.h>
#include"../lowsrc.h"
#include"CentralLibrary.h"
#include"machine.h"

volatile int g_time = 0;


thread_t *t_main;
thread_t t_test;

void cmt_init(void){
	CMT0.CMCR.BIT.CKS = 0;
	CMT0.CMCR.BIT.CMIE = 1;
	
	IPR(CMT0,CMI0) = 10;
	IEN(CMT0,CMI0) = 1;
	
	CMT0.CMCOR = 15000;
	
	CMT.CMSTR0.BIT.STR0 = 1;
}

void *TestA(thread_t * tid,void *attr){
	FILE *fp = (FILE*)attr;
	while(1){
		fprintf(fp,"count Up\n\r");
		for(int i = 0;i < 256;i++){
			for(int j =0 ; j < 256;j++){
			}
		}
	}
	return NULL;
}

void *TestB(thread_t * tid,void *attr){
	FILE *fp = (FILE*)attr;
	PORTA.DR.BIT.B0 = 0;
	PORTA.DR.BIT.B1 = 0;
	while(1){
		g_time++;
		for(int i = 0;i < 256;i++){
			for(int j =0 ; j < 256;j++){
			}
		}
	}
	return NULL;
}

void main(void)
{
	FILE *fp = fopen("SCI0","w");

	setvbuf(fp,(char*)fp->_Buf,_IONBF,1);
	
	thread_create(&t_test,CT_PRIORITY_MIN,TestB,fp);
	
	PORTA.DDR.BIT.B0 = 0x01;
	PORTA.DDR.BIT.B1 = 0x01;
	while(1){
		static int count = 0;
		int time = g_time;
		fprintf(fp,"time[%d] %d\n\r",count++,time);
	}
	while(1);
}

extern "C"{
void user_abort(void){
}
}