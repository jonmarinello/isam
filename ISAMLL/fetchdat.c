#include "iisam.h"

/*      Fetch a data passkey from disk (Used by the deletion and rewrite        */
/*      functions.                                                      */

int	fetchdata(int isfd, char *passkey, char *datarec)
{
        struct  keydesc *keydesc = &ftbl[isfd].descptr[ftbl[isfd].livekey];
        struct  NODE    worknode;       /* Work node for treesearch()      */

        if (treesearch(isfd, keydesc, passkey, &worknode, WHOLEKEY) == NOTFOUND) {
                freenode(&worknode);    
                iserrno = ENOREC;
                return(-1);
        }
        sunum = getzapunum(worknode.kptr[kindex]->keyval, keydesc, DUMMYGET, GET);
        /* 11/07/85 Joma - Changed code to use:

                "worknode.kptr[kindex]->dataptr"

                instead of:

                "doffset"

                For following lseek command so locking works in rewrite
        */
        /* Test for ISAUTOLOCK & ISMANULOCK */
        if (!(ftbl[isfd].omode & ISEXCLLOCK)) {
                lseek(ISDFD, worknode.kptr[kindex]->dataptr, 0);
                if (isplock(ISDFD, F_TEST, ftbl[isfd].dptr.datalength)) {
                        freenode(&worknode);    
                        iserrno = ELOCKED;
                        return(-1);
                }
                lseek(ISDFD, worknode.kptr[kindex]->dataptr +
                        ftbl[isfd].dptr.datalength, 0);
                if (isplock(ISDFD, F_TEST, 1L)) {
                        freenode(&worknode);    
                        iserrno = ELOCKED;
                        return(-1);
                }
        }
        loaddata(isfd, datarec, worknode.kptr[kindex]->dataptr);
        freenode(&worknode);    
        return(0);
}
