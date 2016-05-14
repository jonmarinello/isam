/************************************************************************/
/*									*/
/*		- ISAM SUBROUTINE MACRO & DATA DESCRIPTIONS -		*/
/*									*/
/************************************************************************/

#ifndef ISAM_INCL
#define ISAM_INCL

#define CHARTYPE	0
#define CHARSIZE	sizeof(char)

#define INTTYPE		1
#define INTSIZE		sizeof(int)

#define LONGTYPE	2
#define LONGSIZE	sizeof(long)

#define DOUBLETYPE	3
#define DOUBLESIZE	sizeof(double)

#define FLOATTYPE	4
#define FLOATSIZE	sizeof(float)

#define SHORTTYPE	5
#define SHORTSIZE	sizeof(short)

#define MAXTYPE		6
#define ISDESC		0200
#define TYPEMASK	017

#define ISFIRST		0		/* position to first record	*/
#define ISLAST		1		/* position to last record	*/
#define ISNEXT		2		/* position to next record	*/
#define ISPREV		3		/* position to previous record	*/
#define ISCURR		4		/* position to current record	*/
#define ISEQUAL		5		/* position to equal record	*/
#define ISGREAT		6		/* position to greater record	*/
#define ISGTEQ		7		/* position to >= record	*/

/* isread lock modes */
#define ISLOCK		(1<<8)		/* lock record before reading	*/

/* isopen, isbuild lock modes */
#define ISAUTOLOCK	(2<<8)		/* automatic record lock	*/
#define ISMANULOCK	(4<<8)		/* manual record lock		*/
#define ISEXCLLOCK	(8<<8)		/* exclusive isam file lock	*/

#define ISINPUT		0		/* open for input only		*/
#define ISOUTPUT	1		/* open for output only		*/
#define ISINOUT		2		/* open for input and output	*/

/* audit trail mode parameters (Not implemented in L-ISAM */
#define AUDSETNAME	0
#define AUDGETNAME	1
#define AUDSTART	2
#define AUDSTOP		3
#define AUDINFO		4

#define NPARTS		8		/* maximum number of key parts	*/

struct keypart {
	short	kp_start;		/* starting byte of key part	*/
	short	kp_leng;		/* length in bytes		*/
	short	kp_type;		/* type of key part		*/
};

struct	keydesc {
	short	k_flags;	/* compression flags			*/
	short	k_nparts;	/* number of fields in key		*/
	struct	keypart		/* field descriptors (8)		*/
		k_part[NPARTS];
			/* the following is for internal use only	*/
	short	k_len;		/* overall actual length of key		*/
	long	k_rootnode;	/* offset of index tree root node	*/
};

#define k_start k_part[0].kp_start
#define k_leng	k_part[0].kp_leng
#define k_type	k_part[0].kp_type

#define ISNODUPS	000		/* duplicate records not allowed*/
#define ISDUPS		001		/* duplicate records allowed	*/
#define DCOMPRESS	002		/* duplicate key compression	*/
#define LCOMPRESS	004		/* leading compression		*/
#define TCOMPRESS	010		/* trailing compression		*/
#define COMPRESS	016		/* all compression		*/

struct	dictinfo {
	int	di_nkeys;		/* number of keys defined	*/
	int	di_recsize;		/* data record size		*/
	int	di_idxsize;		/* index record size		*/
	long	di_nrecords;		/* number of records in file	*/
};

/* iserrno posible values */
#define EDUPL		100		/* duplicate record		*/
#define ENOTOPEN	101		/* file not open		*/
#define EBADARG 	102		/* illegal argument		*/
#define EBADKEY 	103		/* illegal key description	*/
#define ETOOMANY	104		/* too many files open		*/
#define EBADFILE	105		/* bad isam file format		*/
#define ENOTEXCL	106		/* non-exclusive access		*/
#define ELOCKED 	107		/* record locked		*/
#define EKEXISTS	108		/* key already exists		*/
#define EPRIMKEY	109		/* primary key index		*/
#define EENDFILE	110		/* end/beginning of file	*/
#define ENOREC		111		/* record not found		*/
#define ENOCURR 	112		/* no current record		*/
#define EFLOCKED	113		/* file is currently locked	*/
#define EFNAME		114		/* file name too long		*/
#define ENOLOK		115		/* can't create lock file	*/	
#define EINPUTONLY	116		/* file opened only for INPUT	*/
#define EOUTPUTONLY	117		/* file opened only for OUTPUT	*/
#define ENOTMANULOCK	118		/* can't manual lock file or rec*/
#define EBADLOCK	119		/* bad lock definition		*/
#define EBADVERSION	120		/* bad file version		*/

extern	int	iserrno;		/* isam error return code	*/
extern	int	iserrio;
extern	char	isstat1;
extern	char	isstat2;
extern	char	*isversnumber;		/* L-ISAM version number	*/
extern	char	*iscopyright;		/* LOGORDI SA. copyright	*/
extern	long	isserial;		/* L-ISAM serial number */
extern	int	is_nerr;		/* Highest L-ISAM error code	*/
extern	char	*is_errlist[];		/* L-ISAM error messages	*/

/*	Error message usage:
 *	if (iserrno >= 100 && iserrno < is_nerr)
 *		printf("ISAM error %d: %s\n", iserrno, is_errlist[iserrno-100]);
 */

struct	audithead {
	char	au_type;
	char	au_time[4];
	char	au_procid[2];
	char	au_userid[2];
};

#define AUDHEADSIZE	9;

/* Prototypes */

int	isaddindex(int isfd, struct keydesc *keydesc);
int	isaudit(int isfd, char *filename, int mode);
int	isbuild(char *filename, int recordlength, struct keydesc *keydesc,
		int mode);
int	isclose(int isfd);
int	isdelcurr(int isfd);
int	isdelete(int isfd, char *record);
int	isdelindex(int isfd, struct keydesc *keydesc);
int	iserase(char *filename);
int	isindexinfo(int isfd, struct keydesc *buffer, int number);
int	islock(int isfd);
int	isopen(char *filename, int mode);
int	isread(int isfd, char *record, int mode);
int	isrelease(int isfd);
int	isrename(char *oldname, char *newname);
int	isrewcurr(int isfd, char *record);
int	isrewrite(int isfd, char *record);
int	isstart(int isfd, struct keydesc *keydesc, int length, char *record,
		int mode);
int	isuniqueid(int isfd, long *uniqueid);
int	isunlock(int isfd);
int	iswrite(int isfd, char *record);

#endif
