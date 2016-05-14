#include "iisam.h"

/*      Rewrite 'record'.       */

int	isrewrite(int isfd, char *record)
{
        char    *reckey;        /* Work keyval buffer   */

        iserrno = 0;
        if (chkclose(isfd) < 0)
                return(-1);
        if (chkrolock(isfd) < 0)
                return(-1);
        locktree(isfd);
        reckey = bkey(record, &ftbl[isfd].descptr[ftbl[isfd].livekey]);
        if (rwrec(isfd, reckey, record)) {
                free(reckey);
                ulocktree(isfd);
                return(-1);
        }
        free(reckey);
        autoulock(isfd);
        ulocktree(isfd);
        return(0);
}
