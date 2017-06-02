#include "localization.hpp"

#include "L3G4200D.hpp"
#include "../SPI/_rx621_RSPI1.hpp"
#include "../cmt1.hpp"

L3G4200D *g_gyro;

extern void i_GyroAnalysis(void);

void* localization(thread_t* tid,void *attr){
	
	//SPIバスの初期化
	static RSPI1_Bus spi_bus;
	spi_bus.Begin(0);
	
	//gyroの初期化
	L3G4200D gyro(spi_bus);
	if(gyro.Begin(0) != 0){
		return NULL;
	}
	g_gyro = &gyro;
	
	//ジャイロの更新タスクの呼び出し
	cmt1_timer timer;
	timer.set_timer(2500,CT_PRIORITY_MAX,(void *(*)(thread_t*,void*))i_GyroAnalysis,NULL);
	
	volatile float yaw;
	
	while(1){
		msleep(3);			//ジャイロのデータ更新間隔が2.5ms
		while(!gyro.isNewData());	//ジャイロデータ更新確認
		
		yaw = gyro.getYaw() * 180.0 / 3.141592535;
		((data*)attr)->yaw = yaw;
		((data*)attr)->ave = gyro.getAverage();
		((data*)attr)->devia = gyro.getDeviation();
	}
	
	return NULL;
}