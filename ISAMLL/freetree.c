#include "iisam.h"

/*      Traverse B-Tree and add all nodes to the index free list        */

void	freetree(int isfd, struct keydesc *keydesc)
{
        int     i;
        char    *reckey = ftbl[isfd].curkey;
        struct  NODE    worknode;

        for (i = 0; i < keydesc->k_len; ++i)
                reckey[i] = LOWVALUES;
        treesearch(isfd, keydesc, reckey, &worknode, WHOLEKEY);
        for (;;) {
                if (worknode.ndesc.leftptr == NILL) {   /* Leaf level */
                        kindex = worknode.ndesc.keycnt - 1;
                        if (travtree(isfd, MOVERIGHT, &worknode, keydesc,
                                reckey, WHOLEKEY)) {
                                        freespace(isfd, keydesc->k_rootnode,
                                                INDEXFREELIST);
                                        iserrno = 0;
                                        return; /* End of file */
                        }
			memcpy(reckey, worknode.kptr[kindex]->keyval,
                                keydesc->k_len);
                        continue;
                }
                if (!kindex)
                        freespace(isfd, worknode.ndesc.leftptr, INDEXFREELIST);
                freespace(isfd, worknode.kptr[kindex]->nodenext,
                        INDEXFREELIST);
                travtree(isfd, MOVERIGHT, &worknode, keydesc, reckey, WHOLEKEY);
		memcpy(reckey, worknode.kptr[kindex]->keyval,
                        keydesc->k_len);
        }
}
