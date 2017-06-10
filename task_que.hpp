#ifndef _task_que_HPP_
#define _task_que_HPP_

#include "que.hpp"
#include "task_control_block.h"

class task_list:public list<_task_control_block*,16>{
public:
	void priority_push(_task_control_block* tcb){
		iterator ite = this->begin();
		
		//���X�g����ł���΂��̂܂܃t�����g�v�b�V��
		if(this->empty()){
			this->push_front(tcb);
			return;
		}
		//���X�g����łȂ���΁A�D��x���t�]����܂ŒT���B
		while((*ite)->priority < tcb->priority){
			++ite;
			//�����}���\��̗D��x���ł��Ⴏ��΁A�G���h�Ƀv�b�V��
			if(ite == (this->begin())){
				this->push_back(tcb);
				return;
			}
		}
		
		this->insert(ite,tcb);
	}
};

#endif