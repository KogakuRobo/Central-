#include "RotaryA.hpp"
#include "CentralLibrary.h"
#include "lowsrc.hpp"



class _rotary_a:public _low_file_desc_factor,public _low_file_desc_class{
	static const unsigned char g_rotary_a_path[];
public:
	_rotary_a(void);
	virtual const unsigned char* get_name(void){return this->g_rotary_a_path;}
	virtual _low_file_desc_class* open(const char*,long);
	virtual long close(_low_file_desc_class*);
	
	virtual long read(unsigned char*,long);
	virtual long write(const unsigned char*,long);
	
	virtual long ioctl(unsigned long,void*);
	
private:
	long begin(void *argp);
}Rotary_a;

const unsigned char _rotary_a::g_rotary_a_path[] = "ROTARY_A";

_rotary_a::_rotary_a(void){
	MSTP(MTU0) = 0;
	MSTP(MTU1) = 0;
	
	set_io_driver(&Rotary_a);
}

_low_file_desc_class* _rotary_a::open(const char* name,long mode)
{
	//MTU0.TCNTを24MHzカウントアップ(48MHz / 4 * 2)設定
	MTU0.TCR.BIT.TPSC = 1;
	MTU0.TCR.BIT.CKEG = 2;
	MTU0.TCR.BIT.CCLR = 1;
	
	MTU0.TMDR.BIT.BFB = 1;
	MTU0.TMDR.BIT.MD = 0;
	
	MTU0.TIORH.BIT.IOA = 0;
	MTU0.TIORH.BIT.IOB = 0xc;
	
	MTU0.TIORL.BIT.IOC = 0;
	MTU0.TIORL.BIT.IOD = 0;
	
	MTU0.TIER.BIT.TGIEA = 1;
	MTU0.TIER.BIT.TGIEC = 1;
	
	MTU0.TGRA = 60000 - 1;
	MTU0.TGRC = 30000 - 1;
	
	MTU1.TCR.BIT.CCLR = 0;
	MTU1.TMDR.BIT.MD = 4;
	
	MTU1.TIOR.BIT.IOB = 0xc;
	MTU1.TIOR.BIT.IOA = 0xc;
	
	PORT2.DDR.BIT.B4 = 0;
	PORT2.DDR.BIT.B5 = 0;
	
	PORT2.ICR.BIT.B4 = 1;
	PORT2.ICR.BIT.B5 = 1;
	
	IOPORT.PFCMTU.BIT.TCLKS = 0;
	
	return this;
}

long _rotary_a::close(_low_file_desc_class* desc)
{
	return 0;
}

long _rotary_a::read(unsigned char *buf,long count)
{
	return -1;
}

long _rotary_a::write(const unsigned char *buf,long count)
{
	return -1;
}

long _rotary_a::ioctl(unsigned long request,void* attr)
{
	switch(request){
	case ROTARYA_BEGIN:
		return begin(attr);
	case ROTARYA_GET_COUNT:
		return (long)MTU1.TCNT;
	default:
		return -1;
	}
	return 0;
}

long _rotary_a::begin(void* argp){
	MTUA.TSTR.BIT.CST0 = 1;
	MTUA.TSTR.BIT.CST1 = 1;
}