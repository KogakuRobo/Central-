#ifndef _semaphore_H_
#define _semaphore_H

typedef struct{
	int count;
}sem_t;

//�Z�}�t�H�̏�����
//�����@0
//���s�@-1
extern int sem_init(sem_t* sem,int pshared,unsigned int value);

//�Z�}�t�H�̊m��
extern int sem_wait(sem_t *sem);
extern int sem_trywait(sem_t* sem);

//�Z�}�t�H�̃����[�X
extern int sem_post(sem_t* sem);

#endif