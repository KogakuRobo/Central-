#ifndef _servo_HPP_
#define _servo_HPP_

#include "CentralLibrary.h"

class servo{
public:
	servo(void);
	virtual void set_pos(float) = 0;
};

class servo_a{
	void set_frequency(unsigned short f){
		const unsigned short MTU6_PSC[4] = {1,4,16,64};
		//this->stop();
		for(int i = 0;i < 4;i++){
		
			long temp = (PCK_CLOCK * 1000000) / MTU6_PSC[i] / ( f) - 1;	//プリスケーラを想定して計算
		
			if(temp < ((unsigned short)0xffff)){					//TGRAが設定可能範囲か計算
				MTU6.TCR.BIT.TPSC = i;						//設定
				MTU6.TGRA = (unsigned short)temp;
				return IOCTL_NON_ERROR;
			}
		}
	}
public:
	servo_a(void){
		MSTP(MTU6) = 0;
		
		PORTA.DDR.BIT.B0 = 1;
		MTU6.TMDR.BIT.MD = 2;
		
		MTU6.TIORH.BIT.IOA = 6;
		MTU6.TIORH.BIT.IOB = 5;
	
		MTU6.TCR.BIT.CCLR = 1;
		
		this->set_frequency(50);
		
		MTUB.TSTR.BIT.CST0 = 1;
	}
	void set_pos(float d){
		float ads_duty;
		
		if(duty < 0){
			abs_duty = -1 * d;
		}
		else abs_duty = d;
		MTU6.TGRB = MTU6.TGRA * (abs_duty / 100.0);
	}
	
};

class servo_b{
	void set_frequency(unsigned short f){
		const unsigned short MTU6_PSC[4] = {1,4,16,64};
		//this->stop();
		for(int i = 0;i < 4;i++){
		
			long temp = (PCK_CLOCK * 1000000) / MTU6_PSC[i] / ( f) - 1;	//プリスケーラを想定して計算
		
			if(temp < ((unsigned short)0xffff)){					//TGRAが設定可能範囲か計算
				MTU6.TCR.BIT.TPSC = i;						//設定
				MTU6.TGRC = (unsigned short)temp;
				return IOCTL_NON_ERROR;
			}
		}
	}
public:
	servo_b(void){
		MSTP(MTU6) = 0;
		
		PORTA.DDR.BIT.B2 = 1;
		MTU6.TMDR.BIT.MD = 2;
		
		MTU6.TIORH.BIT.IOC = 6;
		MTU6.TIORH.BIT.IOD = 5;
	
		MTU6.TCR.BIT.CCLR = 1;
		
		this->set_frequency(50);
		
		MTUB.TSTR.BIT.CST0 = 1;
	}
	void set_pos(float d){
		float ads_duty;
		
		if(duty < 0){
			abs_duty = -1 * d;
		}
		else abs_duty = d;
		MTU6.TGRD = MTU6.TGRC * (abs_duty / 100.0);
	}
};

class servo_c{
	void set_frequency(unsigned short f){
		const unsigned short MTU7_PSC[7] = {1,4,16,64,1,1,256};
		//this->stop();
		for(int i = 0;i < 7;i++){
		
			long temp = (PCK_CLOCK * 1000000) / MTU7_PSC[i] / ( f) - 1;	//プリスケーラを想定して計算
		
			if(temp < ((unsigned short)0xffff)){					//TGRAが設定可能範囲か計算
				MTU7.TCR.BIT.TPSC = i;						//設定
				MTU7.TGRA = (unsigned short)temp;
				return IOCTL_NON_ERROR;
			}
		}
	}
public:
	servo_c(void){
		MSTP(MTU7) = 0;
		
		PORTA.DDR.BIT.B4 = 1;
		MTU7.TMDR.BIT.MD = 2;
		
		MTU7.TIOR.BIT.IOA = 6;
		MTU7.TIOR.BIT.IOB = 5;
	
		MTU7.TCR.BIT.CCLR = 1;
		
		this->set_frequency(50);
		
		MTUB.TSTR.BIT.CST1 = 1;
	}
	void set_pos(float d){
		float ads_duty;
		
		if(duty < 0){
			abs_duty = -1 * d;
		}
		else abs_duty = d;
		MTU7.TGRB = MTU7.TGRA * (abs_duty / 100.0);
	}
};

class servo_d{
	void set_frequency(unsigned short f){
		const unsigned short MTU8_PSC[8] = {1,4,16,64,1,1,1,1024};
		//this->stop();
		for(int i = 0;i < 7;i++){
		
			long temp = (PCK_CLOCK * 1000000) / MTU8_PSC[i] / ( f) - 1;	//プリスケーラを想定して計算
		
			if(temp < ((unsigned short)0xffff)){					//TGRAが設定可能範囲か計算
				MTU8.TCR.BIT.TPSC = i;						//設定
				MTU8.TGRA = (unsigned short)temp;
				return IOCTL_NON_ERROR;
			}
		}
	}
public:
	servo_c(void){
		MSTP(MTU8) = 0;
		
		PORTA.DDR.BIT.B6 = 1;
		MTU8.TMDR.BIT.MD = 2;
		
		MTU8.TIOR.BIT.IOA = 6;
		MTU8.TIOR.BIT.IOB = 5;
	
		MTU8.TCR.BIT.CCLR = 1;
		
		this->set_frequency(50);
		
		MTUB.TSTR.BIT.CST2 = 1;
	}
	void set_pos(float d){
		float ads_duty;
		
		if(duty < 0){
			abs_duty = -1 * d;
		}
		else abs_duty = d;
		MTU8.TGRB = MTU8.TGRA * (abs_duty / 100.0);
	}
};

#endif