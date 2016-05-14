#include "iisam.h"

/*      Dump a data record to disk and update record and uniqueness     */
/*      counts within the isam dictionary.                              */

void	dumpdata(int isfd, char *record, int increment)
{
        if (increment)  /* If write() calling get offset else its rewrite() */
                doffset = getspace(isfd, DATAFREELIST);
        lseek(ISDFD, doffset, 0);
        write(ISDFD, record, (unsigned)ftbl[isfd].dptr.datalength);
        markdata(isfd, doffset, ALIVE);
        UNIQUECOUNT += increment;
        DATACOUNT += increment;
}
