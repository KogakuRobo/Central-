#ifndef _que_iterator_HPP_
#define _que_iterator_HPP_

template<typename data,unsigned int size>
list_iterator<data,size>::list_iterator(list<data,size>& _host,int num)
{
	host = &_host;this->num = num;
}

template<typename data,unsigned int size>
list_iterator<data,size>& list_iterator<data,size>::operator++(void)
{
	int next,prev;
	host->get_node(num,next,prev);
	num = next;
	return *this;
}

template<typename data,unsigned int size>
list_iterator<data,size>& list_iterator<data,size>::operator--(void)
{
	int next,prev;
	get_node(num,next,prev);
	num = prev;
	return *this;
}

template<typename data,unsigned int size>
data& list_iterator<data,size>::operator*(void)
{
	return host->get_data(num);
}
template<typename data,unsigned int size>
bool list_iterator<data,size>::operator==(list_iterator<data,size> ite)
{
	return this->num == ite.num;
}

#endif