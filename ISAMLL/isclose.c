#include "iisam.h"

/*      Close the given isam file       */

int	isclose(int isfd)
{
        iserrno = 0;
        if (chkclose(isfd))
                return(-1);
        close(ISIFD);
        close(ISDFD);
        ftbl[isfd].omode = (-1);
        if (ftbl[isfd].descptr)
                free((char *)ftbl[isfd].descptr);
        if (ftbl[isfd].kbptr)
                free((char *)ftbl[isfd].kbptr);
        return(0);
}
