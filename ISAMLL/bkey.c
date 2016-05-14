#include "iisam.h"

/*      Build a key from 'record' using the passed key description      */

/*      Note - bkey(), by default ZAPS reckey with an US_LONG 0 */

char    *bkey(char *record, struct keydesc *keydesc)
{
        int     i, j;
        char    *reckey;        /* Pointer to built key         */
        int     recidx;         /* Byte counter                 */
        struct  keypart *kp;    /* Efficiency                   */

        recidx = 0;
        reckey = salloc(keydesc->k_len);
        for (i = 0; i < keydesc->k_nparts; ++i) {
                kp = &keydesc->k_part[i];
                for (j=recidx; j<recidx+kp->kp_leng; ++j) {
                        if ((kp->kp_type & ISDESC) &&
                        ((kp->kp_type & TYPEMASK) != FLOATTYPE) &&
                        ((kp->kp_type & TYPEMASK) != DOUBLETYPE))
                                reckey[j] = HIGHVALUES -
                                        record[kp->kp_start + (j - recidx)];
                        else
                                reckey[j] =
                                        record[kp->kp_start + (j - recidx)];
                }
                recidx += kp->kp_leng;
        }
        getzapunum(reckey, keydesc, UNIQUESTART, ZAP);
        return(reckey);
}
