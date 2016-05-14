/************************************************************************/
/*									*/
/*				- INTERNAL -				*/
/*									*/
/*		- ISAM SUBROUTINE MACRO & DATA DESCRIPTIONS -		*/
/*									*/
/************************************************************************/

#ifndef IISAM_INCL
#define IISAM_INCL

#include "isam.h"
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <io.h>
#include <sys\locking.h>
#include <stdlib.h>
#include <string.h>

#define E_ER		1
#define E_OK		0

#define LOCKING		1
#define F_LOCK		LK_LOCK
#define F_TEST		LK_NBLCK
#define F_ULOCK		LK_UNLCK

#define US_LONG long

/* duplicate record macros */
#define ZAP		1		/* Zap keyval with dups unum	*/
#define GET		0		/* Get dups unum from keyval	*/
#define UNIQUESIZE	sizeof(US_LONG)/* Dups uniqueness length	*/
#define UNIQUESTART	0L		/* Forces correct kindex on	*/
					/* Deleteion/random lookups	*/
#define HIGHUSTART	2147483647L
					/* Forces correct kindex for	*/
					/* INSERTION only		*/
#define DUMMYGET	0L		/* Dummy arg to getzapunum()	*/
					/* used with GET macro		*/

#define ISCERW		(O_CREAT|O_EXCL|O_RDWR|O_BINARY)
#define WHOLEKEY	0		/* Entire key significant	*/
#define PAGESIZE	512L		/* B-TREE node size (stdio.h)	*/
#define COMPMASK	0xfffe		/* Compression flags mask	*/
#define DUPSMASK	0x0001		/* Duplicate keys mask		*/
#define PRIMARYKEY	0		/* Primary B-TREE number	*/
#define MAXSYSNAME	14		/* Max length of file names	*/
#define INSERT		1		/* Insertion mode flag		*/
#define DELETE		2		/* Deletion mode flag		*/
#define FOUND		1
#define NOTFOUND	0
#define GREATER		1		/* Now look right (offset)	*/	
#define EQUAL		0		/* Keys are equal (found)	*/
#define LESS		(-1)		/* Now look left  (offset)	*/
#define NILL		(-999999999L)	/* Internal NILL pointer for	*/
					/* "NULL" offsets		*/
#define INDEXFREELIST	0		/* Index freelist parameter	*/
#define DATAFREELIST	1		/* Data freelist parameter	*/
#define HIGHVALUES	'\377'		/* Character high values	*/
#define LOWVALUES	'\0'		/* Character low values		*/
#define ALIVE		'\377'		/* LIVE data record		*/
#define DEAD		'\0'		/* DEAD data record		*/
#define MOVERIGHT	1		/* Move one key to the right	*/
#define MOVELEFT	(-1)		/* Move one key to the left	*/
#define ISCREATE	8		/* Used by isbuild to avoid	*/
					/* destruction of an existing	*/
					/* isam file			*/

#define ISIFD		ftbl[isfd].idxfd
#define ISDFD		ftbl[isfd].datfd
#define UNIQUECOUNT	ftbl[isfd].dptr.unumber
#define DATACOUNT	ftbl[isfd].dptr.reccount
#define LOCKMASK	~(ISAUTOLOCK|ISMANULOCK|ISEXCLLOCK)

/*----------------------------------------------------------------------*/

			/* B-TREE NODE FORMAT */

			/* REPEATING KEYS DESCRIPTION */

/*	Please note: In the following structure nodekey, all longs	*/
/*	must be the first fields in the structure due to possible	*/
/*	allignment problems. Also note that the character pointer	*/
/*	declaration "char *keyval;" must and I mean MUST be defined	*/
/*	at the end of the "struct nodekey" definition as shown. This	*/
/*	is due to the fact that the keyvalue pointers are not		*/
/*	actually stored on disk. (See the functions loadnode() and	*/
/*	dumpnode() for examples of this nessecity). The write() and	*/
/*	read() of nodekey is done like this:				*/
/*									*/
/*		write(ISIFD, (char *)worknode->kptr[i],			*/
/*			(unsigned)(NDKSIZE - sizeof(char *)));		*/
/*									*/
/*	If you change the placement of keyval in nodekey, you are	*/
/*	guarenteed to get some FUN results.				*/

