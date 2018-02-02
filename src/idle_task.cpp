
#include"CentralLibrary.h"
#include"machine.h"

void* idle_task(thread_t *tid,void* attr)
{
	while(1){
		wait();			//低消費電力モードへ移行
	}
}