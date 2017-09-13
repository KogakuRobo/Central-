#ifndef _GPIOA_HPP_
#define _GPIOA_HPP_

#include "CentralLibrary.h"
#include "lowsrc.hpp"

class _gpio_a_port_reader:public _low_file_desc_class{
public:
	_gpio_a_port_reader(void);
	virtual long read(unsigned char *,long);
};

class _gpio_a_port_writer:public _low_file_desc_class{
public:
	_gpio_a_port_writer(void);
	virtual long write(const unsigned char *,long);
};

class _gpio_a_bit_reader:public _low_file_desc_class{
	int num;
public:
	_gpio_a_bit_reader(int _num);
	virtual long read(unsigned char *,long);
};

class _gpio_a_bit_writer:public _low_file_desc_class{
	int num;
public:
	_gpio_a_bit_writer(int _num);
	virtual long write(const unsigned char *,long);
};

#endif