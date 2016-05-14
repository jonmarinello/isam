#include "iisam.h"

/*      Load a data record from disk.   */

void	loaddata(int isfd, char *record, long offset)
{
        lseek(ISDFD, offset, 0);
        read(ISDFD, (char *)record, (unsigned)ftbl[isfd].dptr.datalength);
}
