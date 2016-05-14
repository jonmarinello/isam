#include "iisam.h"

/*      Handle disk addressing of new left and right nodes (as well as  */
/*      the root node maybe) after a call to split().                   */

struct  nodekey *splitnode(int isfd, struct NODE *worknode,
		struct keydesc *keydesc)
{
        struct  nodekey *overflow;      /* Pointer to overflow key      */
        struct  NODE    leftnode;       /* Guess you can figure these   */
        struct  NODE    rightnode;      /* two out...                   */
        struct  NODE    savenode;
        int     i;
        long    saddr;                  /* save address                 */
        long    offset;                 /* used for dumping nodes       */
        long    curraddr;               /* worknode's address           */
        long    fathaddr;               /* father's address             */
        long    brotaddr;               /* brother's address            */
        struct  NODE    *fnode;         /* Father node                  */
        struct  NODE    *bnode;         /* Brother node                 */

        curraddr = fstack[stackptr];
        if (!stackptr)
                goto NORMS;

        /* Apply special key rotation algorithm if possible. See Knuth  */
        /* "Sorting and Searching" pages 477, 478. The B*-tree.         */
        /* The superior breed of tree!                                  */
        /* Worknode is full so check for kindex = 0 and kindex = keycnt */
        /* in the father node as these are special cases of having only */
        /* one available brother.                                       */

