#include "iisam.h"

/*      Check to see if a given file is closed or if isfd is out of     */
/*      range (ie. A bad parameter.                                     */
/*      Also check to see if another process has gotten exclusive       */
/*      lock access (ISEXCLLOCK) to the given file.                     */

int	chkclose(int isfd)
{
        struct  filetable       *file = &ftbl[isfd];    /* Efficiency   */

        if (fcount < 0 || isfd < 0 || isfd > fcount || file->omode < 0) {
                iserrno = ENOTOPEN;
                return(-1);
        }
        if (file->omode & ISEXCLLOCK)
                return(0);
        lseek(ISIFD, 0L, 0);
        if (isplock(ISIFD, F_TEST, (long)sizeof(long))) {
                iserrno = EFLOCKED;
                return(-1);
        }
        return(0);
}
