#include "iisam.h"

/*      Delete an index tree from the given isam file.                  */

int	isdelindex(int isfd, struct keydesc *keydesc)
{
        int     i;
        int     descfound = 0;  /* Key description search switch        */
        int     oblocks;        /* How many pages needed before add     */
        int     nblocks;        /* How many pages needed after add      */
        struct  filetable       *file = &ftbl[isfd];    /* Efficiency   */
        long    *nplace = &file->dptr.dsccnt;   /* Pointer to key       */
                                                /* description count    */

        iserrno = 0;
        if (chkclose(isfd) < 0)
                return(-1);
        if (!(file->omode & ISEXCLLOCK)) {
                iserrno = ENOTEXCL;
                return(-1);
        }
        if (!(file->omode & LOCKMASK)) {
                iserrno = EINPUTONLY;
                return(-1);
        }
        for (i = 0; i < *nplace; ++i) {
                if (keydesc->k_rootnode == file->descptr[i].k_rootnode) {
                        ++descfound;
                        break;
                }
        }
        if (descfound && i == PRIMARYKEY) {
                iserrno = EPRIMKEY;
                return(-1);
        }
        if (!descfound) {
                iserrno = EBADKEY;
                return(-1);
        }
        ldiction(isfd);
        oblocks = keyblks(isfd) - 1;
        shiftidx(isfd, i, (int)(--(*nplace)));
        nblocks = keyblks(isfd) - 1;
        if (oblocks != nblocks)
                freespace(isfd, file->kbptr[oblocks].kchain, INDEXFREELIST);
        dkeydesc(isfd);
        free((char *)file->descptr);
        free((char *)file->kbptr);
        lkeydesc(isfd);
        freetree(isfd, keydesc);
        ddiction(isfd);
        autoulock(isfd);
        return(0);
}
