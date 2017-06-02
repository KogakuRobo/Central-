#ifndef _que_host_HPP_
#define _que_host_HPP_


template<typename data,unsigned int size>
list<data,size>::list(void)
{
	length = 0;
	head_num = end_num = -1;
}

template<typename data,unsigned size>
bool list<data,size>::empty(void)
{
	return length == 0;
}

template<typename data,unsigned size>
int list<data,size>::get_node(int num,int &next,int &prev)
{
	next = nodes[num].next;prev = nodes[num].prev;
	return 0;
}

template<typename data,unsigned int size>
int list<data,size>::node_allocate(void)
{
	for(int i=0;i < size;i++){
		if(nodes[i].flags.use == FLAG_UNUSE){
			nodes[i].flags.use = FLAG_USE;
			return i;
		}
	}
	return -1;
}

template<typename data,unsigned int size>
int list<data,size>::push_back(data d)
{
	int new_num = this->node_allocate();
	if(end_num == -1){
		end_num = head_num = new_num;
		nodes[new_num].prev = nodes[new_num].next = new_num;
	}
	else{
		nodes[new_num].prev = end_num;
		nodes[new_num].next = head_num;
		nodes[end_num].next = new_num;
		nodes[head_num].prev = new_num;
		end_num = new_num;
	}
	nodes[new_num].node = d;
	length++;
	return 0;
}

template<typename data,unsigned int size>
int list<data,size>::push_front(data d)
{
	int new_num = this->node_allocate();
	if(head_num == -1){
		end_num = head_num = new_num;
		nodes[new_num].prev = nodes[new_num].next = new_num;
	}
	else{
		nodes[new_num].prev = end_num;
		nodes[new_num].next = head_num;
		nodes[end_num].next = new_num;
		nodes[head_num].prev = new_num;
		head_num = new_num;
	}
	nodes[new_num].node = d;
	length++;
	return 0;
}

template<typename data,unsigned int size>
data list<data,size>::pop_back(void){
	data ret = nodes[end_num].node;
	nodes[end_num].flags.use = FLAG_UNUSE;
	if(nodes[end_num].prev == end_num)
		end_num = head_num = -1;
	else{
		nodes[nodes[end_num].prev].next =head_num;
		nodes[head_num].prev = nodes[end_num].prev;
		end_num = nodes[end_num].prev;
	}
	length--;
	return ret;
	
}

template<typename data,unsigned int size>
data list<data,size>::pop_front(void){
	data ret = nodes[head_num].node;
	nodes[head_num].flags.use = FLAG_UNUSE;
	if(nodes[head_num].prev == head_num)
		end_num = head_num = -1;
	else{
		nodes[nodes[head_num].next].prev =end_num;
		nodes[end_num].next = nodes[head_num].next;
		head_num = nodes[head_num].next;
	}
	length--;
	return ret;
	
}

template<typename data,unsigned int size>
list<data,size>::iterator list<data,size>::begin(void)
{
	iterator ite(*this,head_num);
	return ite;
}

template<typename data,unsigned int size>
list<data,size>::iterator list<data,size>::end(void)
{
	iterator ite(*this,end_num);
	return ite;
}

template<typename data,unsigned int size>
list<data,size>::iterator list<data,size>::insert(iterator ite_p,data d)
{
	int new_num = this->node_allocate();
	iterator ret(*this,new_num);
	
	nodes[new_num].next = ite_p.num;
	nodes[new_num].prev = nodes[ite_p.num].prev;
	nodes[nodes[ite_p.num].prev].next = new_num;
	nodes[ite_p.num].prev = new_num;
	
	if(ite_p.num == head_num)head_num = new_num;
	nodes[new_num].node = d;
	length++;
	return ret;
}

template<typename data,unsigned int size>
list<data,size>::iterator list<data,size>::erase(iterator ite_p)
{
	nodes[ite_p.num].flags.use = FLAG_UNUSE;
	if(nodes[ite_p.num].prev == ite_p.num)
		end_num = head_num = -1;
	else if(ite_p.num == head_num)head_num = nodes[ite_p.num].next;
	else if(ite_p.num == end_num)end_num = nodes[ite_p.num].prev;
	
	nodes[nodes[ite_p.num].next].prev = nodes[ite_p.num].prev;
	nodes[nodes[ite_p.num].prev].next = nodes[ite_p.num].next;
	
	iterator ret(*this,nodes[ite_p.num].next);
	
	length--;
	
	return ret;
}

#endif