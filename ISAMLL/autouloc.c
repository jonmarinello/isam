#include "iisam.h"

void autoulock(int isfd)
{
#ifndef NO_LOCKING
        struct  filetable       *file = &ftbl[isfd];    /* Efficiency   */

        if (file->olockoffset < 0L || !(file->omode & ISAUTOLOCK))
                return;
        errno = 0;
        lseek(ISDFD, file->olockoffset, 0);
        if (isplock(ISDFD, F_ULOCK, file->dptr.datalength)) {
                fprintf(stderr, lockerror);
                abort();
        }
        file->olockoffset = -1L;
#else
		isfd = isfd;
        errno = 0;
#endif
        return;
}
