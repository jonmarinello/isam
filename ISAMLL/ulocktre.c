#include "iisam.h"

/*      Unlock B-tree (This is internal).       */

int	ulocktree(int isfd)
{
        if (ftbl[isfd].omode & ISEXCLLOCK)      /* No unlock if EXCL lock */
                return(0);
        errno = 0;
        lseek(ISIFD, (long)(7 * sizeof(long)), 0);
        isplock(ISIFD, F_ULOCK, 0L);
        return(0);
}
