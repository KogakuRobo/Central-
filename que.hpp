#ifndef _que_HPP_
#define _que_HPP_

#include <stddef.h>

template<typename data,unsigned int size>class list_iterator;

template<typename data,unsigned int size = 16>
class list{
	friend list_iterator<data,size>;
public:
	typedef list_iterator<data,size> iterator;
	
private:
	typedef enum{
		FLAG_UNUSE = 0,
		FLAG_USE = 1,
	}USE_FLAGS;
	struct{
		data node;
		int next,prev;
		struct{
			USE_FLAGS use:1;
		}flags;
	}nodes[size];
	int length;
	
	int head_num,end_num;
private:
//num�Ԃ̃m�[�h�̏����擾���܂��B
	int get_node(int num,int &next,int &prev);
	
//num�Ԃ�data���擾���܂�	
	data& get_data(int num){return nodes[num].node;}
	
//�󂫃m�[�h��������USE�t���O�𗧂ĂāA���̔ԍ���ԋp���܂��B�G���[��(-1)�B
	int node_allocate(void);
public:
	list(void);
	
	bool empty(void);
	
//����n�@�֐�
	virtual int push_back(data d);
	virtual int push_front(data d);

	virtual data pop_back(void);
	virtual data pop_front(void);
	
	virtual iterator begin(void);
	virtual iterator end(void);
	
	virtual iterator insert(iterator,data);
	virtual iterator erase(iterator);
};

template<typename data,unsigned int size>
class list_iterator{
	friend list<data,size>;
	
	unsigned int num;

	list<data,size> *host;
private:
	list_iterator(list<data,size> &_host,int num);

public:
//�Q�ƌn�@�֐�
	list_iterator<data,size>& operator++(void);
	list_iterator<data,size>& operator--(void);
	data& operator*(void);
	
	bool operator==(list_iterator<data,size>);
};

#include "que_host.hpp"
#include "que_iterator.hpp"

#endif