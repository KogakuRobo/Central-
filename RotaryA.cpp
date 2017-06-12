#include "RotaryA.hpp"
#include "CentralLibrary.h"
#include "lowsrc.hpp"



class _rotary_a:public _low_file_desc_factor,public _low_file_desc_class{
	static const unsigned char g_rotary_a_path[];

	//MTU1.TCNTのオーバーフローカウンタ
	static unsigned short over_count;
public:
	static thread_t thread;
private:
	void* (*function)(thread_t*,void*);
	void* arg;
public:
	_rotary_a(void);
	virtual const unsigned char* get_name(void){return this->g_rotary_a_path;}
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
	long set_tgia(void *argp);
	
	static void* mtu_tgia_inte(thread_t* tid,void *attr);
}Rotary_a;

const unsigned char _rotary_a::g_rotary_a_path[] = "ROTARY_A";
unsigned short _rotary_a::over_count;
thread_t _rotary_a::thread;

_rotary_a::_rotary_a(void){
	MSTP(MTU0) = 0;
	MSTP(MTU1) = 0;
	
	over_count = 0;
	
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
	
	MTU1.TIER.BIT.TCIEU = 1;
	MTU1.TIER.BIT.TCIEV = 1;
	
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
		return get_count(attr);
	case ROTARYA_SET_TGIA:
		return set_tgia(attr);
	default:
		return -1;
	}
	return 0;
}

long _rotary_a::begin(void* argp){
	
	IPR(MTU1,TCIV1) = 15;
	IPR(MTU1,TCIU1) = 15;
	IEN(MTU1,TCIV1) = 1;
	IEN(MTU1,TCIU1) = 1;
	
	IPR(MTU0,TGIA0) = 13;
	IEN(MTU0,TGIA0) = 1;
	
	MTUA.TSTR.BIT.CST0 = 1;
	MTUA.TSTR.BIT.CST1 = 1;
	return 0;
}

long _rotary_a::get_count(void *argp){
	return ((unsigned long)(over_count << 16)) | ((unsigned long)MTU1.TCNT);
}

long _rotary_a::set_tgia(void *argp){
	interrupt_stc *p_stc = (interrupt_stc *)argp;
	function = p_stc->function;
	arg = p_stc->argp;
	thread_create(&this->thread,p_stc->attr,_rotary_a::mtu_tgia_inte,p_stc->argp);
	IEN(MTU0,TGIA0) = 1;
	return 0;
}

void* _rotary_a::mtu_tgia_inte(thread_t* tid,void *attr){
	while(!thread_suspend(tid)){
	IEN(MTU0,TGIA0) = 0;
		Rotary_a.function(tid,Rotary_a.arg);
	IEN(MTU0,TGIA0) = 1;
	}
}

#pragma interrupt MTU0_TGIA0(vect = VECT(MTU0,TGIA0))
void MTU0_TGIA0(void){
	thread_resume(&Rotary_a.thread);
}

#pragma interrupt MTU1_TCIV1(vect = VECT(MTU1,TCIV1))
void MTU1_TCIV1(void){
	Rotary_a.count_overflow();
}

#pragma interrupt MTU1_TCIU1(vect = VECT(MTU1,TCIU1))
void MTU1_TCIU1(void){
	Rotary_a.count_underflow();
}