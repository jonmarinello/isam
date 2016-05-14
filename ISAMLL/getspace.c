#include "iisam.h"

/*      Retrieve an offset from either the index or data free list.     */
/*      If the given list is empty, return the offset to EOF.           */

long    getspace(int isfd, int whichlist)
{
        struct  flist   flpage;         /* Free list page               */
        long    *pfree;                 /* Pointer to end of freelist   */
        int     fd;                     /* File pointer to whichlist    */
        long    fchain;                 /* Chain to last freelist block */
        long    freeaddr;               /* Available offset due to the  */
                                        /* free list shrinking          */

        switch (whichlist) {
                case INDEXFREELIST :
                        pfree = &ftbl[isfd].dptr.kfree;
                        fd = ISIFD;
                        break;
                case DATAFREELIST :
                        pfree = &ftbl[isfd].dptr.dfree;
                        fd = ISDFD;
        }
        if (*pfree == NILL)
                return(lseek(fd, 0L, 2));
        lseek(ISIFD, *pfree, 0);
        read(ISIFD, (char *)&flpage, (unsigned)PAGESIZE);
        if (flpage.fidx >= 0) {
                --flpage.fidx;
                lseek(ISIFD, *pfree, 0);
                write(ISIFD, (char *)&flpage, (unsigned)PAGESIZE);
                return(flpage.foffsets[flpage.fidx + 1]);
        }
        if (flpage.llink == NILL) {
                freeaddr = *pfree;
                *pfree = NILL;
                freespace(isfd, freeaddr, INDEXFREELIST);
                return(lseek(fd, 0L, 2));
        }
        fchain = *pfree;
        freeaddr = flpage.llink;
        lseek(ISIFD, flpage.llink, 0);
        read(ISIFD, (char *)&flpage, (unsigned)PAGESIZE);
        lseek(ISIFD, fchain, 0);
        --flpage.fidx;
        write(ISIFD, (char *)&flpage, (unsigned)PAGESIZE);
        freespace(isfd, freeaddr, INDEXFREELIST);
        return(flpage.foffsets[flpage.fidx + 1]);
}
