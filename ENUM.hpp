#ifndef _ENUM_HPP_
#define _ENUM_HPP_

	enum rotary { LEFT, RIGHT };
	enum Position { X, Y, radian };
	enum Position_Rotary {X_Rotary, Y_Rotary, radian_Rotary };
//	enum Position_RGB {X_RGB=0,Y_RGB=1,radian_RGB=2,High=0,Low=1 };
	enum Run_Phase	{WAIT, START, AT_RUNNING, THROUGHING, CHANGE, TEST};//�X�^�[�g�����܁��X���[�C���O�]�[���ց���~
	enum Root{TZ_1, TZ_2, TZ_3};	//�e�s�y�ɍs�����[�g���
	enum Bezier_Co{P0, P1, P2, P3};	//����_�i�x�W�F�j
	enum Bezier_Po{PX, PY};		//����t�ł̍��W�i�x�W�F�j
	enum dir {RO1, RO2, RO3, RO4};
	enum Direction{TO_TZ, TO_SZ};	//�����g��Ȃ��i�s�y�ɍs�����r�y�ɖ߂邩�j
	
#endif