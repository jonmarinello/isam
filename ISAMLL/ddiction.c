#include "iisam.h"

/*      Dump dictionary to disk at the beginning of the index file.     */

void	ddiction(int isfd)
{
        lseek(ISIFD, 0L, 0);
        write(ISIFD, (char *)&ftbl[isfd].dptr, IDSIZE);
}
