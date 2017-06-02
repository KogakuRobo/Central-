#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include "../lowsrc.hpp"
#include "E1_lib.hpp"

const unsigned char E1_file_desc_factor::g_e1_path[] = E1_PATH;

extern "C"{
/* Output one character to standard output */
extern void charput(unsigned char);
/* Input one character from standard input */
extern unsigned char charget(void);
}

_low_file_desc_class* E1_file_desc_factor::open(const char* path,long mode)
{
	return new E1_file_desc;
}

long E1_file_desc_factor::close( _low_file_desc_class* desc )
{
	delete desc;
	return 1;
}

long E1_file_desc::write(
      const unsigned char *buf,       /* The address of destination buffer */
      long  count)                   /* The number of chacter to write    */
{
	long    i;                          /* A variable for counter         */
	unsigned char    c;                 /* An output character            */
	for( i = count; i > 0; --i )
	{
		c = *buf++;
		charput(c);
	}
	return count;        /*Return the number of written characters */
}

long E1_file_desc::read(unsigned char *buf, long count )
{
	long i;

	for(i = count; i > 0; i--){
		*buf = charget();
		if(*buf==CR){              /* Replace the new line character */
			*buf = LF;
		}
		buf++;
	}
	return count;

}


E1_file_desc_factor::E1_file_desc_factor(void)
{	
	set_io_driver(&E1_factor);
	
	if( freopen( "E1", "r", stdin ) == NULL )stdin->_Mode = 0xffff;  /* Not allow the access if it fails to open */
	    stdin->_Mode  = _MOPENR;            /* Read only attribute              */
	    stdin->_Mode |= _MNBF;              /* Non-buffering for data           */
	    stdin->_Bend = stdin->_Buf + 1;  /* Re-set pointer to the end of buffer */
	if( freopen( "E1", "w", stdout ) == NULL )stdout->_Mode = 0xffff; /* Not allow the access if it fails to open */
	    stdout->_Mode |= _MNBF;             /* Non-buffering for data           */
	    stdout->_Bend = stdout->_Buf + 1;/* Re-set pointer to the end of buffer */
	if( freopen( "E1", "w", stderr ) == NULL )stderr->_Mode = 0xffff; /* Not allow the access if it fails to open */
	    stderr->_Mode |= _MNBF;             /* Non-buffering for data           */
	    stderr->_Bend = stderr->_Buf + 1;/* Re-set pointer to the end of buffer */
}