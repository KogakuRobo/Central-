#ifndef _thread_control_HPP_
#define _thread_control_HPP_

#include "task_control_block.h"
#include "syscall_table.h"

extern int kernel_create_thread(CreateThreadStruct*);
extern int kernel_destroy_thread(thread_t*);
extern int kernel_suspend_thread(thread_t*);
extern int kernel_resume_thread(thread_t*);

extern int kernel_msleep(unsigned long);

#endif