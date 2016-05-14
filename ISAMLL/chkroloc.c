#include "iisam.h"

/* Check if another process has write only lock on given isam file.     */

int	chkrolock(int isfd)
{
	errno = 0;
#ifndef NO_LOCKING
	if (ftbl[isfd].omode & ISEXCLLOCK)
		return(0);
	lseek(ISIFD, (long)(6 * sizeof(long)), 0);
	if (isplock(ISIFD, F_TEST, (long)sizeof(long))) {
		iserrno = EFLOCKED;
		return(-1);
	}
#endif
        return(0);
}
