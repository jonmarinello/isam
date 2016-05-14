#include "iisam.h"

/*      Shift all remaining indexes down one notch.     */

void	shiftidx(int isfd, int dindex, int kcount)
{
        int     i;

        for (i = dindex; i < kcount; ++i) {
		memcpy((char *)&ftbl[isfd].descptr[i],
                        (char *)&ftbl[isfd].descptr[i + 1],
                        ftbl[isfd].descptr[i + 1].k_len);
        }
}
