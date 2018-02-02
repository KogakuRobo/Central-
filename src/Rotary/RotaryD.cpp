#include "RotaryD.hpp"
#include "CentralLibrary.h"
#include "lowsrc.hpp"



class _rotary_d:public _low_file_desc_factor,public _low_file_desc_class{
	static const unsigned char g_rotary_d_path[];

	//MTU1.TCNTのオーバーフローカウンタ
	static unsigned short over_count;
public:
	_rotary_d(void);
	virtual const unsigned char* get_name(void){return this->g_rotary_d_path;}
	virtual _low_file_desc_class* open(const char*,long);
	virtual long close(_low_file_desc_class*);
	
	virtual long read(unsigned char*,long);
	virtual long write(const unsigned char*,long);
	
	virtual long ioctl(unsigned long,void*);
	
	static void count_overflow(void){over_count++;}
	
	static void count_underflow(void){over_count--;}
	
private:
	long begin(void *argp);
	long get_count(void *argp);
}Rotary_d;

const unsigned char _rotary_d::g_rotary_d_path[] = "ROTARY_D";
unsigned short _rotary_d::over_count;

_rotary_d::_rotary_d(void){
	MSTP(MTU8) = 0;
	
	over_count = 0;
	
	set_io_driver(&Rotary_d);
}

_low_file_desc_class* _rotary_d::open(const char* name,long mode)
{	
	MTU8.TCR.BIT.CCLR = 0;
	MTU8.TMDR.BIT.MD = 4;
	
	MTU8.TIOR.BIT.IOB = 0xc;
	MTU8.TIOR.BIT.IOA = 0xc;
	
	MTU8.TIER.BIT.TCIEU = 1;
	MTU8.TIER.BIT.TCIEV = 1;
	
	PORTC.DDR.BIT.B0 = 0;
	PORTC.DDR.BIT.B1 = 0;
	
	PORTC.PCR.BIT.B0 = 1;
	PORTC.PCR.BIT.B1 = 1;
	
	PORTC.ICR.BIT.B0 = 1;
	PORTC.ICR.BIT.B1 = 1;
	
	IOPORT.PFDMTU.BIT.TCLKS = 0;
	
	return this;
}

long _rotary_d::close(_low_file_desc_class* desc)
{
	return 0;
}

long _rotary_d::read(unsigned char *buf,long count)
{
	return -1;
}

long _rotary_d::write(const unsigned char *buf,long count)
{
	return -1;
}

long _rotary_d::ioctl(unsigned long request,void* attr)
{
	switch(request){
	case ROTARY_BEGIN:
		return begin(attr);
	case ROTARY_GET_COUNT:
		return get_count(attr);
	default:
		return -1;
	}
	return 0;
}

long _rotary_d::begin(void* argp){
	
	IPR(MTU8,TCIV8) = 15;
	IPR(MTU8,TCIU8) = 15;
	IEN(MTU8,TCIV8) = 1;
	IEN(MTU8,TCIU8) = 1;
	
	MTUB.TSTR.BIT.CST2 = 1;
	return 0;
}

long _rotary_d::get_count(void *argp){
	return ((unsigned long)(over_count << 16)) | ((unsigned long)MTU8.TCNT);
}



#pragma interrupt MTU8_TCIV8(vect = VECT(MTU8,TCIV8))
void MTU8_TCIV8(void){
	Rotary_d.count_overflow();
}

#pragma interrupt MTU8_TCIU8(vect = VECT(MTU8,TCIU8))
void MTU8_TCIU8(void){
	Rotary_d.count_underflow();
}