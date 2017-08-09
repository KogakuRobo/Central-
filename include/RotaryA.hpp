#ifndef _RotaryA_HPP_
#define _RotaryA_HPP_

//�Z���g�����{�[�h�p�@���[�^���[�G���R�[�_A�@�f�o�C�X�h���C�o�w�b�_

//�h���C�o���@"ROTARY_A"

#include "CentralLibrary.h"

typedef enum{
	//�N���b�N���[�g��I�����܂��B
	ROTARYA_SET_CLOCKRATE,
	
	//���W���[���̓�����J�n���܂��B
	ROTARYA_BEGIN,
	
	//�߂�l�ɃJ�E���g���擾���܂��B
	ROTARYA_GET_COUNT,
	
	//TGIA���荞�ݎ��ɍ쐬����X���b�h��ݒ肵�܂��B
	ROTARYA_SET_TGIA,
}rotary_a_ioctl_cmd;

typedef struct{
	void* (*function)(thread_t*,void*);
	long attr;
	void *argp;
}interrupt_stc;

#endif