#include "stack_allocate.hpp"

#define NUMBER_SECT_SU STACK_USER_SIZE/SIZE_OF_SECT_SU
#define NUMBER_SECT_SI STACK_KERNEL_SIZE/SIZE_OF_SECT_SI

//スタックマネジメント構造体
//flagが0であれば空き領域
//使用されている場合、その先頭セクタにサイズが記載されています。
//先頭から探査し、空き領域が必要分連続して開いているときに
//その先頭flagにサイズを記載します。詳しくはFATファイルシステム参照
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

