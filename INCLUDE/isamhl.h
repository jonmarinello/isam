/************************************************************************/
/*									*/
/*	- HIGH LEVEL ISAM SUBROUTINE MACRO & DATA DESCRIPTIONS -	*/
/*									*/
/************************************************************************/

#define bcreate(n, fm, d, m, fl, fmc) \
	Isbuild(fm, (char *)&d, n, "", fl, fmc, m)

#define bcretmp(n, an, fm, d, m, fl, fmc) \
	Isbuild(fm, (char *)&d, n, an, fl, fmc, m)

#define baddind(f, fl, fmc) \
	isaddindex(f->fildes, Icle(f, f->ffmt, fl, fmc))

#define bdelind(f, c) \
	(isindexinfo(f->fildes, &Ltmpc, c) || isdelindex(f->fildes, &Ltmpc))

#define bopen(n, fm, d, m)	Isopen(n, m, fm, (char *)&d)
#define bclose(f)		Isclose(&f)
#define bstart(f, c, l, m)	Isstart(f, m, c, l)
#define bread(f, m)		Isread(f, m)
#define bwrite(f)		Iswrite(f)
#define bwcurr(f)		Iswrcurr(f)
#define brewrite(f)		Isrewrite(f)
#define brwcurr(f)		Isrewcurr(f)
#define bdelete(f) 		Isdelete(f)
#define bdelcurr(f)		Isdelcurr(f)
#define berase(n)		Iserase(n)
#define bunique(f, x)		isuniqueid(f->fildes, x)
#define block(f)		Islock(f)
#define bunlock(f)		isunlock(f->fildes)
#define brelease(f)		isrelease(f->fildes)

typedef struct bfile {
	int	fildes;
	char	*ffmt;
	char	*fdata;
	short	*alnaddr;
	short	ssize;
	char	*physname;
} BFILE;

extern	struct	keydesc *bkey();
extern	BFILE *Iinit();
extern	BFILE *Isbuild();
extern	BFILE *Isopen();
extern	long	isbellcnt;
extern	struct	keydesc Ltmpc;
