#include "iisam.h"

/*      Insert 'workkey' into 'worknode'.       */

void	kinsert(struct NODE *worknode, struct nodekey *workkey, int actln)
{
        int     i;

        worknode->kptr = nkrealloc(worknode->kptr,
                (int)(++worknode->ndesc.keycnt));
        ++kindex;
        for (i = worknode->ndesc.keycnt - 1; i > kindex; --i)
                worknode->kptr[i] = worknode->kptr[i - 1];
        worknode->kptr[kindex] = workkey;
        worknode->ndesc.nodeln += actln;
}
