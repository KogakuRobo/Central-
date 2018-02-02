#ifndef _task_que_HPP_
#define _task_que_HPP_

#include "que.hpp"
#include "task_control_block.h"

class task_list:public list<_task_control_block*,16>{
public:
	void priority_push(_task_control_block* tcb){
		iterator ite = this->begin();
		
		//リストが空であればそのままフロントプッシュ
		if(this->empty()){
			this->push_front(tcb);
			return;
		}
		//リストが空でなければ、優先度が逆転するまで探す。
		while((*ite)->priority < tcb->priority){
			++ite;
			//もし挿入予定の優先度が最も低ければ、エンドにプッシュ
			if(ite == (this->begin())){
				this->push_back(tcb);
				return;
			}
		}
		
		this->insert(ite,tcb);
	}
};

#endif