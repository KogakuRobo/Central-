#ifndef _stack_allocate_HPP_
#define _stack_allocate_HPP_

#include "task_control_block.h"

/*
�X�^�b�N�̈�Ǘ��v���O����

�X�^�b�N�̈�̓��I�����āA���тɉ��������s���v���O�����B
�m�ۂ��ꂽ�X�^�b�N�̈�����̃Z�N�^�ŋ�؂�
�g�p����Ă���Z�N�^���Ǘ����܂��B

�Z�N�^�T�C�Y��

*/


void stack_manager_init(void);
//�X�^�b�N�̈���m�ۂ��A���̃A�h���X��Ԃ��܂��B
//�G���[�̏ꍇ��NULL��Ԃ��܂��B
void* _sualloc(int size);

#endif