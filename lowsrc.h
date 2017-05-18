/***********************************************************************/
/*                                                                     */
/*  FILE        :lowsrc.h                                              */
/*  DATE        :Tue, Oct 31, 2006                                     */
/*  DESCRIPTION :Header file of I/O Stream file                        */
/*  CPU TYPE    :                                                      */
/*                                                                     */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/*                                                                     */
/***********************************************************************/
/*Number of I/O Stream*/
#define IOSTREAM 6
#define IODRIVER 6
#include<stdio.h>

/* file number */
#define STDIN  0                    /* Standard input (console)        */
#define STDOUT 1                    /* Standard output (console)       */
#define STDERR 2                    /* Standard error output (console) */

#define FLMIN  0                    /* Minimum file number     */
#define _MOPENR	0x1
#define _MOPENW	0x2
#define _MOPENA	0x4
#define _MTRUNC	0x8
#define _MCREAT	0x10
#define _MBIN	0x20
#define _MEXCL	0x40
#define _MALBUF	0x40
#define _MALFIL	0x80
#define _MEOF	0x100
#define _MERR	0x200
#define _MLBF	0x400
#define _MNBF	0x800
#define _MREAD	0x1000
#define _MWRITE	0x2000
#define _MBYTE	0x4000
#define _MWIDE	0x8000
/* File Flags */
#define O_RDONLY 0x0001 /* Read only                                       */
#define O_WRONLY 0x0002 /* Write only                                      */
#define O_RDWR   0x0004 /* Both read and Write                             */
#define O_CREAT  0x0008 /* A file is created if it is not existed          */
#define O_TRUNC  0x0010 /* The file size is changed to 0 if it is existed. */
#define O_APPEND 0x0020 /* The position is set for next reading/writing    */
                        /* 0: Top of the file 1: End of file               */
			
#define FPATH_STDIN     "C:\\stdin"
#define FPATH_STDOUT    "C:\\stdout"
#define FPATH_STDERR    "C:\\stderr"


struct _low_file_desc{
	long (*_open)	(struct _low_file_desc *,	long);
	long (*_read)	(struct _low_file_desc *,	unsigned char*,		long);
	long (*_write)	(struct _low_file_desc *,	const unsigned char*,	long);
	long (*_close)	(struct _low_file_desc *);
	
	void *private_data;
	
	struct{
		unsigned char use:1;		//use = 1;unuse = 0;
	}control_flags;
};
typedef struct _low_file_desc _FD;


/*	ドライバ登録関数 	*/
//ドライバを登録する場合には名前と、_FD構造体の静的宣言が必要です。(モジュール内保持）
//
int set_io_driver(const char*,_FD*);
