#include "MotorC.hpp"
#include "CentralLibrary.h"
#include "lowsrc.hpp"

class _motor_c:public _low_file_desc_factor,public _low_file_desc_class{
	static const unsigned char g_motor_c_path[];
	
	float duty;
	
public:
	_motor_c(void);
	virtual const unsigned char* get_name(void){return this->g_motor_c_path;}
	virtual _low_file_desc_class* open(const char*,long);
	virtual long close(_low_file_desc_class*);
	
	virtual long read(unsigned char*,long);
	virtual long write(const unsigned char*,long);
	
	virtual long ioctl(unsigned long,void*);

protected:
	void set_duty(float);
}Motor_C;


const unsigned char _motor_c::g_motor_c_path[] = "MOTOR_C";

_motor_c::_motor_c(void)
{
	MSTP(MTU10) = 0;
	duty = 0.0;
	
	set_io_driver(&Motor_C);
}

_low_file_desc_class* _motor_c::open(const char* name,long mode)
{	
	PORTB.DDR.BYTE = 0xff;
	PORT2.DR.BIT.B2 = 0;
	PORT2.DR.BIT.B4 = 0;
	
	MTUB.TRWER.BIT.RWE = 1;
	
	MTU10.TCR.BIT.CCLR = 1;
	MTU10.TCR.BIT.CKEG = 0;
	MTU10.TCR.BIT.TPSC = 0;
	
	MTU10.TMDR.BIT.MD = 2;
	
	MTU10.TIORH.BIT.IOA = 6;
	MTU10.TIORH.BIT.IOB = 5;
	MTU10.TIORL.BIT.IOC = 6;
	MTU10.TIORL.BIT.IOD = 5;
	
	MTU10.TGRA = 1000;
	MTU10.TGRC = 1000;
	MTU10.TGRB = 0;
	MTU10.TGRD = 0;
	
	MTUB.TSTR.BIT.CST4 = 1;
	
	return this;
}

long _motor_c::close(_low_file_desc_class* desc)
{
	return 0;
}

long _motor_c::read(unsigned char *buf,long count)
{
	return -1;
}

long _motor_c::write(const unsigned char *buf,long count)
{
	return -1;
}

long _motor_c::ioctl(unsigned long request,void* attr)
{
	switch(request){
	case MOTORC_SET_DUTY:
		this->set_duty(*(float *)attr);
		break;
	}
	return 0;
}

void _motor_c::set_duty(float d)
{
	float abs_duty;
	this->duty = d;
	if(duty < 0){
		
		abs_duty = -1.0* duty;
		PORT2.DR.BIT.B2 = 0;
		PORT2.DR.BIT.B4 = 1;
	}
	else if(duty > 0){
		abs_duty = duty;
		PORT2.DR.BIT.B2 = 1;
		PORT2.DR.BIT.B4 = 0;
	}
	else {
		abs_duty = 0;
		PORT2.DR.BIT.B2 = 0;
		PORT2.DR.BIT.B4 = 0;
	}
	MTU10.TGRB = MTU10.TGRA * (abs_duty / 100.0);
}