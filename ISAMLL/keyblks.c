#include "iisam.h"

/*      Compute the number of blocks needed to hold all key description
        records. Note that as far as disk space is concerned, the key
        descriptions are of variable length.
*/

int	keyblks(int isfd)
{
        int     i;
        int     tkb = 0;                        /* Total keydesc bytes*/
        int     epsz = (int)PAGESIZE - KBSIZE;  /* Effective pagesize */

        for (i = 0; i < ftbl[isfd].dptr.dsccnt; ++i)
                tkb += keysize(&ftbl[isfd].descptr[i]);
        if (tkb % epsz != 0)
                return(tkb / epsz + 1);
        else
                return(tkb / epsz);
}
