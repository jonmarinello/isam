#include "iisam.h"

/*      Return either a given key description or the dictionary itself  */
/*      depending on wether number is greater than zero or zero         */
/*      respectively.                                                   */

int	isindexinfo(int isfd, struct keydesc *buffer, int number)
{
        struct  dictinfo        userdict;       /* internal buffer      */
        struct  filetable       *file = &ftbl[isfd];    /* Efficiency   */

        iserrno = 0;
        if (chkclose(isfd) < 0)
                return(-1);
        locktree(isfd);
        if (!number) {  /* Return dictinfo structure */
                ldiction(isfd);
                userdict.di_nkeys = (int)file->dptr.dsccnt;
                userdict.di_recsize = (int)file->dptr.datalength;
                userdict.di_idxsize = (int)PAGESIZE;
                userdict.di_nrecords = (int)DATACOUNT;
		memcpy((char *)buffer, (char *)&userdict, DISIZE);
                autoulock(isfd);
                ulocktree(isfd);
                return(0);
        }
        if (--number >= PRIMARYKEY && number < file->dptr.dsccnt) {
		memcpy((char *)buffer, (char *)&file->descptr[number],
                        KDSIZE);
                autoulock(isfd);
                ulocktree(isfd);
                return(0);
        }
        ulocktree(isfd);
        iserrno = EBADARG;
        return(-1);
}
