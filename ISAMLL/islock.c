#include "iisam.h"

int	islock(int isfd)
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
	if (isplock(ISIFD, F_TEST, (long)LONGSIZE)) {
		iserrno = EFLOCKED;
		return(-1);
	}
#endif
        return(0);
}
