#include "iisam.h"

/*      Split 'worknode' into to new nodes and return an overflow key.  */

struct  nodekey *split(struct NODE *worknode, struct keydesc *keydesc,
		struct NODE *lnode, struct NODE *rnode)
{
        struct  nodekey *overflow;      /* Pointer to overflow key      */
        struct  NODE    *curnode;       /* Points at left or right node */
	unsigned        i = 0;
        long    maxln = PAGESIZE / 2L;

        lnode->kptr = nkalloc(1);
        rnode->kptr = nkalloc(1);
        lnode->ndesc.nodeln = rnode->ndesc.nodeln = NDDSIZE;
        lnode->ndesc.keycnt = rnode->ndesc.keycnt = 0;
        lnode->ndesc.leftptr = worknode->ndesc.leftptr;
        curnode = lnode;
        for (;;) {
                curnode->ndesc.nodeln += complen(keydesc);
                rstnl(keydesc, curnode);
                if (curnode->ndesc.nodeln > maxln) {
                        overflow = worknode->kptr[i++];
                        curnode = rnode;
                        maxln = PAGESIZE;
                        continue;
                }
                curnode->kptr[curnode->ndesc.keycnt++] = worknode->kptr[i++];
                if (i >= worknode->ndesc.keycnt)
                        break;  
                curnode->kptr = nkrealloc(curnode->kptr,
                        (int)curnode->ndesc.keycnt + 1);
        }
        rnode->ndesc.leftptr = overflow->nodenext;
        return(overflow);
}
