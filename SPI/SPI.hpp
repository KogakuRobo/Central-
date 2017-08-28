#ifndef _SPI_HPP_
#define _SPI_HPP_

#include"stddef.h"

//使用予定
class SPI_Channel{
public:
	virtual unsigned int ChannelSet(void) = 0;
};

//SPI_busクラス。
class SPI_Bus{
	
public:
	virtual int Begin(unsigned int) = 0;
//DataRW : unsigned int
//データの読み書き関数
//戻り値が0である時正常終了で、それ以外では何らかの失敗をしている。
	virtual unsigned int DataRW(unsigned int,unsigned char*,unsigned char *,unsigned short) = 0;
};

#endif