#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include "lowsrc.h"
#include "E1_lib.h"

long E1_open(_FD*,long);
long E1_write(_FD*,const unsigned char *,long);
long E1_read(_FD*, unsigned char *,long);
long E1_close(_FD*);

_FD _e1_driver = {
	._open = E1_open,
	._read = E1_read,
	._write = E1_write,
	._close = E1_close,
};
	

/* Output one character to standard output */
extern void charput(unsigned char);
/* Input one character from standard input */
extern unsigned char charget(void);

long E1_open(_FD *fd,                  /* File name                 */
     long  mode)                             /* Open mode                 */
{
    return 0;                             /*Others                  */
}

long E1_close( _FD *fd )
{
    return 1;
}

long E1_write(_FD *fd,             /* File number                       */
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

long E1_read(_FD *fd, unsigned char *buf, long count )
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


void E1_init(void)
{	
	set_io_driver(E1_PATH,&_e1_driver);
	
	if( freopen( E1_PATH, "r", stdin ) == NULL )stdin->_Mode = 0xffff;  /* Not allow the access if it fails to open */
	    stdin->_Mode  = _MOPENR;            /* Read only attribute              */
	    stdin->_Mode |= _MNBF;              /* Non-buffering for data           */
	    stdin->_Bend = stdin->_Buf + 1;  /* Re-set pointer to the end of buffer */
	if( freopen( FPATH_STDOUT, "w", stdout ) == NULL )stdout->_Mode = 0xffff; /* Not allow the access if it fails to open */
	    stdout->_Mode |= _MNBF;             /* Non-buffering for data           */
	    stdout->_Bend = stdout->_Buf + 1;/* Re-set pointer to the end of buffer */
	if( freopen( FPATH_STDERR, "w", stderr ) == NULL )stderr->_Mode = 0xffff; /* Not allow the access if it fails to open */
	    stderr->_Mode |= _MNBF;             /* Non-buffering for data           */
	    stderr->_Bend = stderr->_Buf + 1;/* Re-set pointer to the end of buffer */
}