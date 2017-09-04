#include "MotorB.hpp"
#include "CentralLibrary.h"
#include "lowsrc.hpp"

class _motor_b:public _low_file_desc_factor,public _low_file_desc_class{
	static const unsigned char g_motor_b_path[];
	
	float duty;
	
public:
	_motor_b(void);
	virtual const unsigned char* get_name(void){return this->g_motor_b_path;}
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
}Motor_B;

const unsigned char _motor_b::g_motor_b_path[] = "MOTOR_B";

_motor_b::_motor_b(void)
{
	MSTP(MTU9) = 0;
	duty = 0.0;
	
	set_io_driver(&Motor_B);
}

_low_file_desc_class* _motor_b::open(const char* name,long mode)
{	
	
	this->stop();
	
	//port setting
	PORTB.DDR.BIT.B1 = 1;
	PORTB.DDR.BIT.B6 = 1;
	PORTB.DDR.BIT.B7 = 1;
	PORTB.DR.BIT.B6 = 0;
	PORTB.DR.BIT.B7 = 0;
	
	MTUB.TRWER.BIT.RWE = 1;
	
	MTU9.TMDR.BIT.MD = 2;
	
	MTU9.TIORL.BIT.IOC = 6;
	MTU9.TIORL.BIT.IOD = 5;
	
	MTU9.TCR.BIT.CCLR = 1;
	
	this->set_frequency(20);
	this->set_duty(0);
	
	this->begin();
	
	return this;
}

long _motor_b::close(_low_file_desc_class* desc)
{
	this->stop();
	
	PORTB.DR.BIT.B6 = 0;
	PORTB.DR.BIT.B7 = 0;
	PORTB.DDR.BIT.B1 = 0;
	PORTB.DDR.BIT.B6 = 0;
	PORTB.DDR.BIT.B7 = 0;
	
	return IOCTL_NON_ERROR;
}

long _motor_b::read(unsigned char *buf,long count)
{
	return -1;
}

long _motor_b::write(const unsigned char *buf,long count)
{
	return -1;
}

long _motor_b::ioctl(unsigned long request,void* attr)
{
	int ret;
	switch(request){
	case MOTOR_SET_DUTY:
		ret = this->set_duty(*(float *)attr);
		break;
	case MOTOR_SET_FREQUENCY:
		ret = this->set_frequency(*(unsigned short *)attr);
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

int _motor_b::begin(void)
{
	MTUB.TSTR.BIT.CST3 = 1;
	return IOCTL_NON_ERROR;
}

int _motor_b::stop(void)
{
	MTUB.TSTR.BIT.CST3 = 0;
	return IOCTL_NON_ERROR;
}

int _motor_b::set_duty(float d)
{
	float abs_duty;
	this->duty = d;
	if(duty < 0){
		
		abs_duty = -1.0* duty;
		PORTB.DR.BIT.B6 = 0;
		PORTB.DR.BIT.B7 = 1;
	}
	else if(duty > 0){
		abs_duty = duty;
		PORTB.DR.BIT.B6 = 1;
		PORTB.DR.BIT.B7 = 0;
	}
	else {
		abs_duty = 0;
		PORTB.DR.BIT.B6 = 0;
		PORTB.DR.BIT.B7 = 0;
	}
	MTU9.TGRD = MTU9.TGRA * (abs_duty / 100.0);
	return IOCTL_NON_ERROR;
}

int _motor_b::set_frequency(unsigned short f){
	const unsigned short MTU9_PSC[6] = {1,4,16,64,256,1024};
	this->stop();
	for(int i = 0;i < 6;i++){
		
		long temp = (PCK_CLOCK * 1000000) / MTU9_PSC[i] / ( f * 1000) - 1;	//プリスケーラを想定して計算
		
		if(temp < ((unsigned short)0xffff)){					//TGRAが設定可能範囲か計算
			MTU9.TCR.BIT.TPSC = i;						//設定
			MTU9.TGRC = (unsigned short)temp;
			return IOCTL_NON_ERROR;
		}
	}
	
	return IOCTL_ERROR;
}