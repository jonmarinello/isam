#include "iisam.h"

/*      Begin combining brothers at the leaf level and handle           */
/*      multilevel father underflow which in turn may cause another     */
/*      brother combination.                                            */

void	underflow(int isfd, struct keydesc *keydesc, struct NODE *worknode)
{
        struct  NODE    collnode;       /* Pointer to collapsed node    */
        struct  NODE    brother;        /* Pointer to brother node      */
        struct  NODE    father;         /* Pointer to father node       */
        struct  nodekey *workkey = worknode->kptr[kindex];
        /* 10/18/85 Joma - Added extract nodekey pointer for free()     */
        struct  nodekey *extrct;        /* Pointer to key for free()    */
        /* 10/18/85 Joma - End of modification                          */
        struct  nodekey *inskey;        /* Pointer to pulled down key   */
                                        /* from father node             */
        long    brothaddr;              /* Brother's address            */
        long    curraddr;               /* Address of current node      */
        long    fathaddr;               /* Address of father node       */
        long    colladdr;               /* Address of collapsed node    */
        long    rightaddr;              /* Address of righthand node    */
        int     actln;                  /* Actual length of keys moved  */
        unsigned        swapsw;         /* See collapse()               */
        unsigned        rootempty;      /* Empty tree switch            */
        char    pathkey[INTERNALKEYSZ]; /* Saved return path key        */

        /* 10/18/85 Joma - End of modification                          */
        extrct = kdelete(worknode, complen(keydesc));
        /* 10/18/85 Joma - End of modification                          */
        rstnl(keydesc, worknode);
        if (((worknode->ndesc.nodeln) >= PAGESIZE / 2L) || !stackptr) {
        /* 10/18/85 Joma - Added code to free extracted key             */
                free(extrct->keyval);
                free((char *)extrct);
        /* 10/18/85 Joma - End of modification                          */
                dumpnode(isfd, fstack[stackptr], keydesc, worknode);
                return;
        }
        for (;;) {
                swapsw = 1;
                rightaddr = colladdr = curraddr = fstack[stackptr];
                rootempty = 0;
                fathaddr = pop();
                loadnode(isfd, fathaddr, keydesc, &father);
                sequential(keydesc, workkey->keyval, &father, WHOLEKEY);
        /* 10/18/85 Joma - Added code to free extracted key             */
                if (extrct) {
                        free(extrct->keyval);
                        free((char *)extrct);
                        extrct = (struct nodekey *)NULL;
                }
        /* 10/18/85 Joma - End of modification                          */
                if (kindex < 0) {
                        --swapsw;
                        rightaddr = brothaddr = father.kptr[++kindex]->nodenext;
                }
                else if (kindex == 0)
                        colladdr = brothaddr = father.ndesc.leftptr;
                else
                        colladdr = brothaddr = father.kptr[kindex - 1]->nodenext;
                inskey = kdelete(&father, actln = complen(keydesc));
                if (!father.ndesc.keycnt)
                        ++rootempty;
                dumpnode(isfd, fathaddr, keydesc, &father);
                loadnode(isfd, brothaddr, keydesc, &brother);
		workkey = collapse(&collnode, keydesc, inskey, worknode, &brother,
			swapsw, actln);
		if (workkey) {
			dumpnode(isfd, curraddr, keydesc, worknode);
			dumpnode(isfd, brothaddr, keydesc, &brother);
			loadnode(isfd, fathaddr, keydesc, &father);
			memcpy(pathkey, workkey->keyval, keydesc->k_len);
			workkey->nodenext = rightaddr;
			sequential(keydesc, workkey->keyval, &father, WHOLEKEY);
			binsert(isfd, keydesc, &father, workkey);
			if (actsplit)
				treesearch(isfd, keydesc, pathkey, worknode, WHOLEKEY);
			else
				loadnode(isfd, fathaddr, keydesc, worknode);
                }
                else {  /* no overflow key */
                        if (rootempty) {        /* replace rootnode */
                                dumpnode(isfd, keydesc->k_rootnode, keydesc, &collnode);
                                freespace(isfd, curraddr, INDEXFREELIST);
                                freespace(isfd, brothaddr, INDEXFREELIST);
                                return;
                        }
                        /* dump worknode at leftmost address */
                        freespace(isfd, rightaddr, INDEXFREELIST);
                        dumpnode(isfd, colladdr, keydesc, &collnode);
                        loadnode(isfd, fathaddr, keydesc, worknode);
                }
                /* 10/18/85 Joma - Added freenode() call for memory */
                rstnl(keydesc, worknode);
                if (((worknode->ndesc.nodeln) >= PAGESIZE / 2L) || !stackptr) {
                        freenode(worknode);
                        return;
                }
                /* 10/18/85 Joma - End of modification */
                workkey = worknode->kptr[0];
        }
}
