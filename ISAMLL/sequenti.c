#include "iisam.h"

/*      Sequentialy search 'worknode' for an occurance of 'newkey'.     */

int	sequential(struct keydesc *keydesc, char *newkey,
		struct NODE *worknode, int length)
{
        int     cond;                   /* cond = 0 : EQUAL     */
                                        /* cond > 0 : GREATER   */
                                        /* cond < 0 : LESS      */
        struct  nodekey *workkey;       /* Efficiency           */
        int     signif;                 /* Normalized length    */

        if (worknode->ndesc.keycnt == 0) { /* EMPTY TREE */
                kindex = -1;
                child = NILL;
                return(NOTFOUND);
        }
        if (length)
                signif = length;
        else
                signif = keydesc->k_len;
        for (kindex = 0; kindex < worknode->ndesc.keycnt; ++kindex) {
                workkey = worknode->kptr[kindex];
                if (!(cond = kcmp(keydesc, workkey->keyval, newkey, signif))) {
                        child = workkey->nodenext;
                        return(FOUND);
                }
                else if (cond == LESS) {
                        if (--kindex < 0)
                                child = worknode->ndesc.leftptr;
                        else
                                child = worknode->kptr[kindex]->nodenext;
                        return(NOTFOUND);
                }
        }
        child = worknode->kptr[--kindex]->nodenext;
        return(NOTFOUND);
}
