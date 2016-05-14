#include "iisam.h"

/*      Load all the key descriptions for a given isam file from disk.  */

void	lkeydesc(int isfd)
{
        int     i;
        int     blocks = 0;                     /* Block counter        */
        int     bytes;                          /* Byte counter         */
        struct  keydesc         *keydesc;               /* Efficiency   */
        struct  keyblock        *keyblock;              /* Efficiency   */
        struct  filetable       *file = &ftbl[isfd];    /* Efficiency   */

        if ((file->descptr =
                (struct keydesc *)malloc((unsigned)(file->dptr.dsccnt *
                KDSIZE))) == NULL) {
                        fprintf(stderr, memerror);
                        abort();
        }
        if ((file->kbptr =
                (struct keyblock *)malloc((unsigned)KBSIZE)) == NULL) {
                        fprintf(stderr, memerror);
                        abort();
        }
        lseek(ISIFD, (long)IDSIZE, 0);
        keydesc = &file->descptr[0];
        keyblock = &file->kbptr[0];
        read(ISIFD, (char *)keyblock, KBSIZE);
        lkey(isfd, keydesc);
        bytes = IDSIZE + KBSIZE + keysize(keydesc);
        for (i = 1; i < file->dptr.dsccnt; ++i) {
                if (bytes == keyblock->bcount) {
                        lseek(ISIFD, keyblock->kchain, 0);
                        if ((file->kbptr = (struct keyblock *)
                                realloc((char *)file->kbptr,
                                (unsigned)((++blocks + 1) * KBSIZE))) == NULL) {
                                fprintf(stderr, memerror);
                                abort();
                        }
                        keyblock = &file->kbptr[blocks];
                        read(ISIFD, (char *)keyblock, KBSIZE);
                        bytes = KBSIZE;
                }
                keydesc = &file->descptr[i];
                lkey(isfd, keydesc);
                bytes += keysize(keydesc);
        }
}
