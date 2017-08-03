#include "MotorA.hpp"
#include "CentralLibrary.h"
#include "lowsrc.hpp"

class _motor_a:public _low_file_desc_factor,public _low_file_desc_class{
	static const unsigned char g_motor_a_path[];
	
	float duty;
	
public:
	_motor_a(void);
	virtual const unsigned char* get_name(void){return this->g_motor_a_path;}
	virtual _low_file_desc_class* open(const char*,long);
	virtual long close(_low_file_desc_class*);
	
	virtual long read(unsigned char*,long);
	virtual long write(const unsigned char*,long);
	
	virtual long ioctl(unsigned long,void*);

protected:
	void set_duty(float);
}Motor_A;

const unsigned char _motor_a::g_motor_a_path[] = "MOTOR_A";

_motor_a::_motor_a(void)
{
	MSTP(MTU9) = 0;
	duty = 0.0;
	
	set_io_driver(&Motor_A);
}

_low_file_desc_class* _motor_a::open(const char* name,long mode)
{	
	PORTB.DDR.BYTE = 0xff;
	PORTB.DR.BIT.B2 = 0;
	PORTB.DR.BIT.B3 = 0;
	
	MTUB.TRWER.BIT.RWE = 1;
	
	MTU9.TCR.BIT.CCLR = 1;
	MTU9.TCR.BIT.CKEG = 0;
	MTU9.TCR.BIT.TPSC = 0;
	
	MTU9.TMDR.BIT.MD = 2;
	
	MTU9.TIORH.BIT.IOA = 6;
	MTU9.TIORH.BIT.IOB = 5;
	MTU9.TIORL.BIT.IOC = 6;
	MTU9.TIORL.BIT.IOD = 5;
	
	MTU9.TGRA = 1000;
	MTU9.TGRC = 1000;
	MTU9.TGRB = 0;
	MTU9.TGRD = 0;
	
	MTUB.TSTR.BIT.CST3 = 1;
	
	return this;
}

long _motor_a::close(_low_file_desc_class* desc)
{
	return 0;
}

long _motor_a::read(unsigned char *buf,long count)
{
	return -1;
}

long _motor_a::write(const unsigned char *buf,long count)
{
	return -1;
}

long _motor_a::ioctl(unsigned long request,void* attr)
{
	switch(request){
	case MOTORA_SET_DUTY:
		this->set_duty(*(float *)attr);
		break;
	}
	return 0;
}

void _motor_a::set_duty(float d)
{
	float abs_duty;
	this->duty = d;
	if(duty < 0){
		
		abs_duty = -1.0* duty;
		PORTB.DR.BIT.B2 = 0;
		PORTB.DR.BIT.B3 = 1;
	}
	else if(duty > 0){
		abs_duty = duty;
		PORTB.DR.BIT.B2 = 1;
		PORTB.DR.BIT.B3 = 0;
	}
	else {
		abs_duty = 0;
		PORTB.DR.BIT.B2 = 0;
		PORTB.DR.BIT.B3 = 0;
	}
	MTU9.TGRB = MTU9.TGRA * (abs_duty / 100.0);
}