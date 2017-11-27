#ifndef _task_control_block_H_
#define _task_control_block_H_

#include "thread.h"
/*/
#pragma stacksize su=0x4000
#pragma stacksize si=0x4000
//*/
//*/

#define STACK_USER_SIZE		0x4000
#define STACK_KERNEL_SIZE	0x4000

//旧仕様
#define NUMBER_OF_MAX_TASK 	16
#define SIZE_OF_USER_STACK	0x400
#define SIZE_OF_KERNEL_STACK	0x400

//新仕様
#define SIZE_OF_SECT_SU		0x100
#define SIZE_OF_SECT_SI		0x100
//*/
//以下の式が成り立たなければならない。
//su = NUMBER_OF_MAX_TASK * SIZE_OF_USER_STACK
//si = NUMBER_OF_MAX_TASK * SIZE_OF_KERNEL_STACK

#endif