struct	nodekey {
	long	nodenext;		/* Offset to other sons		*/	
	long	dataptr;		/* Offset to data record	*/
	short	ldbytes;		/* Leading bytes		*/
	short	tlblanks;		/* Trailing bytes		*/
	char	*keyval;		/* Actual KEY value, this	*/
					/* includes uniqueness value	*/
};

struct nodedesc {			/* Node description struct	*/
	long	leftptr;		/* Offset to first son		*/
	short	nodeln;			/* #of bytes in this NODE	*/
	short	keycnt;			/* #of keys in this NODE	*/	
};

struct	NODE {
	struct	nodedesc	ndesc;	/* Node general description	*/
	struct	nodekey		**kptr; /* Pointer to array of keys for */
					/* a node varying "M" c/o Knuth */
};

/*----------------------------------------------------------------------*/

			/* DICTIONARY DESCRIPTION FORMAT */

struct	idict {
	long	dsccnt;		/* #of key description format records	*/
	long	dfree;		/* Offset of first free list data record*/
	long	kfree;		/* Offset of first free list key record */
	US_LONG unumber;/* Current uniqueness counter		*/
	long	unique;		/* File uniqueness number		*/
	long	datalength;	/* Total length of data record		*/
	long	reccount;	/* Number of records in file		*/
	long	sn;		/* Serial Number			*/
	char	vn[20];		/* Version Number			*/
	char	cr[50];		/* Copyright				*/
	char	fu[25];		/* Future use				*/
};

#define MAXKEYLENGTH	((PAGESIZE - (sizeof(struct nodedesc)+\
			(4*(sizeof(struct nodekey)-sizeof(char *)))))/4)

#define INTERNALKEYSZ	MAXKEYLENGTH+UNIQUESIZE
			
		/* ARRAY OF ISAM FILE & STRUCTURE POINTERS */

struct	filetable {
	int	idxfd;			/* File pointer to index file	*/
	int	datfd;			/* File pointer to data file	*/
	long	offset;			/* Offset of current node	*/
	int	omode;			/* Mode of file access		*/
	int	startsw;		/* Isstart called switch	*/
	int	lastsw;			/* Isstart called w/ISLAST	*/
	int	livekey;		/* "live" key description	*/
	char	curkey[INTERNALKEYSZ];	/* "current record pointer"	*/
	struct	keydesc *descptr;	/* Key description array	*/
	struct	keyblock *kbptr;	/* Key description block array	*/
	struct	idict	dptr;		/* Dictionary discription	*/
	long	olockoffset;		/* Lock on previous record	*/
};

		/* FREELIST STRUCTURE DEFINITION */

#define FREESIZE	((PAGESIZE - (2 * LONGSIZE)) / LONGSIZE)

struct	flist {
	long	llink;		/* Last free page in list (if any)	*/
	long	fidx;		/* Index into foffests table		*/
	long	foffsets[FREESIZE];	
};

		/* KEY DESCRIPTION BLOCK HEADER */

struct	keyblock {
	long	kchain;		/* Chain to next key description block	*/
	short	bcount;		/* Number of bytes in this block	*/
};

#define KPSIZE	sizeof(struct keypart)
#define KDSIZE	sizeof(struct keydesc)
#define NDKSIZE sizeof(struct nodekey)
#define NDDSIZE sizeof(struct nodedesc)
#define NDSIZE	sizeof(strict NODE)
#define IDSIZE	sizeof(struct idict)
#define FTSIZE	sizeof(struct filetable)
#define DISIZE	sizeof(struct dictinfo)
#define FLSIZE	sizeof(struct flist)
#define KBSIZE	sizeof(struct keyblock)

extern	int	errno;
extern	int	fcount;			/* Number of opened files	*/
extern	char	*memerror;		/* Out of memory error message	*/
extern	char	*lockerror;		/* Locking deadlock error	*/
extern	long	doffset;		/* Current data offset		*/
extern	struct	filetable	*ftbl;	/* Pointer to file environment	*/
extern	long	*fstack;		/* Stack of father offsets	*/
extern	int	stackptr;		/* Index into fstack (fathers)	*/	
extern	int	stackmax;		/* #of stack memory alloc's	*/
extern	int	kindex;			/* Index of key(n) in NODE	*/
extern	long	child;			/* Offset of next child NODE	*/
extern	US_LONG sunum;			/* Save area for uniqueness num */
extern	int	actsplit;		/* Signifies node split in	*/
					/* binsert();			*/