        cpnode(&savenode, worknode, keydesc);
        fnode = &leftnode;
        bnode = &rightnode;
        curraddr = fstack[stackptr];
        fathaddr = fstack[stackptr - 1];
        loadnode(isfd, fathaddr, keydesc, fnode);
        if (curraddr == fnode->ndesc.leftptr) {

                /* Only right hand rotation possible */

                brotaddr = fnode->kptr[0]->nodenext;
                loadnode(isfd, brotaddr, keydesc, bnode);
                kindex = 0;
                overflow = kdelete(fnode, complen(keydesc));
                saddr = overflow->nodenext;
                overflow->nodenext = bnode->ndesc.leftptr;
                kindex = -1;
                kinsert(bnode, overflow, complen(keydesc));
                bnode->ndesc.leftptr =
                        worknode->kptr[kindex = worknode->ndesc.keycnt - 1]->nodenext;
                overflow = kdelete(worknode, complen(keydesc));
                overflow->nodenext = saddr;
                kindex = -1;
                kinsert(fnode, overflow, complen(keydesc));
        }
        else if (curraddr == fnode->kptr[fnode->ndesc.keycnt - 1]->nodenext) {

                /* Only hand left rotation possible */

                if (fnode->ndesc.keycnt > 1)
                        brotaddr = fnode->kptr[fnode->ndesc.keycnt - 2]->nodenext;
                else
                        brotaddr = fnode->ndesc.leftptr;
                loadnode(isfd, brotaddr, keydesc, bnode);
                kindex = fnode->ndesc.keycnt - 1;
                overflow = kdelete(fnode, complen(keydesc));
                saddr = overflow->nodenext;
                overflow->nodenext = worknode->ndesc.leftptr;
                kindex = bnode->ndesc.keycnt - 1;
                kinsert(bnode, overflow, complen(keydesc));
                kindex = 0;
                overflow = kdelete(worknode, complen(keydesc));
                worknode->ndesc.leftptr = overflow->nodenext;
                overflow->nodenext = saddr;
                kindex = fnode->ndesc.keycnt - 1;
                kinsert(fnode, overflow, complen(keydesc));
        }
        else {
                /* Right or left hand rotation possible */

                for (i = 0; i < fnode->ndesc.keycnt - 1; ++i) {
                        if (curraddr == fnode->kptr[i]->nodenext)
                                break;  /* Find father key              */
                }
                /* Try left rotation */
                if (i > 0)
                        brotaddr = fnode->kptr[i - 1]->nodenext;
                else
                        brotaddr = fnode->ndesc.leftptr;
                loadnode(isfd, brotaddr, keydesc, bnode);
                kindex = i;
                overflow = kdelete(fnode, complen(keydesc));
                saddr = overflow->nodenext;
                overflow->nodenext = worknode->ndesc.leftptr;
                kindex = bnode->ndesc.keycnt - 1;
                kinsert(bnode, overflow, complen(keydesc));
                kindex = 0;
                overflow = kdelete(worknode, complen(keydesc));
                worknode->ndesc.leftptr = overflow->nodenext;
                overflow->nodenext = saddr;
                kindex = i - 1;
                kinsert(fnode, overflow, complen(keydesc));
                rstnl(keydesc, worknode);
                rstnl(keydesc, fnode);
                rstnl(keydesc, bnode);
                if (worknode->ndesc.nodeln <= PAGESIZE &&
                        fnode->ndesc.nodeln <= PAGESIZE &&
                        bnode->ndesc.nodeln <= PAGESIZE) {
                                goto ROK; /* Left rotation ok */
                }
                freenode(worknode);
                freenode(fnode);
                freenode(bnode);
                cpnode(worknode, &savenode, keydesc);
                loadnode(isfd, fathaddr, keydesc, fnode);
                /* Try right rotation */
                brotaddr = fnode->kptr[++i]->nodenext;
                loadnode(isfd, brotaddr, keydesc, bnode);
                kindex = i;
                overflow = kdelete(fnode, complen(keydesc));
                saddr = overflow->nodenext;
                overflow->nodenext = bnode->ndesc.leftptr;
                kindex = -1;
                kinsert(bnode, overflow, complen(keydesc));
                bnode->ndesc.leftptr =
                        worknode->kptr[kindex = worknode->ndesc.keycnt - 1]->nodenext;
                overflow = kdelete(worknode, complen(keydesc));
                overflow->nodenext = saddr;
                kindex = i - 1;
                kinsert(fnode, overflow, complen(keydesc));
        }
        rstnl(keydesc, worknode);
        rstnl(keydesc, fnode);
        rstnl(keydesc, bnode);
        if (worknode->ndesc.nodeln > PAGESIZE ||
                fnode->ndesc.nodeln > PAGESIZE ||
                bnode->ndesc.nodeln > PAGESIZE) {

                /* Rotation failed, throw away these nodes and restore  */
                /* worknode to previous state to perform normal split   */

                        freenode(worknode);
                        freenode(fnode);
                        freenode(bnode);
                        worknode = &savenode;
                        goto NORMS;
        }
ROK:    dumpnode(isfd, fathaddr, keydesc, fnode);
        dumpnode(isfd, brotaddr, keydesc, bnode);
        dumpnode(isfd, curraddr, keydesc, worknode);
        actsplit = 0;           /* Rotatation successful        */
        freenode(&savenode);
        return((struct nodekey *)NULL);
NORMS:  overflow = split(worknode, keydesc, &leftnode, &rightnode);
        if (stackptr) {         /* NOT ROOT NODE */
                dumpnode(isfd, curraddr, keydesc, &leftnode);
                offset = getspace(isfd, INDEXFREELIST);
                dumpnode(isfd, offset, keydesc, &rightnode);
                overflow->nodenext = offset;
                free((char *)worknode->kptr);
                return(overflow);
        }                       /* ROOT NODE */
        offset = getspace(isfd, INDEXFREELIST);
        dumpnode(isfd, offset, keydesc, &leftnode);
        worknode->ndesc.leftptr = offset;
        offset = getspace(isfd, INDEXFREELIST);
        dumpnode(isfd, offset, keydesc, &rightnode);
        /* 10/18/85 Joma - Added code to free() memory properly */
        free((char *)worknode->kptr);
        worknode->kptr = nkalloc(1);
        /* 10/18/85 Joma - End of modification */
        worknode->ndesc.keycnt = 1;
        worknode->kptr[0] = overflow;
        worknode->kptr[0]->nodenext = offset;
        /* Nodelength set in dumpnode() */
        dumpnode(isfd, keydesc->k_rootnode, keydesc, worknode);
        return((struct nodekey *)NULL);
}
