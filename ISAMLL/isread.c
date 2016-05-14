#include "iisam.h"

/*      Read an isam file record.       */

int	isread(int isfd, char *record, int mode)
{
        struct  keydesc *keydesc;       /* Pointer to live keydesc      */
        struct  filetable       *file = &ftbl[isfd];    /* Efficiency   */
        int     rlmode = (mode & ISLOCK);       /* Record lock mode     */
        int     flmode = ftbl[isfd].omode;
        char    skey[INTERNALKEYSZ];

        iserrno = 0;
        if (rlmode && !(flmode & ISMANULOCK)) {
                iserrno = EBADARG;
                return(-1);
        }
        if (chkclose(isfd) < 0)
                return(-1);
        if ((file->omode & LOCKMASK) == ISOUTPUT) {
                iserrno = EOUTPUTONLY;
                return(-1);
        }
        keydesc = &file->descptr[file->livekey];
        if ((mode == ISCURR) && file->lastsw) {
                iserrno = ENOCURR;
                return(-1);
        }
        locktree(isfd);
	memcpy(skey, file->curkey, file->descptr[file->livekey].k_len);
        switch (mode) {
                case ISNEXT :
                        if (file->lastsw) {
                                iserrno = EENDFILE;
                                ulocktree(isfd);
                                return(-1);
                        }
                        if (file->startsw) {
                                setrecptr(isfd, keydesc, 0, record, ISCURR);
                                break;
                        }
                        setrecptr(isfd, keydesc, 0, record, ISNEXT);
                        break;
                case ISPREV :
                        if (file->lastsw) {
                                setrecptr(isfd, keydesc, 0, record, ISCURR);
                                break;
                        }
                        setrecptr(isfd, keydesc, 0, record, ISPREV);
                        break;
                default :
                        setrecptr(isfd, keydesc, 0, record, mode & ~ISLOCK);
        }
        if (iserrno) {
                ulocktree(isfd);
                if (mode == ISCURR)
                        iserrno = EENDFILE;
                return(-1);
        }
        if (flmode & ISEXCLLOCK)
                goto LDD;
        lseek(ISDFD, doffset, 0);       /* Test for ISAUTOLOCK */
        if (isplock(ISDFD, F_TEST, file->dptr.datalength)) {
                iserrno = ELOCKED;
                goto ERX;
        }
        /* 11/03/85 - Joma Changed code to handle ISMANULOCK properly   */
        if ((flmode & ISMANULOCK) && rlmode) {  /* ISMANULOCK   */
                if (chkrolock(isfd) < 0) {
                        iserrno = EFLOCKED;
                        goto ERX;
                }
                lseek(ISDFD, doffset + file->dptr.datalength, 0);
		if (isplock(ISDFD, F_TEST, 1L)) {
			iserrno = ELOCKED;
			goto ERX;
		}
	}
	else if (flmode & ISAUTOLOCK) {         /* ISAUTOLOCK   */
		if (chkrolock(isfd) < 0) {
			iserrno = EFLOCKED;
			goto ERX;
		}
		lseek(ISDFD, doffset, 0);
		if (isplock(ISDFD, F_TEST, file->dptr.datalength)) {
			iserrno = ELOCKED;
			goto ERX;
		}
		/* Don't unlock if its the same record read */
		if (file->olockoffset != doffset) {
			autoulock(isfd);
			file->olockoffset = doffset;
		}
	}
	/* 11/03/85 - Joma End of modification */
LDD:    loaddata(isfd, record, doffset);
	file->startsw = file->lastsw = 0;
	ulocktree(isfd);
	return(0);
ERX:    memcpy(file->curkey, skey,
		(int)file->descptr[file->livekey].k_len);
	ulocktree(isfd);
	return(-1);
}
