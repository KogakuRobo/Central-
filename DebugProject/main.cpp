
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "localization.hpp"
#include "CentralLibrary.h"
#include "RotaryC.hpp"
#include "MotorA.hpp"

thread_t *t_main;

volatile data d;

void MTU9_Init(void){
	
	MSTP(MTU9) = 0;
	PORTB.DDR.BYTE = 0xff;
	PORTB.DR.BIT.B2 = 1;
	PORTB.DR.BIT.B3 = 0;
	
	MTUB.TRWER.BIT.RWE = 1;
	
	MTU9.TCR.BIT.CCLR = 1;
	MTU9.TCR.BIT.CKEG = 0;
	MTU9.TCR.BIT.TPSC = 0;
	
	MTU9.TMDR.BIT.MD = 2;
	
	MTU9.TIORH.BIT.IOA = 6;
	MTU9.TIORH.BIT.IOB = 5;
	MTU9.TIORL.BIT.IOC = 6;
	MTU9.TIORL.BIT.IOD = 5;
	
	MTU9.TGRA = 1000;
	MTU9.TGRC = 1000;
	MTU9.TGRB = 0;
	MTU9.TGRD = 0;
	
	MTUB.TSTR.BIT.CST3 = 1;
}

void main(void)
{
	//SCI0のopenとノンバッファ処理
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
	
	MTU9_Init();
	
	msleep(2000);
	
	fprintf(fp,"Average:,%d\n\r",d.ave);
	fprintf(fp,"Deviation:,%d\n\r",d.devia);
	fprintf(fp,"duty,speed\n\r");
	
	int rotaryc = open("ROTARY_C",0,0);
	ioctl(rotaryc,ROTARYC_BEGIN,NULL);
	
	int motora = open("MOTOR_A",0,0);
	
	for(float i = 0.0;;i = i + 1.0){
		float duty = 0;
		//msleep(10);
		//fprintf(fp,"%d,%f\n\r",d.time,d.yaw);
		/*fprintf(fp,"%d,%f,%f,%f,%f,%f\n\r",kernel_time,d.X*1000,d.Y*1000,d.yaw,
			d.X*1000 - 244 * sin(d.yaw) + 56 * cos(d.yaw),
			d.Y*1000 + 56 * sin(d.yaw) + 244 * cos(d.yaw)
		);*/
		duty = 99.0*sin(i / 20);
		ioctl(motora,MOTORA_SET_DUTY,&duty);
		msleep(1000);
		int befor = ioctl(rotaryc,ROTARYC_GET_COUNT,NULL);
		msleep(500);
		int after = ioctl(rotaryc,ROTARYC_GET_COUNT,NULL);
		
		fprintf(fp,"%f,%d\n\r",duty,after - befor);
		
		//printf("FFF\n");
	}
	while(1);
}

extern "C"{
void user_abort(void){
}
}