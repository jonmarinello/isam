#include "iisam.h"

/*      Insert or delete a key to/from an index B-Tree. */

int	modtree(int isfd, char *passkey, struct keydesc *keydesc, int mode)
{
        struct  NODE    worknode;       /* node for treesearch     */
        struct  nodekey *workkey;       /* target key for either*/
                                        /* INSERT or DELETE     */

        switch (mode) {
                case INSERT :
                        if ((treesearch(isfd, keydesc, passkey, &worknode,
                                        WHOLEKEY) == FOUND) &&
                                !(keydesc->k_flags & ISDUPS)) {
                                        freenode(&worknode);    
                                        free(passkey);
                                        iserrno = EDUPL;
                                        return(-1);
                        }
                        workkey = ksalloc();
                        workkey->ldbytes = workkey->tlblanks = 0;
                        workkey->nodenext = NILL;
                        workkey->dataptr = doffset;
                        workkey->keyval = passkey;

                        /* Note - 'passkey' is automatically free'd in  */
                        /*      the insertion process.                  */

                        getzapunum(workkey->keyval, keydesc, UNIQUECOUNT, ZAP);
                        binsert(isfd, keydesc, &worknode, workkey);
                        break;
                case DELETE :
                        if (treesearch(isfd, keydesc, passkey,
                                &worknode, WHOLEKEY) == NOTFOUND) {
                                        freenode(&worknode);    
                                        free(passkey);
                                        iserrno = ENOREC;
                                        return(-1);
                        }
                        doffset = worknode.kptr[kindex]->dataptr;
                        bdelete(isfd, keydesc, &worknode);
                        free(passkey);
                        break;
        }
        return(0);
}
