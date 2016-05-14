#include "iisam.h"

/*      Start on the given isam file    */

int	isstart(int isfd, struct keydesc *keydesc, int length, char *record,
		int mode)
{
        int     descloop;       /* Key description search counter       */
        int     savedesc;       /* Livekey save buffer in case of error */
        int     descfound = 0;  /* Key description search found switch  */
        int     i;              /* Loop counter                         */
        struct  filetable       *file = &ftbl[isfd];    /* Efficiency   */

        iserrno = 0;
        if (length < 0 || length > keydesc->k_len) {
                iserrno = EBADARG;
                return(-1);
        }
        if (chkclose(isfd) < 0)
                return(-1);
        if ((file->omode & LOCKMASK) == ISOUTPUT) {
                iserrno = EOUTPUTONLY;
                return(-1);
        }
        savedesc = file->livekey;
        for (descloop = 0; descloop < file->dptr.dsccnt; ++descloop) {
                if (keydesc->k_rootnode == file->descptr[descloop].k_rootnode) {
                        ++descfound;
                        break;
                }
        }
        if (!descfound) {
                iserrno = EBADKEY;
                return(-1);
        }
        file->livekey = descloop;
        locktree(isfd);
        setrecptr(isfd, &file->descptr[descloop], length, record, mode);
        if (mode == ISFIRST || mode == ISLAST)
                iserrno = 0;
        if (!iserrno) {
                if (mode == ISLAST)
                        ++file->lastsw;
                else
                        file->lastsw = 0;
                ++file->startsw;
                autoulock(isfd);
                ulocktree(isfd);
                return(0);
        }
        for (i = 0; i < keydesc->k_len; ++i)
                file->curkey[i] = HIGHVALUES;
        file->startsw = 0;
        file->livekey = savedesc;
        ulocktree(isfd);
        return(-1);
}
