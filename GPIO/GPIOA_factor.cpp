#include "CentralLibrary.h"

#include "lowsrc.hpp"

class _gpio_a:public _low_file_desc_factor{
	static const unsigned char g_gpio_a_path[];
public:
	_gpio_a(void);
	
	virtual const unsigned char* get_name(void){return this->g_gpio_a_path;}
	virtual _low_file_desc_class* open(const char*,long);
	virtual long close(_low_file_desc_class*);
}Gpio_A;

const unsigned char _gpio_a::g_gpio_a_path[] = "GPIO_A";

_gpio_a::_gpio_a(void){
	set_io_driver(&Gpio_A);
}

_low_file_desc_class* _gpio_a::open(const char* name,long attr)
{
	
}

long _gpio_a::close(_low_file_desc_class* desc)
{
	return CLOSE_NON_ERROR;
}

