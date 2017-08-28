#ifndef _SPI_HPP_
#define _SPI_HPP_

#include"stddef.h"

//�g�p�\��
class SPI_Channel{
public:
	virtual unsigned int ChannelSet(void) = 0;
};

//SPI_bus�N���X�B
class SPI_Bus{
	
public:
	virtual int Begin(unsigned int) = 0;
//DataRW : unsigned int
//�f�[�^�̓ǂݏ����֐�
//�߂�l��0�ł��鎞����I���ŁA����ȊO�ł͉��炩�̎��s�����Ă���B
	virtual unsigned int DataRW(unsigned int,unsigned char*,unsigned char *,unsigned short) = 0;
};

#endif