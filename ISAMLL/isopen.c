#include "iisam.h"

/*      Open an isam file and position the 'Current record pointer' on  */
/*      the first record.                                               */

int	isopen(char *filename, int mode)
{
        int     isfd;   /* Pointer into filetable       */
        int     sern;

        iserrno = 0;
        if ((isfd = iopen(filename, mode)) < 0)
                return(-1);
        if (chkclose(isfd)) {
                sern = iserrno;
                isclose(isfd);
                iserrno = sern;
                return(-1);
        }
        locktree(isfd);
        ldiction(isfd);
        if (strncmp(ftbl[isfd].dptr.vn, isversnumber, 12)) {
                isclose(isfd);
                iserrno = EBADVERSION;
                return(-1);
        }
        lkeydesc(isfd);
        ulocktree(isfd);
        isstart(isfd, &ftbl[isfd].descptr[PRIMARYKEY], 0, " ", ISFIRST);
        iserrno = 0;
        return(isfd);
}
