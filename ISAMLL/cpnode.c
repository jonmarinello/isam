#include "iisam.h"

/*      Copy a node in memory. Note - this is not just pointer manip.   */

void	cpnode(struct NODE *tnode, struct NODE *snode,
	struct keydesc *keydesc)
{
        int     i;

	memcpy((char *)&tnode->ndesc, (char *)&snode->ndesc, NDDSIZE);
        tnode->kptr = nkalloc((int)tnode->ndesc.keycnt);
        for (i = 0; i < tnode->ndesc.keycnt; ++i) {
                tnode->kptr[i] = ksalloc();
		memcpy((char *)tnode->kptr[i], (char *)snode->kptr[i], NDKSIZE);
		tnode->kptr[i]->keyval = salloc((int)keydesc->k_len);
		memcpy(tnode->kptr[i]->keyval, snode->kptr[i]->keyval,
			(int)keydesc->k_len);
        }
}
