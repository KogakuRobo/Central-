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
}data;

extern void *localization(thread_t*,void*);

#endif