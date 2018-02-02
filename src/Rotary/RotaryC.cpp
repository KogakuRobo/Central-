#include "RotaryC.hpp"
#include "CentralLibrary.h"
#include "lowsrc.hpp"



class _rotary_c:public _low_file_desc_factor,public _low_file_desc_class{
	static const unsigned char g_rotary_c_path[];

	//MTU1.TCNTのオーバーフローカウンタ
	static unsigned short over_count;
public:
	_rotary_c(void);
	virtual const unsigned char* get_name(void){return this->g_rotary_c_path;}
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
}Rotary_c;

const unsigned char _rotary_c::g_rotary_c_path[] = "ROTARY_C";
unsigned short _rotary_c::over_count;

_rotary_c::_rotary_c(void){
	MSTP(MTU2) = 0;
	
	over_count = 0;
	
	set_io_driver(&Rotary_c);
}

_low_file_desc_class* _rotary_c::open(const char* name,long mode)
{	
	MTU2.TCR.BIT.CCLR = 0;
	MTU2.TMDR.BIT.MD = 4;
	
	MTU2.TIOR.BIT.IOB = 0xc;
	MTU2.TIOR.BIT.IOA = 0xc;
	
	MTU2.TIER.BIT.TCIEU = 1;
	MTU2.TIER.BIT.TCIEV = 1;
	
	PORTC.DDR.BIT.B4 = 0;
	PORTC.DDR.BIT.B5 = 0;
	
	PORTC.PCR.BIT.B4 = 1;
	PORTC.PCR.BIT.B5 = 1;
	
	PORTC.ICR.BIT.B4 = 1;
	PORTC.ICR.BIT.B5 = 1;
	
	IOPORT.PFCMTU.BIT.TCLKS = 1;
	
	return this;
}

long _rotary_c::close(_low_file_desc_class* desc)
{
	return 0;
}

long _rotary_c::read(unsigned char *buf,long count)
{
	return -1;
}

long _rotary_c::write(const unsigned char *buf,long count)
{
	return -1;
}

long _rotary_c::ioctl(unsigned long request,void* attr)
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

long _rotary_c::begin(void* argp){
	
	IPR(MTU2,TCIV2) = 15;
	IPR(MTU2,TCIU2) = 15;
	IEN(MTU2,TCIV2) = 1;
	IEN(MTU2,TCIU2) = 1;
	
	MTUA.TSTR.BIT.CST2 = 1;
	return 0;
}

long _rotary_c::get_count(void *argp){
	return ((unsigned long)(over_count << 16)) | ((unsigned long)MTU2.TCNT);
}



#pragma interrupt MTU2_TCIV2(vect = VECT(MTU2,TCIV2))
void MTU2_TCIV2(void){
	Rotary_c.count_overflow();
}

#pragma interrupt MTU2_TCIU2(vect = VECT(MTU2,TCIU2))
void MTU2_TCIU2(void){
	Rotary_c.count_underflow();
}