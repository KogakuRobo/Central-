#ifndef _thread_H_
#define _thread_H_

#define CT_PRIORITY_MAX 0x00000000
#define CT_PRIORITY_MIN 0x000000ff

#define CT_PRIORITY(x) x & 0x000000ff

typedef struct {
	void *p_TCB;
	void *p_Que;
}thread_t;

extern int thread_create(thread_t*,long,void*(*)(thread_t*,void*),void*);
extern int thread_wakeup(thread_t*);
extern int thread_destroy(thread_t*);
extern int thread_suspend(thread_t*);
extern int thread_resume(thread_t*);

extern int get_tid(thread_t**);

#endif