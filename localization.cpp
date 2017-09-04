#include "localization.hpp"

#include <math.h>

#include "L3G4200D.hpp"
#include "../SPI/_rx621_RSPI1.hpp"
#include "../cmt1.hpp"

#include "RotaryClass.hpp"
#include "RotaryA.hpp"
#include "RotaryB.hpp"

extern void i_GyroAnalysis(void);

RSPI1_Bus *g_spi_bus;
L3G4200D *g_gyro;

int rotary_a;
int rotary_b;

Rotary *rotaryc_a;
Rotary *rotaryc_b;

volatile loca_data d;

Localization::Localization(void)
{
	localization_init();
	return;
}

Localization::~Localization(void)
{
	return;
}

/*
int Localization::Begin(void)
{
	this->spi_bus = new RSPI1_Bus;
	spi_bus->Begin(0);
	
	this->yaw_gyro = new L3G4200D(*spi_bus);
	if(yaw_gyro->Begin(0) != 0){
		return NULL;
	}
	static cmt1_timer timer;
	timer.set_timer(2500,CT_PRIORITY_MAX,(void *(*)(thread_t*,void*))i_GyroAnalysis,NULL);
	
	while(!yaw_gyro.isNewData());	//ジャイロデータ更新確認
	
}
*/

float Localization::GetX(void)
{
	return d.X;
}

float Localization::GetY(void)
{
	return d.Y;
}

float Localization::GetYaw(void)
{
	return d.yaw;
}

volatile loca_data &Localization::Get_d(void)
{
	return d;
}
	

void* localization_init(void)
{
	//static thread_t loca;
	//thread_create(&loca,CT_PRIORITY_MAX + 3,localization,(void*)&d);
	
	static RSPI1_Bus spi_bus;
	spi_bus.Begin(0);
	
	static L3G4200D gyro(spi_bus);
	if(gyro.Begin(0) != 0){
		return NULL;
	}
	g_gyro = &gyro;
	
	//ジャイロの更新タスクの呼び出し
	static cmt1_timer timer;
	timer.set_timer(2500,CT_PRIORITY_MAX,(void *(*)(thread_t*,void*))i_GyroAnalysis,NULL);
	
	while(!gyro.isNewData());	//ジャイロデータ更新確認
	
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
	
	return 0;
}

extern long kernel_time;

enum{
	Y,
	X,
};

void* localization(thread_t* tid,void *attr){
	float yaw = g_gyro->getYaw();
	long count[2] = {rotaryc_a->GetCount(),rotaryc_b->GetCount()};		//この変数は同時性を保証しなければならないのでアクセス方法が特殊
	static float point[2] = {0,0};
	static long b_count[2] = {0,0};
	
	const float parameter_K = 7.540E-5;
	//parameter_K :エンコーダのカウント差分から取り付けられたタイヤの接線速度を算出するための変数 
	//parameter_K = r * pi / ( 2 * PPR)
	//r: タイヤ半径[m]
	//pi : 円周率
	//PPR : エンコーダのパルス数[pulse per revolve]
	// = 0.024[m] * 3.1415926535 / ( 2 * 1024)
	
	static long hensa[2];
	
	((loca_data*)attr)->yaw = yaw;
	((loca_data*)attr)->count_A = count[Y];
	((loca_data*)attr)->count_B = count[X];
	
	hensa[X] = count[X] - b_count[X];
	hensa[Y] = count[Y] - b_count[Y];
	
	point[X] += (hensa[X]*cos(yaw) - hensa[Y]*sin(yaw))*parameter_K;
	point[Y] += (hensa[Y]*cos(yaw) + hensa[X]*sin(yaw))*parameter_K ;
	
	((loca_data*)attr)->X = point[X];
	((loca_data*)attr)->Y = point[Y];
	
	b_count[X] = count[X];
	b_count[Y] = count[Y];
	return NULL;
}