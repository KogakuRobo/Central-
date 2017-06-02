/*********************************************************************
*
* Device     : RX
*
* File Name  : lowsrc.c
*
* Abstract   : Program of I/O Stream.
*
* History    : 1.00  (2009-08-07)
*            : 1.01  (2011-02-21)
*            : 1.10  (2014-10-22)
*
* NOTE       : THIS IS A TYPICAL EXAMPLE.
*
* Copyright (C) 2009(2011-2014) Renesas Electronics Corporation.
* and Renesas Solutions Corp.
*
*********************************************************************/

#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <ctype.h>
#include "lowsrc.hpp"

extern const long _nfiles;     /* The number of files for input/output files */

#include <stdio.h>

_low_file_desc_factor* 	factor[IODRIVER];
_low_file_desc_class*	file_desc[IOSTREAM];

//ドライバ登録関数
int set_io_driver(_low_file_desc_factor* _factor){
	for(int i = 0;i < IODRIVER;i++){
		if(factor[i] == NULL){
			factor[i] = _factor;
			return i;
		}
	}
	return -1;
}

//ドライバ検索関数
int search_io_driver(const char *name)
{
	int i = 0;
	const char* n;
	//識別ネームが同一であることを確認
	for(;i < IODRIVER;i++){
		if(factor[i] == NULL)break;
		n = (const char*)factor[i]->get_name();
		if(strcmp( name, n ) == 0) {
			return i;
		}
	}
	return -1;
}

int extraction_driver_name(const char *path,char *dest)
{
	int i;
	//空白を除く表示文字
	for (i = 0; isgraph(path[i]); i++){
		dest[i] = path[i];
	}
	dest[i] = '\0';
	return 0;
}
/****************************************************************************/
/* _INIT_IOLIB                                                              */
/*  Initialize C library Functions, if necessary.                           */
/*  Define USES_SIMIO on Assembler Option.                                  */
/****************************************************************************/
void _INIT_IOLIB( void )
{
	int i;
    /* A file for standard input/output is opened or created. Each FILE     */
    /* structure members are initialized by the library. Each _Buf member   */
    /* in it is re-set the end of buffer pointer.                           */

    /* Initializations of File Stream Table                                 */
    _Files[0] = stdin;
    _Files[1] = stdout;
    _Files[2] = stderr;

    for(i = 0;i < IOSTREAM;i++)file_desc[i] = NULL;
    for(i = 0;i < IODRIVER;i++)factor[i] = NULL;
}

/****************************************************************************/
/* _CLOSEALL                                                                */
/****************************************************************************/
void _CLOSEALL( void )
{
    long i;

    for( i=0; i < _nfiles; i++ )
    {
        /* Checks if the file is opened or not                               */
        if( _Files[i]->_Mode & (_MOPENR | _MOPENW | _MOPENA ) )
        fclose( _Files[i] );    /* Closes the file                           */
    }
}

long kernel_open(const char *name,                  /* File name                 */
     long  mode,                             /* Open mode                 */
     long  flg)                              /* Open flag                 */
{
	int fdno = 0;
	int driver_no;
	char path[6];
	//空きディスクリプタナンバー調べ
	for(;fdno < IOSTREAM;fdno++){
		if(file_desc[fdno] == NULL)break;
	}
	if(fdno == IOSTREAM)return -1;		//空き無し
	
	//該当デバイスドライバ検索
	extraction_driver_name(name,path);
	driver_no = search_io_driver(path);
	if(driver_no == -1)return -1;		//当該ドライバなし
	
	file_desc[fdno] = factor[driver_no]->open(name,mode);
	if(file_desc[fdno] == NULL){
		return -1;
	}
	file_desc[fdno]->set_factor(factor[driver_no]);
	return fdno;
}

/**************************************************************************/
/* write:Data write                                                       */
/*  Return value:Number of write characters (Pass)                        */
/*               -1                         (Failure)                     */
/**************************************************************************/
long kernel_write(long  fileno,             /* File number                       */
      const unsigned char *buf,       /* The address of destination buffer */
      long  count)                   /* The number of chacter to write    */
{
    if(fileno < IOSTREAM){
	    if(file_desc[fileno] != NULL){
		    return file_desc[fileno]->write(buf,count);
	    }
    }
    return -1;
}

long kernel_read( long fileno, unsigned char *buf, long count )
{
    if(fileno < IOSTREAM){
	    if(file_desc[fileno] != NULL){
		    return file_desc[fileno]->read(buf,count);
	    }
    }
    return -1;
}

long kernel_close( long fileno )
{
    if(fileno < IOSTREAM){
	    if(file_desc[fileno] != NULL){
		    int ret =file_desc[fileno]->get_factor()->close(file_desc[fileno]);
		    if(ret == 0)
		    {
			return 0;
		    }
		    else return -1;
	    }
	    return 0;
    }
    return -1;
}

long kernel_ioctl(long fileno,unsigned long request,void *argp)
{
	if(fileno < IOSTREAM){
	    if(file_desc[fileno] != NULL){
		    return file_desc[fileno]->ioctl(request,argp);
	    }
    }
    return -1;
}

long lseek( long fileno, long offset, long base )
{
    return -1L;
}

#ifdef _REENTRANT
// For Reentrant Library (generated lbgrx with -reent option)
#define MALLOC_SEM   1  /* Semaphore No. for malloc */
#define STRTOK_SEM   2  /* Semaphore No. for strtok */
#define FILE_TBL_SEM 3  /* Semaphore No. for fopen  */
#define MBRLEN_SEM   4  /* Semaphore No. for mbrlen */
#define FPSWREG_SEM  5  /* Semaphore No. for FPSW register */
#define FILES_SEM    6  /* Semaphore No. for _Files */
#define SEMSIZE     26  /* FILES_SEM + _nfiles (assumed _nfiles=20) */

#define TRUE 1
#define FALSE 0
#define OK 1
#define NG 0
extern long *errno_addr(void);
extern long wait_sem(long);
extern long signal_sem(long);
static long sem_errno;
static int force_fail_signal_sem = FALSE;
static int semaphore[SEMSIZE];

/******************************************************************************/
/*                  errno_addr: Acquisition of errno address                  */
/*                        Return value: errno address                         */
/******************************************************************************/
long *errno_addr(void)
{
    /* Return the errno address of the current task */
    return (&sem_errno);
}

/******************************************************************************/
/*           wait_sem: Defines the specified numbers of semaphores            */
/*                       Return value: OK(=1) (Normal)                        */
/*                                     NG(=0) (Error)                         */
/******************************************************************************/
long wait_sem(long semnum) /* Semaphore ID */
{
    if((0 < semnum) && (semnum < SEMSIZE)) {
        if(semaphore[semnum] == FALSE) {
            semaphore[semnum] = TRUE;
            return(OK);
        }
    }
    return(NG);
}

/******************************************************************************/
/*          signal_sem: Releases the specified numbers of semaphores          */
/*                       Return value: OK(=1) (Normal)                        */
/*                                     NG(=0) (Error)                         */
/******************************************************************************/
long signal_sem(long semnum) /* Semaphore ID */
{
    if(!force_fail_signal_sem) {
        if((0 <= semnum) && (semnum < SEMSIZE)) {
            if( semaphore[semnum] == TRUE ) {
                semaphore[semnum] = FALSE;
                return(OK);
            }
        }
    }
    return(NG);
}
#endif
