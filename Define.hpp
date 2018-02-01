#ifndef _DEFINE_HPP_
#define _DEFINE_HPP_

//#include <iodefine.h>


//���荞�݂Ƃ�
#define cmt0 /*((48000000 / (8 * pow(4, CMT0.CMCR.BIT.CKS))) / CMT0.CMCOR)*/100
#define pulse  (500*4)		
#define R_per_p  ((2*pi)/pulse)					//���W�A���@/�@�p���X
#define pi 3.14159265359
#define S_wheel_radius	0.01//���[�^���[�p�^�C�����a
#define S_wheel_distance 0.07	//�^�C���ԋ���
#define arrive_time	0//�ڕW���W�ւ̓��B����

//���x���~�b�^�[//
#define RIMIT 12

//�f���[�e�B//
#define DUTY_1		MTU6.TGRB
#define DUTY_2		MTU4.TGRB
#define DUTY_3		MTU9.TGRB
#define DUTY_4		MTU10.TGRB

//���[�^���[�G���R�[�_//
#define RO_1 		MTU8.TCNT//���[�^���[�J�E���g�Ɏg�p
#define RO_2 		MTU7.TCNT
#define RO_3 		MTU2.TCNT
#define RO_4 		MTU1.TCNT

//�o�h�c����//
#define KP	0.1
#define KI	0
#define KD	0
#define DELTA_T	0.01

//���荞��//
#define cmt1	100

enum root{str, right, left, back, s_right, s_left, b_right, b_left, stop};
enum {ro_1, ro_2, ro_3, ro_4};

//�O���[�o���֐�//
extern int g_rotary_count[4];//���[�^���[�J�E���g
extern int  g_check[4];//
extern double V_now;


#endif