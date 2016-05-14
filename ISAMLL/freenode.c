#include "iisam.h"

/*      Free the memory currently occupied by the passed worknode.      */

void	freenode(struct NODE *worknode)
{
        int     i;

        for (i = 0; i < worknode->ndesc.keycnt; ++i) {
                free((char *)worknode->kptr[i]->keyval);
                free((char *)worknode->kptr[i]);
        }
        free((char *)worknode->kptr);
}
