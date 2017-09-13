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
	
	virtual long ioctl(unsigned long,void*);

protected:
	virtual int begin(void);
	virtual int stop(void);
	virtual int set_duty(float);
	virtual int set_frequency(unsigned short);
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
	//
	this->stop();
	
	//port setting
	PORTB.DDR.BIT.B0 = 1;
	PORTB.DDR.BIT.B2 = 1;
	PORTB.DDR.BIT.B3 = 1;
	PORTB.DR.BIT.B2 = 0;
	PORTB.DR.BIT.B3 = 0;
	
	//MTU9 register write enable
	MTUB.TRWER.BIT.RWE = 1;
	
	//PWM mode 1
	MTU9.TMDR.BIT.MD = 2;
	
	MTU9.TIORH.BIT.IOA = 6;
	MTU9.TIORH.BIT.IOB = 5;
	
	MTU9.TCR.BIT.CCLR = 1;

	this->set_frequency(20);		//20kHz
	this->set_duty(0);
	
	this->begin();
	
	return this;
}

long _motor_a::close(_low_file_desc_class* desc)
{
	this->stop();
	
	PORTB.DR.BIT.B2 = 0;
	PORTB.DR.BIT.B3 = 0;
	PORTB.DDR.BIT.B0 = 0;
	PORTB.DDR.BIT.B2 = 0;
	PORTB.DDR.BIT.B3 = 0;
	
	return CLOSE_NON_ERROR;
}

long _motor_a::ioctl(unsigned long request,void* attr)
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

int _motor_a::begin(void)
{
	MTUB.TSTR.BIT.CST3 = 1;
	return IOCTL_NON_ERROR;
}

int _motor_a::stop(void)
{
	MTUB.TSTR.BIT.CST3 = 0;
	return IOCTL_NON_ERROR;
}

int _motor_a::set_duty(float d)
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
	return IOCTL_NON_ERROR;
}

int _motor_a::set_frequency(unsigned short f){
	const unsigned short MTU9_PSC[6] = {1,4,16,64,256,1024};
	this->stop();
	for(int i = 0;i < 6;i++){
		
		long temp = (PCK_CLOCK * 1000000) / MTU9_PSC[i] / ( f * 1000) - 1;	//プリスケーラを想定して計算
		
		if(temp < ((unsigned short)0xffff)){					//TGRAが設定可能範囲か計算
			MTU9.TCR.BIT.TPSC = i;						//設定
			MTU9.TGRA = (unsigned short)temp;
			return IOCTL_NON_ERROR;
		}
	}
	
	return IOCTL_ERROR;
}