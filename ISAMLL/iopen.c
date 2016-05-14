#include "iisam.h"

char    *isversnumber = "version 3.00";
char    *iscopyright = "Copyright (C) 1986, Jon E. Marinello";
long    isserial = 1L;
int     iserrno;
int     iserrio;
char    isstat1;
char    isstat2;
int     fcount = (-1);
long    doffset;
long    *fstack;
int     stackptr;
int     stackmax = 0;
int     kindex;
long    child;
char    *memerror = "Isam fatal error: can't allocate any more memory.\n";
char    *lockerror = "Isam fatal error: can't lock resource, deadlock.\n";
int     actsplit;
US_LONG sunum;
struct  filetable       *ftbl;
int     is_nerr = 121;
char    *is_errlist[] = {
        "Duplicate record",
        "File not open",
        "Illegal argument",
        "Bad key descriptor",
        "Too many files",
        "Corrupted isam file",
        "Need exclusive access",
        "Record or file locked",
        "Index already exists",
        "Primary index",
        "End of file",
        "Record not found",
        "No current record",
        "File is in use",
        "File name too long",
        "Bad lock device",
        "File open for input only",
        "File open for output only",
        "Not in manual locking mode",
        "Invalid lock definition",
        "Bad file version"
};
int     iobptr;
char    iob[PAGESIZE];

int	iopen(char *filename, int mode)
{
        int     fdidx, fddat;   /* '.idx' & '.dat' file pointers        */      
        char    *idxname;       /* Filename work areas                  */
        char    *datname;
        int     envptr;         /* Maybe use previously closed file area*/
        int     existenv = 0;   /* Available area switch                */

        umask(000);
        if (flenchk(filename))
                return(-1);
        for (envptr = 0; envptr < fcount; ++envptr)
                if (ftbl[envptr].omode < 0) {
                        ++existenv;
                        break;
                }
        idxname = salloc(strlen(filename)+5);
        datname = salloc(strlen(filename)+5);
        sprintf(idxname, "%s.idx", filename);
        sprintf(datname, "%s.dat", filename);
        errno = 0;
        switch(mode & LOCKMASK) {
                case ISCREATE+ISINPUT : /* isbuild() calling */
                case ISCREATE+ISOUTPUT :
                case ISCREATE+ISINOUT :
                        if (!(mode & ISEXCLLOCK)) {
                                iserrno = ENOTEXCL;
                                return(-1);
                        }
                        if ((fdidx = open(idxname, ISCERW, 0666)) < 0) {
                                iserrno = errno;
                                return(-1);
                        }
                        if ((fddat = open(datname, ISCERW, 0666)) < 0) {
                                iserrno = errno;
                                close(fdidx);
                                return(-1);
                        }
                        mode -= ISCREATE;       /* get mode into good format */
                        break;
                case ISINPUT :  /* isopen() calling */
                case ISINOUT :
                case ISOUTPUT :
                        if ((fdidx = open(idxname, O_RDWR)) < 0) {
                                iserrno = errno;
                                return(-1);
                        }
                        if ((fddat = open(datname, O_RDWR)) < 0) {
                                iserrno = errno;
                                close(fdidx);
                                return(-1);
                        }
                        break;
                default :
                        iserrno = EBADARG;
                        free(idxname);
                        free(datname);
                        return(-1);
        }
        free(idxname);
        free(datname);
        if ((mode & ISEXCLLOCK)) {
                lseek(fdidx, 0L, 0);
		if (isplock(fdidx, F_TEST, 0L)) {
                        iserrno = EFLOCKED;
                        close(fdidx);
                        close(fddat);
                        return(-1);
                }
                lseek(fddat, 0L, 0);
		if (isplock(fddat, F_TEST, 0L)) {
                        iserrno = EFLOCKED;
                        close(fdidx);
                        close(fddat);
                        return(-1);
                }
        }
        if (existenv) {
                initftbl(&ftbl[envptr], fdidx, fddat, mode);
                return(envptr);
        }
        if (!(++fcount))
                ftbl = (struct filetable *)salloc(FTSIZE);
        else {
                if ((ftbl = (struct filetable *)realloc((char *)ftbl,
                        (unsigned)((fcount + 1) * FTSIZE))) == NULL) {
                                fprintf(stderr, memerror);
                                abort();
                }
        }
        initftbl(&ftbl[fcount], fdidx, fddat, mode);
        return(fcount);
}
