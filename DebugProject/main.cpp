
#include<stdio.h>
#include<stdlib.h>
#include"../lowsrc.h"
#include"CentralLibrary.h"

void *Test(thread_t * tid,void *attr){
	FILE *fp = (FILE*)attr;
	fprintf(fp,"b\r\n");
	thread_t t_test;
	thread_create(&t_test,CT_PRIORITY_MAX + 1,Test,attr);
	return NULL;
}

void main(void){
	
	FILE *fp = fopen("SCI0","w");
	fprintf(fp,"a\r\n");
	
	thread_t t_test;
	thread_create(&t_test,CT_PRIORITY_MAX,Test,fp);
	
	while(1);
}

extern "C"{
void user_abort(void){
}
}