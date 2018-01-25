#ifndef _localization_HPP_
#define _localization_HPP_

#include "CentralLibrary.h"
//#include "Gyro.hpp"
#include "L3G4200D.hpp"
#include "RotaryClass.hpp"

//���Ȉʒu����p�v���O����

//�d�l���W���[��
//CMT1	(�W���C���Z���T�̃f�[�^�擾���������j
//RSPI1�@�i�W���C���Z���T�[�Ƃ̒ʐM�j

class Localization
{
private:
	
	SPI_Bus *spi_bus;
	L3G4200D *yaw_gyro;

	Rotary* rotaryc_a;
	Rotary* rotaryc_b;
	
	float yaw;
	long count_A;
	long count_B;
	
	float X;
	float Y;
	
	long encorder_ppr;
	
	//parameter_K :�G���R�[�_�̃J�E���g����������t����ꂽ�^�C���̐ڐ����x���Z�o���邽�߂̕ϐ� ,1�p���X������̋����u���v
	//parameter_K = r * pi / ( 2 * PPR)
	//r: �^�C�����a[m]
	//pi : �~����
	//PPR : �G���R�[�_�̃p���X��[pulse per revolve]
	// = 0.024[m] * 3.1415926535 / ( 2 * 1024)
	float parameter_K;
	
	static void* localization(thread_t*,void*);
public:
	Localization(void);
	~Localization(void);
	int Begin(void);
	
	virtual float GetX(void);			//[m]
	virtual float GetY(void);			//[m]
	virtual float GetYaw(void);			//[rad]
	
	void set_encorder_ppr(long p);
};

#endif