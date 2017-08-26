#ifndef _localization_HPP_
#define _localization_HPP_

#include "CentralLibrary.h"

//���Ȉʒu����p�v���O����

//�d�l���W���[��
//CMT1	(�W���C���Z���T�̃f�[�^�擾���������j
//RSPI1�@�i�W���C���Z���T�[�Ƃ̒ʐM�j

typedef struct{
	long ave;
	long devia;
	float yaw;
	
	long count_A;
	
	long count_B;
	
	long time;
	
	float X,Y;
}data;

//class Localization

extern volatile data d;

extern void* localization_init(void);
extern void *localization(thread_t*,void*);

#endif