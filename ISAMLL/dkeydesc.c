#include "iisam.h"

/*      Dump all the key descriptions for a given file to disk.         */

void	dkeydesc(int isfd)
{
        int     i;
        int     blocks = 0;     /* Number of pages needed for key       */
                                /* description list on disk             */
        struct  keydesc         *keydesc;               /* Efficiency   */
        struct  keyblock        *keyblock;              /* Efficiency   */
        struct  filetable       *file = &ftbl[isfd];    /* Efficiency   */

        keyblock = &file->kbptr[0];
        keyblock->bcount = IDSIZE + KBSIZE;
        lseek(ISIFD, (long)(IDSIZE + KBSIZE), 0);
        for (i = 0; i < file->dptr.dsccnt; ++i) {
                keydesc = &file->descptr[i];
                if (keyblock->bcount + keysize(keydesc) > PAGESIZE) {
                        if (!blocks)
                                lseek(ISIFD, (long)IDSIZE, 0);
                        else
                                lseek(ISIFD, file->kbptr[blocks - 1].kchain, 0);
                        write(ISIFD, (char *)keyblock, KBSIZE);
                        lseek(ISIFD, keyblock->kchain + (long)KBSIZE, 0);
                        keyblock = &file->kbptr[++blocks];
                        keyblock->bcount = KBSIZE;
                }
                dkey(isfd, keydesc);
                keyblock->bcount += keysize(keydesc);
        }
        if (!blocks)
                lseek(ISIFD, (long)IDSIZE, 0);
        else
                lseek(ISIFD, file->kbptr[blocks - 1].kchain, 0);
        write(ISIFD, (char *)keyblock, KBSIZE);
}
