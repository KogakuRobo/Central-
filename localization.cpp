#include "localization.hpp"

#include <math.h>

#include "L3G4200D.hpp"
#include "../SPI/_rx621_RSPI1.hpp"
#include "../cmt1.hpp"

#include "RotaryA.hpp"
#include "RotaryB.hpp"

extern void i_GyroAnalysis(void);

RSPI1_Bus *g_spi_bus;
L3G4200D *g_gyro;

int rotary_a;
int rotary_b;

void* localization_init(void)
{
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
	
	rotary_a = open("ROTARY_A",0,0);
	ioctl(rotary_a,ROTARYA_SET_TGIA,&stc);
	ioctl(rotary_a,ROTARYA_BEGIN,NULL);
	
	rotary_b = open("ROTARY_B",0,0);
	ioctl(rotary_b,ROTARYB_BEGIN,NULL);
	
	return 0;
}

extern long kernel_time;

enum{
	Y,
	X,
};
void* localization(thread_t* tid,void *attr){
	float yaw = g_gyro->getYaw();
	long count[2] = {ioctl(rotary_a,ROTARYA_GET_COUNT,NULL),ioctl(rotary_b,ROTARYB_GET_COUNT,NULL)};
	static float point[2] = {0,0};
	static long b_count[2] = {0,0};
	
	long hensa[2];
	
	((data*)attr)->yaw = yaw;
	((data*)attr)->count_A = count[Y];
	((data*)attr)->count_B = count[X];
	
	hensa[X] = count[X] - b_count[X];
	hensa[Y] = count[Y] - b_count[Y];
	
	point[X] += (hensa[X]*cos(yaw) - hensa[Y]*sin(yaw))*3.6816E-05;
	point[Y] += (hensa[Y]*cos(yaw) + hensa[X]*sin(yaw))*3.6816E-05;
	
	((data*)attr)->X = point[X];
	((data*)attr)->Y = point[Y];
	
	b_count[X] = count[X];
	b_count[Y] = count[Y];
	return NULL;
}