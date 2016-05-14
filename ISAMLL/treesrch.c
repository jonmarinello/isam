#include "iisam.h"

/*      Search a given tree (pointed to by the passed key description)  */
/*      for an occurance of reckey.                                     */

int	treesearch(int isfd, struct keydesc *keydesc, char *reckey,
		struct NODE *worknode, int length)
{
        int     i;
        US_LONG unum;   /* Uniqueness number            */

        if (length) {   /* Generic Start Key Initialization */
                for (i = length; i < keydesc->k_len - UNIQUESIZE; ++i)
                        reckey[i] = 0;
                getzapunum(reckey, keydesc, UNIQUESTART, ZAP);
        }
        stackptr = (-1);
        push(keydesc->k_rootnode);
        loadnode(isfd, keydesc->k_rootnode, keydesc, worknode);
        for (;;) {
                if (!(keydesc->k_flags & ISDUPS)) { /* NODUPS */
                        if (sequential(keydesc, reckey, worknode,
                                keydesc->k_len - UNIQUESIZE) == FOUND)
                                        return(FOUND);
                }
                else if (sequential(keydesc, reckey, worknode, keydesc->k_len) == FOUND)
                        return(FOUND);
                if (child == NILL) {
                        unum = getzapunum(reckey, keydesc, DUMMYGET, GET);

                        /* iswrite() calling */

                        if (unum == (US_LONG)HIGHUSTART)
                                return(NOTFOUND);

                        /* isrewrite() or isrewcurr() calling */

                        if (unum != (US_LONG)UNIQUESTART)
                                return(NOTFOUND);

                        /* isdelete() or isdelcurr() calling */

                        if ((keydesc->k_flags & ISDUPS) && kindex > (-1)) {
                                if (!(kcmp(keydesc, worknode->kptr[kindex]->keyval,
                                        reckey,
                                        keydesc->k_len - UNIQUESIZE)))
                                                return(FOUND);
                        }

                        /* length specified c/o isstart() or UNIQUESTART */

                        if (travtree(isfd, MOVERIGHT, worknode, keydesc, reckey, keydesc->k_len - UNIQUESIZE)) {
                                iserrno = 0;
                                return(NOTFOUND);
                        }
                        if (!(kcmp(keydesc, worknode->kptr[kindex]->keyval,
                                reckey, keydesc->k_len - UNIQUESIZE)))
                                        return(FOUND);
                        return(NOTFOUND);
                }
                push(child);
                freenode(worknode);
                loadnode(isfd, fstack[stackptr], keydesc, worknode);
        }
}
