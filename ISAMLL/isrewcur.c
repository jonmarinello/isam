#include "iisam.h"

/*      Rewrite 'record' using the 'Current record pointer'             */

int	isrewcurr(int isfd, char *record)
{

        iserrno = 0;
        if (chkclose(isfd) < 0)
                return(-1);
        if (chkrolock(isfd) < 0)
                return(-1);
        if (ftbl[isfd].startsw) {
                iserrno = ENOCURR;
                return(-1);
        }
        if (nocurr(isfd))
                return(-1);

        /* Fetch 'Current record pointer' into 'record' */

        locktree(isfd);
        fetchcrp(isfd, record);
        if (rwrec(isfd, ftbl[isfd].curkey, record)) {
                ulocktree(isfd);
                return(-1);
        }
        autoulock(isfd);
        ulocktree(isfd);
        return(0);
}
