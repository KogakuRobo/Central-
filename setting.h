#ifndef _SETTING_H_		//define�ɂ���`
#define _SETIING_H_

#define pi 3.14159265359 
#define M_W_diameter 0.06 //���C���^�C���̒��a
#define S_W_diameter 0.15 //���[�^���[�p�^�C���̒��a[m]
#define L_wheel_distance 0.1908	//�@�̂����^�C���̊Ԋu[m]
#define cmt 0.01 //����
#define cmt1 1
#define cmt2 1


enum {
	x, y, r
};
///���[�^���[///
#define pulse  (500*4)		
#define R_per_p  ((2*pi)/pulse)			//���W�A���@/�@�p���X
#define S_wheel_distance 1				//���[�^���[�p�^�C���̊Ԋu(mm)
#define S_W_mileage_per_p (S_W_diameter*pi/ pulse)		//1pulse������̑��s����(mm)

///���o�͗p///

#endif 

