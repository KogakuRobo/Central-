
#include"CentralLibrary.h"
#include"machine.h"

void* idle_task(thread_t *tid,void* attr)
{
	while(1){
		wait();			//�����d�̓��[�h�ֈڍs
	}
}