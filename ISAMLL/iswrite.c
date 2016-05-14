#include "iisam.h"

/*      Write 'record' to a given isam file.    */

int	iswrite(int isfd, char *record)
{
        int     i, j;
        char    *reckey;                /* Work keyval pointer          */
        struct  keydesc *keydesc;       /* Pointer to different index   */
                                        /* tree key descriptions        */
        struct  filetable       *file = &ftbl[isfd];    /* Efficiency   */
        long    daddr;                  /* Data record offset save area */

        iserrno = 0;
        if (chkclose(isfd) < 0)
                return(-1);
        if (!(file->omode & LOCKMASK)) {
                iserrno = EINPUTONLY;
                return(-1);
        }
        if (chkrolock(isfd) < 0)
                return(-1);
        locktree(isfd);
        ldiction(isfd);
        dumpdata(isfd, record, 1);
        daddr = doffset;
        for (i = PRIMARYKEY; i < file->dptr.dsccnt; ++i) {
                keydesc = &file->descptr[i];
                reckey = bkey(record, keydesc);
                getzapunum(reckey, keydesc, HIGHUSTART, ZAP);
                if (modtree(isfd, reckey, keydesc, INSERT)) {
                        for (j = i - 1; j >= PRIMARYKEY; --j) {
                                keydesc = &file->descptr[j];
                                reckey = bkey(record, keydesc);
                                getzapunum(reckey, keydesc, UNIQUECOUNT, ZAP);
                                modtree(isfd, reckey, keydesc, DELETE);
                        }
                        --UNIQUECOUNT;
                        markdata(isfd, daddr, DEAD);
                        ddiction(isfd);
                        ulocktree(isfd);
                        return(-1);
                }
        }
        ddiction(isfd);
        autoulock(isfd);
        ulocktree(isfd);
        return(0);
}
