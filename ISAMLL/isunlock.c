#include "iisam.h"

int	isunlock(int isfd)
{
	errno = iserrno = 0;
#ifndef NO_LOCKING
	if (chkclose(isfd) < 0)
		return(-1);
	if (!(ftbl[isfd].omode & ISMANULOCK)) {
		iserrno = ENOTMANULOCK;
		return(-1);
	}
	lseek(ISIFD, (long)(6 * LONGSIZE), 0);
	isplock(ISIFD, F_ULOCK, (long)LONGSIZE);
#else
	isfd = 0;
#endif
        return(0);
}
