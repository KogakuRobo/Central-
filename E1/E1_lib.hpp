#ifndef _E1_lib_H_
#define _E1_lib_H_

#define E1_PATH "E1"

/* Special character code */
#define CR 0x0d                     /* Carriage return */
#define LF 0x0a                     /* Line feed       */

class E1_file_desc_factor:public _low_file_desc_factor{
	static const unsigned char g_e1_path[];
public:
	E1_file_desc_factor(void);
	virtual const unsigned char* 	get_name(void){return g_e1_path;}
	virtual _low_file_desc_class*	open(const char*,long);
	virtual long 			close(_low_file_desc_class*);
}E1_factor;

class E1_file_desc:public _low_file_desc_class{
	//プライベートデータなし
public:
	virtual long read(unsigned char*,long);
	virtual long write(const unsigned char*,long);
};

#endif