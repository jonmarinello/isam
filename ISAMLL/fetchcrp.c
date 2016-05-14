#include "iisam.h"

/*      Fetch and copy the 'Current file pointer' key into 'record'     */

/*      Note - Calling this function ensures that the user really       */
/*              calls rwrec() with the 'Current record pointer'         */

void	fetchcrp(int isfd, char *record)
{
        int     i, j;
        int     recidx = 0;     /* Byte counter                 */
        char    *reckey = ftbl[isfd].curkey;
        struct  keydesc *keydesc = &ftbl[isfd].descptr[ftbl[isfd].livekey];
        struct  keypart *kp;
        int     dumbidx;        /* Some machines don't allow    */
                                /* computations in the index    */

        for (i = 0; i < keydesc->k_nparts; ++i) {
                kp = &keydesc->k_part[i];
                for (j = recidx; j < recidx+kp->kp_leng; ++j) {
                        dumbidx = kp->kp_start + (j - recidx);
                        if ((kp->kp_type & ISDESC) &&
                        ((kp->kp_type & TYPEMASK) != FLOATTYPE) &&
                        ((kp->kp_type & TYPEMASK) != DOUBLETYPE)) {
                                record[dumbidx] = HIGHVALUES - reckey[j];
                        }
                        else {
                                record[dumbidx] = reckey[j];
                        }
                }
                recidx += kp->kp_leng;
        }
}
