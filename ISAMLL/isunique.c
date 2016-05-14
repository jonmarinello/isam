#include "iisam.h"

/*      Return a unique id number.      */

int	isuniqueid(int isfd, long *uniqueid)
{
        iserrno = 0;
        if (chkclose(isfd) < 0)
                return(-1);
        if (chkrolock(isfd) < 0)
                return(-1);
        locktree(isfd);
        ldiction(isfd);
        *uniqueid = ftbl[isfd].dptr.unique++;
        ddiction(isfd);
        autoulock(isfd);
        ulocktree(isfd);
        return(0);
}
