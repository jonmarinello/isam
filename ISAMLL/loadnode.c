#include "iisam.h"

static	void	getndata(char *, unsigned);

/*      Load a node from disk into 'worknode'.  */

void	loadnode(int isfd, long offset, struct keydesc *keydesc,
		struct NODE *worknode)
{
	int     tlstart;
        int     k;
        int     i;
        int     j;
	short   btr;            /* Actual number of bytes to read       */
        struct  nodekey *pkp;   /* Optimize indirection                 */
        struct  nodekey *tkp;   /* Optimize indirection                 */
	char    wkey[INTERNALKEYSZ]; /* Work key for decompression      */

        iobptr = 0;
        ftbl[isfd].offset = offset;
        lseek(ISIFD, offset, 0);
        read(ISIFD, iob, (unsigned)PAGESIZE);
        getndata((char *)&worknode->ndesc, NDDSIZE);
        worknode->kptr = nkalloc((int)worknode->ndesc.keycnt);
        worknode->ndesc.nodeln = NDDSIZE;
        if (!worknode->ndesc.keycnt)
                return; /* Empty Tree */
        worknode->kptr[0] = ksalloc();
        pkp = worknode->kptr[0];
        getndata((char *)pkp, (unsigned)(NDKSIZE - sizeof(char *)));
	pkp->keyval = salloc((int)keydesc->k_len);
        if ((keydesc->k_flags & COMPRESS)) {
		btr = pkp->ldbytes >> 8;
                pkp->ldbytes = pkp->ldbytes & 0x00FF;
		/* Read compressed keyval */
		getndata(wkey, (unsigned)btr);
                /* Read duplicate key unique id */
                getndata((char *)&wkey[keydesc->k_len - UNIQUESIZE],
                        (unsigned)UNIQUESIZE);
		for (i = 0; i < btr; ++i)
                        pkp->keyval[i] = wkey[i];
                padkey(pkp->keyval, i, keydesc->k_len - UNIQUESIZE);
                for (i = keydesc->k_len - UNIQUESIZE; i < keydesc->k_len; ++i)
                        pkp->keyval[i] = wkey[i];
        }
        else {
                getndata(pkp->keyval, (unsigned)keydesc->k_len);
        }
        pkp->ldbytes = pkp->tlblanks = 0;
        for (i = 1; i < worknode->ndesc.keycnt; ++i) {
                worknode->kptr[i] = ksalloc();
                tkp = worknode->kptr[i];
                getndata((char *)tkp, (unsigned)(NDKSIZE - sizeof(char *)));
                tkp->keyval = salloc((int)keydesc->k_len);
                if ((keydesc->k_flags & COMPRESS)) {
			btr = tkp->ldbytes >> 8;
                        tkp->ldbytes = tkp->ldbytes & 0x00FF;
			/* Read compressed keyval */
			getndata(wkey, (unsigned)btr);
                        /* Read duplicate key unique id */
                        getndata((char *)&wkey[keydesc->k_len - UNIQUESIZE],
                                (unsigned)UNIQUESIZE);
                        for (j = 0; j < tkp->ldbytes; ++j)
                                tkp->keyval[j] = pkp->keyval[j];
                        k = 0;
			for (j = tkp->ldbytes; j < tkp->ldbytes + btr; ++j)
                                tkp->keyval[j] = wkey[k++];
                        tlstart = j;
                        j = keydesc->k_len - UNIQUESIZE - 1;
                        while (j && pkp->keyval[j] == ' ')
                                --j;
                        j -= (tkp->tlblanks - 1);
                        for (k = 0; k < tkp->tlblanks; ++k)
                                tkp->keyval[tlstart++] = pkp->keyval[j++];
			padkey(tkp->keyval, tkp->ldbytes + tkp->tlblanks + btr,
                                keydesc->k_len - UNIQUESIZE);
                        for (k = keydesc->k_len - UNIQUESIZE; k < keydesc->k_len; ++k)
                                tkp->keyval[k] = wkey[k];
                        pkp = tkp;
                }
                else {
                        getndata(tkp->keyval, (unsigned)keydesc->k_len);
                }
                tkp->ldbytes = tkp->tlblanks = 0;
        }
        worknode->ndesc.nodeln = NDDSIZE +
                (worknode->ndesc.keycnt * complen(keydesc));
}

static	void	getndata(char *buf, unsigned nchars)
{
        int     i = 0;

        while (nchars) {
                buf[i++] = iob[iobptr++];
                --nchars;
        }
}
