#ifndef _SETTING_H_		//define�ɂ���`
#define _SETIING_H_

#define pi 3.14159265359 
#define squa_two 1.41421356237
#define M_W_diameter 0.12 //���C���^�C���̒��a
#define S_W_diameter 0.05 //���[�^���[�p�^�C���̒��a[m]
#define L_wheel_distance 0.4				//x���[�^�����獶�E�̎�^�C���̊Ԋu[m]
#define L_wheel_distance_front 0.83058			//x���[�^������O���E�^�C���̒��S�܂�0.76[m] 
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

