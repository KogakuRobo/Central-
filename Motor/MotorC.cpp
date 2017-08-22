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
	virtual int begin(void);
	virtual int stop(void);
	virtual int set_duty(float);
	virtual int set_frequency(unsigned short);
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
	
	this->stop();
	
	
	PORTB.DDR.BIT.B4 = 1;
	PORT2.DDR.BIT.B2 = 1;
	PORT2.DDR.BIT.B4 = 1;
	PORT2.DR.BIT.B2 = 0;
	PORT2.DR.BIT.B4 = 0;
	MTUB.TOER.BIT.OE4A = 1;
	
	MTUB.TRWER.BIT.RWE = 1;
	
	MTU10.TMDR.BIT.MD = 2;
	
	MTU10.TIORH.BIT.IOA = 6;
	MTU10.TIORH.BIT.IOB = 5;
	
	MTU10.TCR.BIT.CCLR = 1;
	
	this->set_frequency(20);
	this->set_duty(0);
	
	this->begin();
	
	return this;
}

long _motor_c::close(_low_file_desc_class* desc)
{
	this->stop();
	
	PORT2.DR.BIT.B2 = 0;
	PORT2.DR.BIT.B4 = 0;
	PORTB.DDR.BIT.B4 = 0;
	PORT2.DDR.BIT.B2 = 0;
	PORT2.DDR.BIT.B4 = 0;
	MTUB.TOER.BIT.OE4A = 0;
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
	int ret;
	switch(request){
	case MOTOR_SET_DUTY:
		ret = this->set_duty(*(float *)attr);
		break;
	case MOTOR_SET_FREQUENCY:
		ret = this->set_frequency(*(unsigned short*)attr);
		break;
	case MOTOR_BEGIN:
		ret = this->begin();
		break;
	case MOTOR_STOP:
		ret = this->stop();
		break;
	default:
		ret = IOCTL_ERROR;
	}
	return ret;
}

int _motor_c::begin(void)
{
	MTUB.TSTR.BIT.CST4 = 1;
	return IOCTL_NON_ERROR;
}

int _motor_c::stop(void)
{
	MTUB.TSTR.BIT.CST4 = 0;
	return IOCTL_NON_ERROR;
}

int _motor_c::set_duty(float d)
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
	return IOCTL_NON_ERROR;
}

int _motor_c::set_frequency(unsigned short f){
	const unsigned short MTU10_PSC[6] = {1,4,16,64,256,1024};
	this->stop();
	for(int i = 0;i < 6;i++){
		
		long temp = (PCK_CLOCK * 1000000) / MTU10_PSC[i] / ( f * 1000) - 1;	//プリスケーラを想定して計算
		
		if(temp < ((unsigned short)0xffff)){					//TGRAが設定可能範囲か計算
			MTU10.TCR.BIT.TPSC = i;						//設定
			MTU10.TGRA = (unsigned short)temp;
			return IOCTL_NON_ERROR;
		}
	}
	
	return IOCTL_ERROR;
}