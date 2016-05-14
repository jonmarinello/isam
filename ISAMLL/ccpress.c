#include "iisam.h"

/*      Compute compression of keys within a B-tree node.               */

int	ccpress(struct keydesc *keydesc, struct NODE *worknode)
{
        int     i;                      /* Which key to compress        */
        int     j;                      /* Which character to check     */
        int     bsaved = 0;             /* Bytes saved by compression   */
        int     actlen = keydesc->k_len - UNIQUESIZE; /* Actual size    */
        int     poffset;
        int     toffset;
        struct  nodekey *pkp;           /* Previous key pointer         */
        struct  nodekey *tkp;           /* This (current) key pointer   */
        char    *pk;                    /* Previous keyval              */
        char    *tk;                    /* This (current) keyval)       */
        int     soffset;                /* Save offset                  */

        if (!(keydesc->k_flags & COMPRESS) || !(worknode->ndesc.keycnt))
                return(0);
        for (i = 0; i < worknode->ndesc.keycnt; ++i) {
                tkp = worknode->kptr[i];
                tkp->ldbytes = tkp->tlblanks = 0;
        }
        /* Remove blanks in first key */
        j = actlen - 1;
        pkp = worknode->kptr[0];
        pkp->ldbytes = 0;
        pk = pkp->keyval;
        while (j > -1 && pk[j] == ' ') {
                ++bsaved;
                --j;
        }
        poffset = j;
        for (i = 1; i < worknode->ndesc.keycnt; ++i) {
                /* Leading compression first    */
                tkp = worknode->kptr[i];
                tk = tkp->keyval;
                for (j = 0; j < actlen && pk[j] == tk[j]; ++j)
                        ;
                tkp->ldbytes = j;
                bsaved += j;
                if (j == actlen)
                        continue;       /* Duplicate key found          */
                /* Trailing compression next */
                tkp->tlblanks = 0;
                j = actlen - 1;
                while (j > -1 && tk[j] == ' ') {
                        ++bsaved;
                        --j;
                }
                soffset = toffset = j;
                while (poffset > -1 && toffset > -1 && pk[poffset] == tk[toffset]) {
                        /* Check for leading/trailing overlap */
                        if (tkp->ldbytes > toffset)
                                goto SKIPTCPRESS;
                        if (pkp->ldbytes > poffset)
                                goto SKIPTCPRESS;
                        ++tkp->tlblanks;
                        --poffset;
                        --toffset;
                }
SKIPTCPRESS:    bsaved += tkp->tlblanks;
                poffset = soffset;
                pk = tk;
                pkp = tkp;
        }
        return(bsaved);
}
