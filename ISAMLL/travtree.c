#include "iisam.h"

/*      Traverse a given index tree either left or right.       */

int	travtree(int isfd, int direction, struct NODE *worknode,
		struct keydesc *keydesc, char *reckey, int length)
{
        if (!worknode->ndesc.keycnt) {
                iserrno = ENOREC;
                return(-1);
        }
        if (direction == MOVERIGHT) {
                if (child != NILL) {
                        do {
                                freenode(worknode);
                                push(child);
                                loadnode(isfd, child, keydesc, worknode);
                                child = worknode->ndesc.leftptr;
                        } while (child != NILL);
                        kindex = 0;
                        return(0);
                }
                if (++kindex < worknode->ndesc.keycnt)
                        return(0);
                if (stackptr) {
                        do {
                                freenode(worknode);
                                loadnode(isfd, pop(), keydesc, worknode);
/* Mon Jan 13 15:39:10 PAR 1986 - Joma added check for FOUND case ISDUPS */
                                if (sequential(keydesc, reckey, worknode, length) == FOUND && (keydesc->k_flags & ISDUPS)) {
                                        child = worknode->kptr[kindex]->nodenext;
                                        return(0);
                                }
                                if (++kindex < worknode->ndesc.keycnt) {
                                        child = worknode->kptr[kindex]->nodenext;
                                        return(0);
                                }
                        } while (stackptr);
                        iserrno = ENOREC;
                        return(-1);
                }
        }
        else {  /* MOVELEFT */
                if (child != NILL) {
                        do {
                                freenode(worknode);
                                push(child);
                                loadnode(isfd, child, keydesc, worknode);
                                child = worknode->kptr[worknode->ndesc.keycnt - 1]->nodenext;
                        } while (child != NILL);
                        kindex = worknode->ndesc.keycnt - 1;
                        return(0);
                }
                if (--kindex > -1)
                        return(0);
                if (stackptr) {
                        do {
                                freenode(worknode);
                                loadnode(isfd, pop(), keydesc, worknode);
                                sequential(keydesc, reckey, worknode, length);
                                if (kindex > -1) {
                                        child = worknode->kptr[kindex]->nodenext;
                                        return(0);
                                }
                        } while (stackptr);
                        iserrno = ENOREC;
                        return(-1);
                }
        }
        iserrno = ENOREC;
        return(-1);
}
