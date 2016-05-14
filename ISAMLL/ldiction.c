#include "iisam.h"

/*      Load a given isam files dictionary from disk.           */

void	ldiction(int isfd)
{
        lseek(ISIFD, 0L, 0);
        read(ISIFD, (char *)&ftbl[isfd].dptr, IDSIZE);
}
