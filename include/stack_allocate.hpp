#ifndef _stack_allocate_HPP_
#define _stack_allocate_HPP_

#include "task_control_block.h"

/*
スタック領域管理プログラム

スタック領域の動的割当て、並びに解放制御を行うプログラム。
確保されたスタック領域を一定のセクタで区切り
使用されているセクタを管理します。

セクタサイズは

*/


void stack_manager_init(void);
//スタック領域を確保し、そのアドレスを返します。
//エラーの場合はNULLを返します。
void* _sualloc(int size);

#endif