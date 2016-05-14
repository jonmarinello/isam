#include "iisam.h"

/*      Delete the key indexed by kindex from 'worknode'. This key is   */
/*      then returned to the calling function.                          */

struct  nodekey *kdelete(struct NODE *worknode, int actln)
{
        int     i;
        struct  nodekey *extrct = worknode->kptr[kindex];

        for (i = kindex; i < worknode->ndesc.keycnt - 1; ++i)
                worknode->kptr[i] = worknode->kptr[i + 1];
        --worknode->ndesc.keycnt;
        worknode->ndesc.nodeln -= actln;
        return(extrct);
}
