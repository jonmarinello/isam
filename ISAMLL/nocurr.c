#include "iisam.h"

/*      Determine if there is a current file pointer.   */

int	nocurr(int isfd)
{
        struct  filetable       *file = &ftbl[isfd];    /* Efficiency   */
        int     i;

        for (i = 0; i < file->descptr[file->livekey].k_len; ++i)
                if (((unsigned)file->curkey[i] != (unsigned)HIGHVALUES))
                        return(0);
        iserrno = ENOCURR;
        return(-1);
}
