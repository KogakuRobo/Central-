#include "localization.hpp"

#include <math.h>

#include "L3G4200D.hpp"
#include "../SPI/_rx621_RSPI1.hpp"

#include "RotaryClass.hpp"
#include "RotaryA.hpp"
#include "RotaryB.hpp"

extern void* i_GyroAnalysis(thread_t*,void*);

Localization::Localization(void)
{
	set_encorder_ppr(2048);
	X = 0;
	Y = 0;
	yaw = 0;
	return;
}

Localization::~Localization(void)
{
	return;
}

int Localization::Begin(void)
{
	
	this->spi_bus = new RSPI1_Bus;
	this->spi_bus->Begin(0);
	
	this->yaw_gyro = new L3G4200D(*spi_bus);
	if(this->yaw_gyro->Begin(0) != 0){
		return NULL;
	}
	
	static cmt1_timer timer;
	timer.set_timer(2500,CT_PRIORITY_MAX,i_GyroAnalysis,this->yaw_gyro);
	
	while(!this->yaw_gyro->isNewData());	//ジャイロデータ更新確認
	
	
	interrupt_stc stc;
	stc.function = localization;
	stc.argp = (void*)this;
	stc.attr = CT_PRIORITY_MAX + 3;
	
	//rotary_a = open("ROTARY_A",0,0);
	//ioctl(rotary_a,ROTARYA_SET_TGIA,&stc);
	//close(rotary_a);
	
	rotaryc_a = new Rotary("ROTARY_A");
	rotaryc_a->SetTGIA(stc);
	rotaryc_b = new Rotary("ROTARY_B");
	
	//localization_init();
}

float Localization::GetX(void)
{
	return X;
}

float Localization::GetY(void)
{
	return Y;
}

float Localization::GetYaw(void)
{
	return yaw;
}

void Localization::set_encorder_ppr(long p){
	encorder_ppr = p;
	parameter_K = 0.0175 * 3.14159265 / (2 * this->encorder_ppr);
}

//void* Localization::localization_init(void)
//{
	//static thread_t loca;
	//thread_create(&loca,CT_PRIORITY_MAX + 3,localization,(void*)&d);
	
	/*/
	static RSPI1_Bus spi_bus;
	spi_bus.Begin(0);
	
	static L3G4200D gyro(spi_bus);
	if(gyro.Begin(0) != 0){
		return NULL;
	}
	g_gyro = &gyro;
	//*/
	
	/*/
	//ジャイロの更新タスクの呼び出し
	static cmt1_timer timer;
	timer.set_timer(2500,CT_PRIORITY_MAX,(void *(*)(thread_t*,void*))i_GyroAnalysis,NULL);
	
	while(!gyro.isNewData());	//ジャイロデータ更新確認
	//*/
	
	/*/
	interrupt_stc stc;
	stc.function = localization;
	stc.argp = (void*)&d;
	stc.attr = CT_PRIORITY_MAX + 3;
	
	//rotary_a = open("ROTARY_A",0,0);
	//ioctl(rotary_a,ROTARYA_SET_TGIA,&stc);
	//close(rotary_a);
	
	rotaryc_a = new Rotary("ROTARY_A");
	rotaryc_a->SetTGIA(stc);
	rotaryc_b = new Rotary("ROTARY_B");
	//*/
//	return 0;
//}

extern long kernel_time;

enum{
	ROTARY_Y,
	ROTARY_X,
};

enum{
	ROTARY_R,
	ROTARY_L,
};

void* Localization::localization(thread_t* tid,void *attr){
	Localization* This = (Localization*)attr;
	
	long count[2] = {This->rotaryc_b->GetCount(),This->rotaryc_a->GetCount()};		//この変数は同時性を保証しなければならないのでアクセス方法が特殊
	static float point[2] = {0,0};
	static long b_count[2] = {0,0};
	static long hensa[2];
	static long hensa_av=0;
	
	This->yaw =
			This->yaw_gyro->getYaw();
	/*This->count_A =
			count[ROTARY_Y];
	This->count_B =
			count[ROTARY_X];
	
	hensa[ROTARY_X] = count[ROTARY_X] - b_count[ROTARY_X];
	hensa[ROTARY_Y] = count[ROTARY_Y] - b_count[ROTARY_Y];
	
	point[ROTARY_X] += (hensa[ROTARY_X]*cos(This->yaw) - hensa[ROTARY_Y]*sin(This->yaw))*This->parameter_K;
	point[ROTARY_Y] += (hensa[ROTARY_Y]*cos(This->yaw) + hensa[ROTARY_X]*sin(This->yaw))*This->parameter_K ;
	
	This->X = point[ROTARY_X];
	This->Y = point[ROTARY_Y];
	
	b_count[ROTARY_X] = count[ROTARY_X];
	b_count[ROTARY_Y] = count[ROTARY_Y];
	return NULL;*/
	
	This->count_A =count[ROTARY_R];
	This->count_B =count[ROTARY_L];
	
	hensa[ROTARY_L] =-( count[ROTARY_L] - b_count[ROTARY_L]);
	hensa[ROTARY_R] = count[ROTARY_R] - b_count[ROTARY_R];
	
	hensa_av=(hensa[ROTARY_R]+hensa[ROTARY_L])/2;
	
	point[ROTARY_X] += hensa_av*sin(This->yaw)*This->parameter_K;
	point[ROTARY_Y] += hensa_av*cos(This->yaw)*This->parameter_K;
	
	This->X = -point[ROTARY_X];
	This->Y = point[ROTARY_Y];
	
	b_count[ROTARY_L] = count[ROTARY_L];
	b_count[ROTARY_R] = count[ROTARY_R];
	return NULL;
	
}