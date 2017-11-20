
#include"L3G4200D.hpp"
#include"CentralLibrary.h"

void* i_GyroAnalysis(thread_t *tid,void *attr){
	//while(1){
		//thread_suspend(get_tid());
		L3G4200D *g_gyro = (L3G4200D*)attr;
		g_gyro->Processing();
	//}
}
