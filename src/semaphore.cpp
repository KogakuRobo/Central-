#include "semaphore.hpp"
#include "syscall_table.h"
#include "user_system_call.h"

#define SEM_VALUE_MAX 255

int sem_atmoic_increment_val(sem_t* sem)
{
}

int sem_init(sem_t* sem,int pshared,unsigned int value)
{
	if(value > SEM_VALUE_MAX){
		return -1;
	}
	
	sem->count = value;
	return 0;
}
/*/
int sem_wait(sem_t* sem)
{
	int err;
	do{
		if(user_syscall(SYSCALL_ATOMIC_DECREMENT_VAL,&sem->count) == 0){
			return 0;
		}
	}while
	return -1;
}
//*/
int sem_trywait(sem_t* sem)
{
}

int sem_post(sem_t* sem)
{
}
