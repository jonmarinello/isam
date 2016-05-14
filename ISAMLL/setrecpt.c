#include "iisam.h"

/*      Set/Reset the 'Current record pointer'. */

int	setrecptr(int isfd, struct keydesc *keydesc, int length,
		char *record, int mode)
{
        int     i;
        char    *newcurkey = NULL;      /* Newly built key              */
        char    *reckey = NULL;         /* Also points at built key     */
        struct  NODE    worknode;       /* node for treesearch          */
        struct  filetable       *file = &ftbl[isfd];    /* Efficiency   */

        switch (mode) {
                case ISFIRST :
                        for (i = 0; i < keydesc->k_len; ++i)
                                file->curkey[i] = HIGHVALUES;
                        loadnode(isfd, keydesc->k_rootnode, keydesc, &worknode);
                        if (worknode.ndesc.keycnt == 0) {
                                iserrno = EENDFILE;
                                break;  /* File empty */
                        }
                        while (worknode.ndesc.leftptr != NILL) {
                                freenode(&worknode);
                                loadnode(isfd, worknode.ndesc.leftptr,
                                        keydesc, &worknode);
                        }
                        kindex = 0;
                        newcurkey = worknode.kptr[0]->keyval;
                        break;
                case ISLAST :
                        for (i = 0; i < keydesc->k_len; ++i)
                                file->curkey[i] = HIGHVALUES;
                        loadnode(isfd, keydesc->k_rootnode, keydesc, &worknode);
                        if (worknode.ndesc.keycnt == 0) {
                                freenode(&worknode);
                                iserrno = EENDFILE;
                                return(-1);
                        }
                        while (worknode.kptr[worknode.ndesc.keycnt - 1]->nodenext != NILL) {
                                freenode(&worknode);
                                loadnode(isfd, worknode.kptr[worknode.ndesc.keycnt - 1]->nodenext,
                                        keydesc, &worknode);
                        }
                        kindex = worknode.ndesc.keycnt - 1;
                        newcurkey = worknode.kptr[worknode.ndesc.keycnt - 1]->keyval;
                        break;
                case ISEQUAL :
                        newcurkey = reckey = bkey(record, keydesc);
                        treesearch(isfd, keydesc, reckey, &worknode, length);
                        if (kindex == worknode.ndesc.keycnt ||
                                !worknode.ndesc.keycnt ||
                                        kindex < 0) {
                                        iserrno = ENOREC;
                                        break;
                        }
                        if (kcmp(keydesc, worknode.kptr[kindex]->keyval,
                                reckey, (length ? length :
                                keydesc->k_len - UNIQUESIZE))) {
                                        iserrno = ENOREC;
                                        break;
                        }
                        newcurkey = worknode.kptr[kindex]->keyval;
                        break;
                case ISCURR :
                        if (nocurr(isfd)) {
                                iserrno = ENOCURR;
                                return(-1);
                        }
                        if (treesearch(isfd, keydesc, file->curkey,
                                &worknode, WHOLEKEY) == NOTFOUND) {
                                        iserrno = ENOREC;
                                        break;
                        }
                        newcurkey = worknode.kptr[kindex]->keyval;
                        break;
                case ISNEXT :
                        treesearch(isfd, keydesc, file->curkey, &worknode, WHOLEKEY);
                        if (travtree(isfd, MOVERIGHT, &worknode,
                                keydesc, file->curkey, WHOLEKEY)) {
                                        iserrno = EENDFILE;
                                        break;
                        }
                        newcurkey = worknode.kptr[kindex]->keyval;
                        break;
                case ISPREV :
                        if (!(treesearch(isfd, keydesc, file->curkey,
                                &worknode, WHOLEKEY)))
                                        ++kindex;
                        if (kindex == 0)
                                child = worknode.ndesc.leftptr;
                        else
                                child = worknode.kptr[kindex - 1]->nodenext;
                        if (travtree(isfd, MOVELEFT, &worknode,
                                keydesc, file->curkey, WHOLEKEY)) {
                                        iserrno = EENDFILE;
                                        break;
                        }
                        newcurkey = worknode.kptr[kindex]->keyval;
                        break;
                case ISGREAT :
                        newcurkey = reckey = bkey(record, keydesc);
                        treesearch(isfd, keydesc, reckey, &worknode, length);
                        do {
                                if (kindex == worknode.ndesc.keycnt ||
                                        !worknode.ndesc.keycnt ||
                                        kindex < 0) {
                                                iserrno = ENOREC;
                                                break;
                                }
                                if ((kcmp(keydesc, worknode.kptr[kindex]->keyval, reckey,
                                        (length ? length : keydesc->k_len - UNIQUESIZE))) == LESS)
                                                break;
                        } while (!travtree(isfd, MOVERIGHT, &worknode,
                                keydesc, reckey, (length ? length : keydesc->k_len - UNIQUESIZE)));
                        if (iserrno)
                                break;
                        newcurkey = worknode.kptr[kindex]->keyval;
                        break;
                case ISGTEQ :
                        newcurkey = reckey = bkey(record, keydesc);
                        if ((treesearch(isfd, keydesc, reckey, &worknode, length)) == NOTFOUND)
                                if (kindex == worknode.ndesc.keycnt ||
                                        !worknode.ndesc.keycnt ||
                                        kindex < 0) {
                                                iserrno = ENOREC;
                                                break;
                                }
                        newcurkey = worknode.kptr[kindex]->keyval;
                        break;
                default :
                        iserrno = EBADARG;
                        return(-1);
        }
        if (!iserrno) {
                doffset = worknode.kptr[kindex]->dataptr;
		memcpy(file->curkey, newcurkey, keydesc->k_len);
                if (reckey != NULL)
                        free(reckey);
                freenode(&worknode);
                return(0);
        }
        freenode(&worknode);
        if (reckey != NULL)
                free(reckey);
        return(-1);
}
