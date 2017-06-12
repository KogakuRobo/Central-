
#include <stdio.h>
#include <stdlib.h>
#include "localization.hpp"
#include "CentralLibrary.h"
#include "RotaryA.hpp"
#include "RotaryB.hpp"

thread_t *t_main;

volatile data d;

void main(void)
{
	//SCI0��open�ƃm���o�b�t�@����
	FILE *fp = fopen("SCI0","w");
	if(fp == NULL){
		printf("LKK");
	}
	setvbuf(fp,(char*)fp->_Buf,_IONBF,1);
	
	fprintf(fp,"Program Start\n\r");
	
	//thread_t loca;
	//thread_create(&loca,CT_PRIORITY_MAX + 3,localization,&d);
	
	PORTA.DDR.BIT.B0 = 1;
	PORTA.DDR.BIT.B1 = 1;
	
	extern long kernel_time;
	
	localization_init();
	
	msleep(2000);
	
	fprintf(fp,"Average:,%d\n\r",d.ave);
	fprintf(fp,"Deviation:,%d\n\r",d.devia);
	fprintf(fp,"\n\rtime,yaw \n\r");
	
	for(float i = 0.0;;i = i + 1.0){
		msleep(10);
		//fprintf(fp,"%d,%f\n\r",d.time,d.yaw);
		fprintf(fp,"%d,%f,%f,%f\n\r",kernel_time,d.X*1000,d.Y*1000,d.yaw);
		//printf("FFF\n");
	}
	while(1);
}

extern "C"{
void user_abort(void){
}
}