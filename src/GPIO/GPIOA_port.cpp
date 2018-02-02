#include "CentralLibrary.h"
#include "lowsrc.hpp"
#include "GPIOA.hpp"

_gpio_a_port_reader::_gpio_a_port_reader(void)
{
	PORT1.DDR.BIT.B2 = 0;
	PORT1.DDR.BIT.B3 = 0;
	PORT5.DDR.BIT.B0 = 0;
	PORT5.DDR.BIT.B1 = 0;
	PORT5.DDR.BIT.B2 = 0;
	PORT5.DDR.BIT.B3 = 0;
	PORT5.DDR.BIT.B4 = 0;
	PORT5.DDR.BIT.B5 = 0;
}

long _gpio_a_port_reader::read(unsigned char *format,long len){
	if(len != 1)return -1;
	
	unsigned char temp = 0;
	
	temp = 	 (PORT5.PORT.BIT.B5 << 7)
		|(PORT5.PORT.BIT.B4 << 6)
		|(PORT5.PORT.BIT.B3 << 5)
		|(PORT5.PORT.BIT.B2 << 4)
		|(PORT5.PORT.BIT.B1 << 3)
		|(PORT5.PORT.BIT.B0 << 2)
		|(PORT1.PORT.BIT.B3 << 1)
		|(PORT1.PORT.BIT.B2 << 0);
	
	*format = temp;
	return 0;
}

_gpio_a_port_writer::_gpio_a_port_writer(void)
{
	PORT1.DDR.BIT.B2 = 1;
	PORT1.DDR.BIT.B3 = 1;
	PORT5.DDR.BIT.B0 = 1;
	PORT5.DDR.BIT.B1 = 1;
	PORT5.DDR.BIT.B2 = 1;
	PORT5.DDR.BIT.B3 = 1;
	PORT5.DDR.BIT.B4 = 1;
	PORT5.DDR.BIT.B5 = 1;
}

long _gpio_a_port_writer::write(const unsigned char *format,long len)
{
	unsigned char temp = format[0];
	PORT5.DR.BIT.B5 = (temp >> 7) & 0x01;
	PORT5.DR.BIT.B4 = (temp >> 6) & 0x01;
	//PORT5.DR.BIT.B3 = (temp >> 5) & 0x01;
	PORT5.DR.BIT.B2 = (temp >> 4) & 0x01;
	PORT5.DR.BIT.B1 = (temp >> 3) & 0x01;
	PORT5.DR.BIT.B0 = (temp >> 2) & 0x01;
	PORT1.DR.BIT.B3 = (temp >> 1) & 0x01;
	PORT1.DR.BIT.B2 = (temp >> 0) & 0x01;
	return 0;
}

_gpio_a_bit_reader::_gpio_a_bit_reader(int _num):
num(_num)
{
	switch(num){
	case 0:
		PORT1.DDR.BIT.B2 = 0;
		break;
	case 1:
		PORT1.DDR.BIT.B3 = 0;
		break;
	case 2:
		PORT5.DDR.BIT.B0 = 0;
		break;
	case 3:
		PORT5.DDR.BIT.B1 = 0;
		break;
	case 4:
		PORT5.DDR.BIT.B2 = 0;
		break;
	case 5:
		PORT5.DDR.BIT.B3 = 0;
		break;
	case 6:
		PORT5.DDR.BIT.B4 = 0;
		break;
	case 7:
		PORT5.DDR.BIT.B5 = 0;
		break;
	default:
		break;
	}
}

long _gpio_a_bit_reader::read(unsigned char *format,long ren)
{
	long ret = 0;
	if(ren != 1)return -1;
	switch(num){
	case 0:
		format[0] = PORT1.PORT.BIT.B2;
		ret = 0;
		break;
	case 1:
		format[0] = PORT1.PORT.BIT.B3;
		ret = 0;
		break;
	case 2:
		format[0] = PORT5.PORT.BIT.B0;
		ret = 0;
		break;
	case 3:
		format[0] = PORT5.PORT.BIT.B1;
		ret = 0;
		break;
	case 4:
		format[0] = PORT5.PORT.BIT.B2;
		ret = 0;
		break;
	case 5:
		format[0] = PORT5.PORT.BIT.B3;
		ret = 0;
		break;
	case 6:
		format[0] = PORT5.PORT.BIT.B4;
		ret = 0;
		break;
	case 7:
		format[0] = PORT5.PORT.BIT.B5;
		ret = 0;
		break;
	default:
		ret = -1;
	}
	return ret;
}

_gpio_a_bit_writer::_gpio_a_bit_writer(int _num):
num(_num)
{
	switch(num){
	case 0:
		PORT1.DDR.BIT.B2 = 1;
		break;
	case 1:
		PORT1.DDR.BIT.B3 = 1;
		break;
	case 2:
		PORT5.DDR.BIT.B0 = 1;
		break;
	case 3:
		PORT5.DDR.BIT.B1 = 1;
		break;
	case 4:
		PORT5.DDR.BIT.B2 = 1;
		break;
	case 5:
		PORT5.DDR.BIT.B3 = 1;
		break;
	case 6:
		PORT5.DDR.BIT.B4 = 1;
		break;
	case 7:
		PORT5.DDR.BIT.B5 = 1;
		break;
	default:
		break;
	}
}

long _gpio_a_bit_writer::write(const unsigned char *format,long ren)
{
	long ret = 0;
	
	if(ren != 1)return -1;
	
	switch(num){
	case 0:
		PORT1.DR.BIT.B2 = format[0];
		ret = 0;
		break;
	case 1:
		PORT1.DR.BIT.B3 = format[0];
		ret = 0;
		break;
	case 2:
		PORT5.DR.BIT.B0 = format[0];
		ret = 0;
		break;
	case 3:
		PORT5.DR.BIT.B1 = format[0];
		ret = 0;
		break;
	case 4:
		PORT5.DR.BIT.B2 = format[0];
		ret = 0;
		break;
	case 5:
		//PORT5.DR.BIT.B3 = format[0];
		ret = -1;
		break;
	case 6:
		PORT5.DR.BIT.B4 = format[0];
		ret = 0;
		break;
	case 7:
		PORT5.DR.BIT.B5 = format[0];
		ret = 0;
		break;
	default:
		ret = -1;
	}
	return ret;
}