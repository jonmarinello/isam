#include "iisam.h"

/*      Add an offset to either the index or data free space list.      */

void	freespace(int isfd, long offset, int whichlist)
{
        struct  flist   flpage;         /* Free list page               */
        long    *pfree;                 /* Pointer to end of freelist   */

        switch (whichlist) {
                case INDEXFREELIST :
                        pfree = &ftbl[isfd].dptr.kfree;
                        break;
                case DATAFREELIST :
                        pfree = &ftbl[isfd].dptr.dfree;
        }
        if (*pfree == NILL) {   /* freelist empty so initialize it */
                *pfree = lseek(ISIFD, 0L, 2);
                flpage.llink = NILL;
                flpage.foffsets[flpage.fidx = 0L] = offset;
                write(ISIFD, (char *)&flpage, (unsigned)PAGESIZE);
                return;
        }
        lseek(ISIFD, *pfree, 0);
        read(ISIFD, (char *)&flpage, (unsigned)PAGESIZE);
        if (++flpage.fidx < FREESIZE) {         /* Fits in this free page */
                flpage.foffsets[flpage.fidx] = offset;
                lseek(ISIFD, *pfree, 0);
                write(ISIFD, (char *)&flpage, (unsigned)PAGESIZE);
                return;
        }
        /* Allocate a new free page to free list */
        flpage.llink = *pfree;
        *pfree = lseek(ISIFD, 0L, 2);
        flpage.foffsets[flpage.fidx = 0L] = offset;
        write(ISIFD, (char *)&flpage, (unsigned)PAGESIZE);
}
