#ifndef _L3G4200D_HPP_
#define _L3G4200D_HPP_


#include<stdio.h>

#include"Gyro.hpp"
#include"../SPI/SPI.hpp"

#include"L3G4200D_defined.hpp"

#define NUMBER_OF_CALIBRATION	500		//�L�����u���[�V�����ɗp����T���v�����O���B

class L3G4200D:public Gyro{
	
	unsigned char new_data;			//�X�V�t���O�B����̃f�[�^���X�V���ꂽ���ǂ����Ŏg���BgetYaw�ŃN���A
	float yaw;				//�v�Z�p�x
	signed short z;				//�擾Z�f�[�^�i���j
	
	SPI_Bus *spi_bus;			//�ʐM�o�X�N���X
	
	struct{
		signed short average;		//�L�����u���[�V�������̕��ϒl
		signed short deviation;		// �E�E		�@�@�@�̕΍�
		signed short max;		//			�̍ő�l
		signed short min;		//			�̍ŏ��l
		signed short* p_buffer;		//�΍��Z�o�̂��߂̃T���v�����O�l�ێ��o�b�t�@�ւ̃A�h���X�Bbegin���œ��I�m��
	}gcs;
	
	enum {
		INITIALIZE,
		CALIBRATION,
		RUN,
	}mode;					//�������
	
	FILE *fp;
public:
	L3G4200D(SPI_Bus&);
	int Begin(unsigned int);
	
	float getYaw(void);
	signed short getZ(void);
	signed short getAverage(void);
	signed short getDeviation(void);
	
	void Processing(void);
	
	bool isCalibration(void);
	bool isNewData(void);
private:
	int RegisterWrite(unsigned char,unsigned char *,int);
	int RegisterRead(unsigned char,unsigned char*,int);
	
	unsigned char ReadWhoAmI(void);
	void WriteCTRL_REG1(unsigned char);
	void WriteCTRL_REG2(unsigned char);
	void WriteCTRL_REG3(unsigned char);
	void WriteCTRL_REG4(unsigned char);
	void WriteCTRL_REG5(unsigned char);
	unsigned short OUT_Z(void);
	
	bool Calibration(signed short);
	signed short Filting(signed short);
};

#endif