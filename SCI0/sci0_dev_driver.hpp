#ifndef _sci0_dev_driver_HPP_
#define _sci0_dev_driver_HPP_

#include "Description.h"

//�{�[���[�g��ݒ肵�܂��B[bps]
//�ݒ�Ɏ��s�����-1��Ԃ��܂��B
//���������0��Ԃ��܂��B
int _sci0_set_baudrate(unsigned long);

//�p���e�B�r�b�g�̎g�p���邩�ݒ肵�܂��Btrue�Ŏg�p�Bfalse�ŕs�g�p
//���0���������܂��B
int _sci0_use_parity_bit(bool = true);

//���g�Ȃ�
int _sci0_set_parity_bit(unsigned char);

int _sci0_gpio_enable(bool = true);

#endif