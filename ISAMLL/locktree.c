#include "iisam.h"

/*      Lock B-tree (This is internal). */

int	locktree(int isfd)
{
#ifndef NO_LOCKING
	errno = 0;
	if (ftbl[isfd].omode & ISEXCLLOCK)      /* No lock if EXCL lock */
		return(0);
	lseek(ISIFD, (long)(7 * sizeof(long)), 0);
	isplock(ISIFD, F_LOCK, 0L);
#else
	isfd = 0;
#endif
	return(0);
}
