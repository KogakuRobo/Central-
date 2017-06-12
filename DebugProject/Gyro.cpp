
#include"L3G4200D.hpp"
#include"CentralLibrary.h"

extern L3G4200D *g_gyro;

void i_GyroAnalysis(void){
	//while(1){
		//thread_suspend(get_tid());
		g_gyro->Processing();
	//}
}
