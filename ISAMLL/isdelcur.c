#include "iisam.h"

/*      Delete the record pointed to by the 'Current record pointer'.   */

int	isdelcurr(int isfd)
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
        locktree(isfd);
        ldiction(isfd);
        if (delrec(isfd, ftbl[isfd].curkey)) {
                ulocktree(isfd);
                return(-1);
        }
        markdata(isfd, doffset, DEAD);
        ddiction(isfd);
        autoulock(isfd);
        ulocktree(isfd);
        return(0);
}
