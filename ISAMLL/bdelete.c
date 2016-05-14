#include "iisam.h"

/*      Delete the key indexed by kindex from a given tree      */

void	bdelete(int isfd, struct keydesc *keydesc, struct NODE *worknode)
{
        struct  NODE    successor;      /* Successor node (maybe)       */
        struct  nodekey *workkey;       /* Starting workkey             */
        struct  nodekey *succkey;       /* Successor key (maybe)        */
        int     wsneq = 1;              /* Work/succ have same keyval   */
        char    *tmpkey;                /* Malloc'd keyval for free()   */
        char    pathkey[INTERNALKEYSZ]; /* Key to refind path to node   */
        int     saveptr;                /* Save disk offset             */
        int     tmpptr;                 /* Temporary disk offset place  */

        workkey = worknode->kptr[kindex];
        if (workkey->nodenext == NILL) {
                underflow(isfd, keydesc, worknode);
                return;
        }
        saveptr = stackptr; /* Save stack pointer */
        loadnode(isfd, fstack[stackptr], keydesc, &successor);
        travtree(isfd, MOVERIGHT, &successor, keydesc,
                workkey->keyval, WHOLEKEY);
        succkey = successor.kptr[0];
        workkey->dataptr = succkey->dataptr;
        if (keydesc->k_flags & ISDUPS)
                wsneq = kcmp(keydesc, workkey->keyval, succkey->keyval,
                        keydesc->k_len - UNIQUESIZE);
        if (wsneq) {
                worknode->ndesc.nodeln -= complen(keydesc);
                tmpkey = salloc((int)keydesc->k_len);
		memcpy(tmpkey, succkey->keyval, keydesc->k_len);
                free((char *)workkey->keyval);
                workkey->keyval = tmpkey;
		memcpy(pathkey, succkey->keyval, keydesc->k_len);
                tmpptr = stackptr;
                stackptr = saveptr;
                worknode->ndesc.nodeln += complen(keydesc);
                actsplit = 0;
                for (;;) {
                        rstnl(keydesc, worknode);
                        if (worknode->ndesc.nodeln > PAGESIZE) {
				++actsplit;
				workkey = splitnode(isfd, worknode, keydesc);
				if (workkey) {
					loadnode(isfd, pop(), keydesc, worknode);
					sequential(keydesc, workkey->keyval, worknode, WHOLEKEY);
					kinsert(worknode, workkey, complen(keydesc));
                                }
                                /* 10/18/85 Joma - Added break statement */
                                else
                                        break;
                                /* 10/18/85 Joma - End of modification   */
                        }
                        else {
                                dumpnode(isfd, fstack[stackptr],
                                        keydesc, worknode);
                                break;
                        }
                }
                if (actsplit) {
                        treesearch(isfd, keydesc, pathkey, worknode, WHOLEKEY);
                        travtree(isfd, MOVERIGHT, worknode, keydesc,
                                workkey->keyval, WHOLEKEY);
                        freenode(&successor);
                }
                else {
                        worknode = &successor;
                        stackptr = tmpptr;
                }
        }
        else { /* keys are equal (ISDUPS) so no insertion required */
                getzapunum(workkey->keyval, keydesc, 
                        getzapunum(succkey->keyval, keydesc, DUMMYGET, GET), ZAP);
                dumpnode(isfd, fstack[saveptr], keydesc, worknode);
                worknode = &successor;
        }
        kindex = 0;
        underflow(isfd, keydesc, worknode);
}
