#include "iisam.h"

int	isrelease(int isfd)
{
	iserrno = 0;
#ifndef NO_LOCKING
	if (chkclose(isfd) < 0)
		return(-1);
	if (!(ftbl[isfd].omode & ISMANULOCK)) {
		iserrno = ENOTMANULOCK;
		return(-1);
	}
	lseek(ISDFD, 0L, 0);
	isplock(ISDFD, F_ULOCK, 0L);
#else
	isfd = 0;
#endif
        return(0);
}
