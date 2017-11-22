#ifndef _task_control_block_H_
#define _task_control_block_H_

#include "thread.h"
/*/
#pragma stacksize su=0x2000
#pragma stacksize si=0x2000
//*/
//*/
#define NUMBER_OF_MAX_TASK 	16
#define SIZE_OF_USER_STACK	0x400
#define SIZE_OF_KERNEL_STACK	0x400
//*/
//à»â∫ÇÃéÆÇ™ê¨ÇËóßÇΩÇ»ÇØÇÍÇŒÇ»ÇÁÇ»Ç¢ÅB
//su = NUMBER_OF_MAX_TASK * SIZE_OF_USER_STACK
//si = NUMBER_OF_MAX_TASK * SIZE_OF_KERNEL_STACK

#endif