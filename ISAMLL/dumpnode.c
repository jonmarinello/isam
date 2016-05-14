#include "iisam.h"

/*      Dump a node to disk     */

void	dumpnode(int isfd, long offset, struct keydesc *keydesc,
			struct NODE *worknode)
{
        int     i;
        int     j;
        short   ldsave;
        struct  nodekey *tkp;   /* Optimize indirection                 */
        int     bcount;         /* Space character count                */
        int     compress;       /* Length of keyval after compression   */
        long    start;          /* May not need to write PAGESIZE if    */
                                /* using an old page                    */

        iobptr = 0;
        start = lseek(ISIFD, 0L, 2);
        lseek(ISIFD, offset, 0);
        rstnl(keydesc, worknode);
        putndata((char *)&worknode->ndesc, NDDSIZE);
        for (i = 0; i < worknode->ndesc.keycnt; ++i) {
                tkp = worknode->kptr[i];
                if ((keydesc->k_flags & COMPRESS)) {
                        bcount = 0;
                        j = keydesc->k_len - UNIQUESIZE - 1;
                        while (j > -1 && tkp->keyval[j] == ' ') {
                                --j;
                                ++bcount;
                        }
                        compress = keydesc->k_len - (UNIQUESIZE + 
                                tkp->ldbytes + tkp->tlblanks + bcount);
                        if (compress < 0)
                                compress = 0;
                        ldsave = tkp->ldbytes;
                        tkp->ldbytes = (compress << 8);
                        tkp->ldbytes |= ldsave;
                }
                else {
                        tkp->ldbytes = tkp->tlblanks = ldsave = 0;
                        compress = keydesc->k_len - UNIQUESIZE;
                }
                putndata((char *)tkp, (unsigned)(NDKSIZE - sizeof(char *)));
                putndata((char *)&tkp->keyval[ldsave], (unsigned)compress);
                putndata((char *)&tkp->keyval[keydesc->k_len - UNIQUESIZE],
                        (unsigned)UNIQUESIZE);
        }
        freenode(worknode);
        if (offset >= start)
                write(ISIFD, iob, (unsigned)PAGESIZE);
        else
                write(ISIFD, iob, (unsigned)iobptr);
}

void putndata(char *buf, unsigned nchars)
{
        int     i = 0;

        while (nchars) {
                iob[iobptr++] = buf[i++];
                --nchars;
        }
}
