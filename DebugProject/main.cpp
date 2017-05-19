
#include<stdio.h>
#include"CentralLibrary.h"

void main(void){
	int i = 0;
	FILE *fp = fopen("SCI0","w");
	printf("%d\n",FOPEN_MAX);
	while(1);
}

extern "C"{
void user_abort(void){
}
}