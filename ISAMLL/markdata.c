#include "iisam.h"

/*      Flag a data record as either dead or alive. (Shoot 'em up!)     */

void	markdata(int isfd, long daddr, char deadoralive)
{
        lseek(ISDFD, (long)(daddr + ftbl[isfd].dptr.datalength), 0);
        write(ISDFD, &deadoralive, 1);
        if (deadoralive == DEAD) {
                --DATACOUNT;
                freespace(isfd, daddr, DATAFREELIST);
        }
}
