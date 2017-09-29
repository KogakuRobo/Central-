#ifndef _semaphore_H_
#define _semaphore_H

typedef struct{
	int count;
}sem_t;

//セマフォの初期化
//成功　0
//失敗　-1
extern int sem_init(sem_t* sem,int pshared,unsigned int value);

//セマフォの確保
extern int sem_wait(sem_t *sem);
extern int sem_trywait(sem_t* sem);

//セマフォのリリース
extern int sem_post(sem_t* sem);

#endif