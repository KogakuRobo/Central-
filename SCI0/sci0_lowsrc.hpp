#ifndef _rx621_sci0_HPP_
#define _rx621_sci0_HPP_

#define SCI0_PATH "SCI0"
#include "que.hpp"

class sci0_file_desc_factor:public _low_file_desc_factor{
//	SCI0のデバイス名とオープンしたファイル数
//	デバイスドライバ登録・呼び出し時に使用されます。
	static const unsigned char g_sci0_path[];
	static int n_openfile;				//sci0の開いたファイル数
	
public:
	sci0_file_desc_factor(void);
	virtual const unsigned char*	get_name(void){return g_sci0_path;}
	virtual _low_file_desc_class*	open(const char*,long);
	virtual long close(_low_file_desc_class*);

}sci0_factor;

class sci0_file_desc:public _low_file_desc_class{
	static thread_list t_list;
	static DTC_TABLE *tx_table;
	
	unsigned char *tx_buf;
	
private:
	int buf_allocate(int);
	static int suspend(void);
public:
	int buf_clear(void);
	static int resume(void);
public:
	sci0_file_desc(void);
	virtual long read(unsigned char*,long);
	virtual long write(const unsigned char*,long);
};

#endif