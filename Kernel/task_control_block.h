#ifndef _task_control_block_H_
#define _task_control_block_H_

#include "thread.h"

#pragma stacksize su=0x1800
#pragma stacksize si=0x1800

#define NUMBER_OF_MAX_TASK 	6
#define SIZE_OF_USER_STACK	0x400
#define SIZE_OF_KERNEL_STACK	0x400

//以下の式が成り立たなければならない。
//su = NUMBER_OF_MAX_TASK * SIZE_OF_USER_STACK
//si = NUMBER_OF_MAX_TASK * SIZE_OF_KERNEL_STACK

#endif