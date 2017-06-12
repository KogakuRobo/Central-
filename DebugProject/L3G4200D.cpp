#include"L3G4200D.hpp"
#include<stdlib.h>
#include<math.h>

L3G4200D::L3G4200D(SPI_Bus &_b){
	spi_bus = &_b;
	yaw = 0;
	gcs.average = 0;
	gcs.deviation = 0;
	gcs.p_buffer = NULL;
	
	mode = INITIALIZE;

}

int L3G4200D::Begin(unsigned int attr){
	
	if(this->ReadWhoAmI() != 0xD3)return -1;		//Who Am I が正常に帰らない場合エラー
	
//	CTRL_REG1
//	ODR = 400
//	Cutoff 50
//	WakeUP and Z Enable
//	X,Y disable
	WriteCTRL_REG1(L3G4200D_REG1_ODR_400 | L3G4200D_REG1_BW_0 | L3G4200D_REG1_POWER | L3G4200D_REG1_Z_ENABLE);
	mode= CALIBRATION;									//初期化に伴い、キャリブレーションモードに移行
	gcs.p_buffer = (signed short*)malloc(NUMBER_OF_CALIBRATION * sizeof(signed short));	//キャリブレーションを行うバッファの動的確保

//	CTRL_REG2
//	HPF Cutoff 1
	WriteCTRL_REG2(0x05);

	WriteCTRL_REG4(L3G4200D_REG4_FS_500);
	WriteCTRL_REG5(0x00);
	new_data = 0;

	return 0;
}

unsigned short L3G4200D::OUT_Z(void){
	unsigned short z;
	this->RegisterRead(0x2C,(unsigned char*)&z,2);
	return z;
}

float L3G4200D::getYaw(void){
	new_data = 0;
	return yaw;
}
	
signed short L3G4200D::getZ(void){
	return z;
}

signed short L3G4200D::getAverage(void)
{
	return this->gcs.average;
}

signed short L3G4200D::getDeviation(void)
{
	return this->gcs.deviation;
}

bool L3G4200D::Calibration(signed short z)
{
	static float time = 0;
	static int number_loop = 0;
	static signed long sum = 0;
	
	if(time < 250.0){			//250ms 待ち
		time += 1.0 / 400.0 * 1000.0;
		return 1;
	}
	else if(number_loop < NUMBER_OF_CALIBRATION){
		gcs.p_buffer[number_loop] = z;
		sum += z;
		if(z > this->gcs.max)
			this->gcs.max = z;
		if(z < this->gcs.min)
			this->gcs.min = z;
		number_loop++;
		return 1;
	}
	else if(number_loop == NUMBER_OF_CALIBRATION){
		this->gcs.average = sum / NUMBER_OF_CALIBRATION;
		sum = 0;
		
		for(int i = 0; i < NUMBER_OF_CALIBRATION;i++){
			sum += pow(gcs.p_buffer[i] - this->gcs.average,2);
		}
			
		this->gcs.deviation = sqrt(sum / NUMBER_OF_CALIBRATION);
		
		number_loop = NUMBER_OF_CALIBRATION + 1;
		mode = RUN;
		free(gcs.p_buffer);
		
		return 0;
	}
	else
		return 0;
}

signed short L3G4200D::Filting(signed short z)
{
	signed short ret;
	if(z > this->gcs.average + (signed short)(this->gcs.deviation * 2.75))
		ret = z - this->gcs.average - (this->gcs.deviation * 2.75);
	else if( z < this->gcs.average - (signed short)(this->gcs.deviation * 2.75))
		ret = z - this->gcs.average + (this->gcs.deviation * 2.75);
	else 
		ret = 0;
	return ret;
}

void L3G4200D::Processing(void)
{
	static signed short befor_z = 0;
	float t,zd;
	static float e = 0;
	
	this->z = this->OUT_Z();
	if(isCalibration()){
		this->Calibration(z);
		return;
	}
	
	this->z = Filting(z);
	zd = (this->z + befor_z) * 17.5 / 1000 / 400.0 * 0.5 * 3.141592653589 / 180.0;
	t = this->yaw + (zd + e);
	e = (zd + e) - (t - this->yaw);
	this->yaw = t;
	
	new_data= 1;

	befor_z = z;
	
}

bool L3G4200D::isCalibration(void)
{
	return mode == CALIBRATION;
}

bool L3G4200D::isNewData(void)
{
	return new_data == 1;
}

unsigned char L3G4200D::ReadWhoAmI(void)
{
	unsigned char ret;
	this->RegisterRead(L3G4200D_WHO_AM_I_ADDR,&ret,1);
	return ret;
}

void L3G4200D::WriteCTRL_REG1(unsigned char data)
{
	this->RegisterWrite(L3G4200D_REG1_ADDR,&data,1);
}

void L3G4200D::WriteCTRL_REG2(unsigned char data)
{
	this->RegisterWrite(L3G4200D_REG2_ADDR,&data,1);
}

void L3G4200D::WriteCTRL_REG3(unsigned char data)
{
	this->RegisterWrite(L3G4200D_REG3_ADDR,&data,1);
}

void L3G4200D::WriteCTRL_REG4(unsigned char data)
{
	this->RegisterWrite(L3G4200D_REG4_ADDR,&data,1);
}

void L3G4200D::WriteCTRL_REG5(unsigned char data)
{
	this->RegisterWrite(L3G4200D_REG5_ADDR,&data,1);
}

int L3G4200D::RegisterWrite(unsigned char addr,unsigned char *tx,int len)
{
	unsigned char buff[25];
	unsigned char MSbit = 0;
	if(len > 1)MSbit = 0x40;
	buff[0] = addr | MSbit | 0x00;
	for(int i =0;i < len;i++){
		buff[i + 1] = tx[i];
	}
	spi_bus->DataRW(0,buff,NULL,len + 1);
	return 0;
}

int L3G4200D::RegisterRead(unsigned char addr,unsigned char *rx,int len)
{
	unsigned char buff_tx[25];
	unsigned char buff_rx[25];
	unsigned char MSbit = 0;
	if(len > 1)MSbit = 0x40;
	buff_tx[0] = addr | MSbit | 0x80;

	spi_bus->DataRW(0,buff_tx,buff_rx,len + 1);
	for(int i =0;i < len;i++){
		rx[i] = buff_rx[i + 1];
	}
	return 0;
}