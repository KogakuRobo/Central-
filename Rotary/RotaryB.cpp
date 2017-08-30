#include "RotaryB.hpp"
#include "CentralLibrary.h"
#include "lowsrc.hpp"



class _rotary_b:public _low_file_desc_factor,public _low_file_desc_class{
	static const unsigned char g_rotary_b_path[];

	//MTU1.TCNTのオーバーフローカウンタ
	static unsigned short over_count;
public:
	_rotary_b(void);
	virtual const unsigned char* get_name(void){return this->g_rotary_b_path;}
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
}Rotary_b;

const unsigned char _rotary_b::g_rotary_b_path[] = "ROTARY_B";
unsigned short _rotary_b::over_count;

_rotary_b::_rotary_b(void){
	MSTP(MTU6) = 0;
	MSTP(MTU7) = 0;
	
	over_count = 0;
	
	set_io_driver(&Rotary_b);
}

_low_file_desc_class* _rotary_b::open(const char* name,long mode)
{
	//MTU0.TCNTを24MHzカウントアップ(48MHz / 4 * 2)設定
	MTU6.TCR.BIT.TPSC = 1;
	MTU6.TCR.BIT.CKEG = 2;
	MTU6.TCR.BIT.CCLR = 1;
	
	MTU6.TMDR.BIT.BFB = 1;
	MTU6.TMDR.BIT.MD = 0;
	
	MTU6.TIORH.BIT.IOA = 0;
	MTU6.TIORH.BIT.IOB = 0xc;
	
	MTU6.TIORL.BIT.IOC = 0;
	MTU6.TIORL.BIT.IOD = 0;
	
	MTU6.TIER.BIT.TGIEA = 1;
	MTU6.TIER.BIT.TGIEC = 1;
	
	MTU6.TGRA = 60000 - 1;
	MTU6.TGRC = 30000 - 1;
	
	MTU7.TCR.BIT.CCLR = 0;
	MTU7.TMDR.BIT.MD = 4;
	
	MTU7.TIOR.BIT.IOB = 0xc;
	MTU7.TIOR.BIT.IOA = 0xc;
	
	MTU7.TIER.BIT.TCIEU = 1;
	MTU7.TIER.BIT.TCIEV = 1;
	
	PORTC.DDR.BIT.B2 = 0;
	PORTC.DDR.BIT.B3 = 0;
	
	PORTC.PCR.BIT.B2 = 1;
	PORTC.PCR.BIT.B3 = 1;
	
	PORTC.ICR.BIT.B2 = 1;
	PORTC.ICR.BIT.B3 = 1;
	
	IOPORT.PFDMTU.BIT.TCLKS = 0;
	
	return this;
}

long _rotary_b::close(_low_file_desc_class* desc)
{
	return 0;
}

long _rotary_b::read(unsigned char *buf,long count)
{
	return -1;
}

long _rotary_b::write(const unsigned char *buf,long count)
{
	return -1;
}

long _rotary_b::ioctl(unsigned long request,void* attr)
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

long _rotary_b::begin(void* argp){
	
	IPR(MTU7,TCIV7) = 15;
	IPR(MTU7,TCIU7) = 15;
	IEN(MTU7,TCIV7) = 1;
	IEN(MTU7,TCIU7) = 1;
	
	MTUB.TSTR.BIT.CST0 = 1;
	MTUB.TSTR.BIT.CST1 = 1;
	return 0;
}

long _rotary_b::get_count(void *argp){
	return ((unsigned long)(over_count << 16)) | ((unsigned long)MTU7.TCNT);
}



#pragma interrupt MTU7_TCIV7(vect = VECT(MTU7,TCIV7))
void MTU7_TCIV7(void){
	Rotary_b.count_overflow();
}

#pragma interrupt MTU7_TCIU7(vect = VECT(MTU7,TCIU7))
void MTU7_TCIU7(void){
	Rotary_b.count_underflow();
}