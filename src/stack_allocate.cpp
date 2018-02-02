#include "stack_allocate.hpp"

#define NUMBER_SECT_SU STACK_USER_SIZE/SIZE_OF_SECT_SU
#define NUMBER_SECT_SI STACK_KERNEL_SIZE/SIZE_OF_SECT_SI

//�X�^�b�N�}�l�W�����g�\����
//flag��0�ł���΋󂫗̈�
//�g�p����Ă���ꍇ�A���̐擪�Z�N�^�ɃT�C�Y���L�ڂ���Ă��܂��B
//�擪����T�����A�󂫗̈悪�K�v���A�����ĊJ���Ă���Ƃ���
//���̐擪flag�ɃT�C�Y���L�ڂ��܂��B�ڂ�����FAT�t�@�C���V�X�e���Q��
typedef struct{
	unsigned char flag;
}_stack_managment_struct;

_stack_managment_struct su_m[NUMBER_SECT_SU];
_stack_managment_struct si_m[NUMBER_SECT_SI];

void stack_manager_init(void)
{
	for(int i = 0;i < NUMBER_SECT_SU;i++)
		su_m[i].flag = 0;
	for(int i = 0;i < NUMBER_SECT_SI;i++)
		si_m[i].flag = 0;
}

void* _sualloc(int size)
{
}

