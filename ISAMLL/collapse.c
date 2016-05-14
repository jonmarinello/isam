#include "iisam.h"

/*      Collapse two nodes together and if the result is larger than    */
/*      the size of a page, split it and push up an overflow key.       */
/*      If the result fits, return it through collnode.                 */
/*      If not, collnode is free'd and results are returned through     */
/*      rightnode and leftnode.                                         */

struct  nodekey *collapse(struct NODE *collnode,
	struct keydesc *keydesc, struct nodekey *inskey,
	struct NODE *leftnode, struct NODE *rightnode, unsigned swapsw,
	int actln)
{
        int     cindex;         /* Collapse index for collapsed r & l   */
        int     windex;         /* Collapse index for worknode          */
        struct  NODE    *swapnode;      /* Temporary swap node pointer  */
        struct  nodekey *retkey;        /* Return key for correct free  */

        if (swapsw) {
                swapnode = rightnode;
                rightnode = leftnode;
                leftnode = swapnode;
        }
        inskey->nodenext = rightnode->ndesc.leftptr;
        collnode->ndesc.leftptr = leftnode->ndesc.leftptr;
        collnode->ndesc.keycnt = leftnode->ndesc.keycnt +
                rightnode->ndesc.keycnt + 1;
        collnode->ndesc.nodeln = (leftnode->ndesc.nodeln +
                rightnode->ndesc.nodeln + actln) - NDDSIZE;
        collnode->kptr = nkalloc((int)collnode->ndesc.keycnt);
        windex = 0;
        for (cindex = 0; cindex < leftnode->ndesc.keycnt; ++cindex)
                collnode->kptr[windex++] = leftnode->kptr[cindex];
        collnode->kptr[windex++] = inskey;
        for (cindex = 0; cindex < rightnode->ndesc.keycnt; ++cindex)
                collnode->kptr[windex++] = rightnode->kptr[cindex];
        free((char *)rightnode->kptr);
        free((char *)leftnode->kptr);
        /* Result fits so return in collnode collapsed node             */
        rstnl(keydesc, collnode);
        if (collnode->ndesc.nodeln <= PAGESIZE)
                return((struct nodekey *)NULL);
        /* 10/18/85 Joma - Seperated strait return() call into          */
        /*              following code to allow free() call for memory  */
        retkey = split(collnode, keydesc, leftnode, rightnode);
        free((char *)collnode->kptr);
        /* Result was split so leftnode and rightnode contain result.   */
        return(retkey);
        /* 10/18/85 Joma - End of modification                          */
}
