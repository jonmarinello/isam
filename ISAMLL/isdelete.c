#include "iisam.h"

/*      Delete 'record' from the given isam file.                       */

int	isdelete(int isfd, char *record)
{
        char    *reckey;                /* Pointer to key made from     */
                                        /* user's 'record'              */

        iserrno = 0;
        if (chkclose(isfd) < 0)
                return(-1);
        if (chkrolock(isfd) < 0)
                return(-1);
        locktree(isfd);
        ldiction(isfd);
        reckey = bkey(record, &ftbl[isfd].descptr[ftbl[isfd].livekey]);
        if (delrec(isfd, reckey)) {
                free(reckey);
                ulocktree(isfd);
                return(-1);
        }
        markdata(isfd, doffset, DEAD);
        ddiction(isfd);
        free(reckey);
        autoulock(isfd);
        ulocktree(isfd);
        return(0);
}
