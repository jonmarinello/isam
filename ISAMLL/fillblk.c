#include "iisam.h"

/*      Fill the remainder of a disk page to the next page boundry.     */

void	fillblk(int fh)
{
        char    c = '\0';       /* NULL character fill  */
        long    offset;         /* start of fill        */

        iobptr = 0;
        offset = lseek(fh, 0L, 1);
        while ((offset++ % PAGESIZE) != 0L)
                putndata((char *)&c, 1);        /* Bufferize output */
        write(fh, iob, (unsigned)iobptr);
}
