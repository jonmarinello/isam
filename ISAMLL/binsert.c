#include "iisam.h"

/*      Insert workkey into the given tree      */

void	binsert(int isfd, struct keydesc *keydesc, struct NODE *worknode,
		struct nodekey *workkey)
{
        actsplit = 0;           /* Reset split switch           */
        while (workkey) {
                kinsert(worknode, workkey, complen(keydesc));
                rstnl(keydesc, worknode);
                if (worknode->ndesc.nodeln > PAGESIZE) {
                        ++actsplit;     /* split occured (path changed) */
			workkey = splitnode(isfd, worknode, keydesc);
			if (workkey) {
                                loadnode(isfd, pop(), keydesc, worknode);
                                sequential(keydesc, workkey->keyval,
                                        worknode, WHOLEKEY);
                        }
                }
                else {
                        dumpnode(isfd, ftbl[isfd].offset, keydesc, worknode);
                        return;
                }
        }
}