extern	long	nlock;
extern	int	iobptr;
extern	char	iob[];

/* prototypes */

void	autoulock(int isfd);
void	bdelete(int isfd, struct keydesc *keydesc, struct NODE *worknode);
void	binsert(int isfd, struct keydesc *keydesc, struct NODE *worknode,
		struct nodekey *workkey);
char	*bkey(char *record, struct keydesc *keydesc);
int	ccpress(struct keydesc *keydesc, struct NODE *worknode);
int	chkclose(int isfd);
int	chkkdesc(int recordlength, struct keydesc *keydesc);
int	chkrolock(int isfd);
struct	nodekey *collapse(struct NODE *collnode,
	struct keydesc *keydesc, struct nodekey *inskey,
	struct NODE *leftnode, struct NODE *rightnode, unsigned swapsw,
	int actln);
int	complen(struct keydesc *keydesc);
void	cpnode(struct NODE *tnode, struct NODE *snode,
	struct keydesc *keydesc);
void	ddiction(int isfd);
int	delrec(int isfd, char *passkey);
void	dkey(int isfd, struct keydesc *keydesc);
void	dkeydesc(int isfd);
void	dumpdata(int isfd, char *record, int increment);
void	dumpnode(int isfd, long offset, struct keydesc *keydesc,
			struct NODE *worknode);
void	fetchcrp(int isfd, char *record);
int	fetchdata(int isfd, char *passkey, char *datarec);
void	fillblk(int fh);
int	flenchk(char *filename);
void	freenode(struct NODE *worknode);
void	freespace(int isfd, long offset, int whichlist);
void	freetree(int isfd, struct keydesc *keydesc);
long	getspace(int isfd, int whichlist);
US_LONG getzapunum(char *keystring, struct keydesc *keydesc,
		US_LONG unum, int mode);
void	initftbl(struct filetable *file, int fdidx, int fddat, int mode);
int	iopen(char *filename, int mode);
int	isplock(int fildes, int function, long size);
int	kcmp(struct keydesc *keydesc, char *curkey, char *newkey, int length);
struct	nodekey *kdelete(struct NODE *worknode, int actln);
int	kdsccmp(char *newkey, char *curkey);
int	keyblks(int isfd);
int	keysize(struct keydesc *keydesc);
void	kinsert(struct NODE *worknode, struct nodekey *workkey, int actln);
struct	nodekey *ksalloc(void);
void	ldiction(int isfd);
void	lkey(int isfd, struct keydesc *keydesc);
void	lkeydesc(int isfd);
void	loaddata(int isfd, char *record, long offset);
void	loadnode(int isfd, long offset, struct keydesc *keydesc,
		struct NODE *worknode);
int	locktree(int isfd);
void	markdata(int isfd, long daddr, char deadoralive);
int	modtree(int isfd, char *passkey, struct keydesc *keydesc, int mode);
struct	nodekey **nkalloc(int nnk);
struct	nodekey **nkrealloc(struct nodekey **aptr, int nnk);
int	nocurr(int isfd);
void	padkey(char *key, int start, short max);
long	pop(void);
void	push(long address);
void	putndata(char *buf, unsigned nchars);
void	rstnl(struct keydesc *keydesc, struct NODE *worknode);
int	rwrec(int isfd, char *passkey, char *record);
char	*salloc(int nchars);
int	sequential(struct keydesc *keydesc, char *newkey,
		struct NODE *worknode, int length);
int	setrecptr(int isfd, struct keydesc *keydesc, int length,
		char *record, int mode);
void	shiftidx(int isfd, int dindex, int kcount);
struct	nodekey *split(struct NODE *worknode, struct keydesc *keydesc,
		struct NODE *lnode, struct NODE *rnode);
struct	nodekey *splitnode(int isfd, struct NODE *worknode,
		struct keydesc *keydesc);
int	travtree(int isfd, int direction, struct NODE *worknode,
		struct keydesc *keydesc, char *reckey, int length);
int	treesearch(int isfd, struct keydesc *keydesc, char *reckey,
		struct NODE *worknode, int length);
int	ulocktree(int isfd);
void	underflow(int isfd, struct keydesc *keydesc, struct NODE *worknode);

#endif
