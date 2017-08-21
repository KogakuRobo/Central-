#ifndef _RotaryA_HPP_
#define _RotaryA_HPP_

//�Z���g�����{�[�h�p�@���[�^���[�G���R�[�_A�@�f�o�C�X�h���C�o�w�b�_

//�h���C�o���@"ROTARY_A"

#include "CentralLibrary.h"
#include "Rotary.hpp"

typedef enum{
	//TGIA���荞�ݎ��ɍ쐬����X���b�h��ݒ肵�܂��B
	ROTARYA_SET_TGIA = ROTARY_IOCTL_LAST,
};

typedef struct{
	void* (*function)(thread_t*,void*);
	long attr;
	void *argp;
}interrupt_stc;

#endif