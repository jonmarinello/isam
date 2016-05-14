#include "iisam.h"

/*      Delete a key and corresponding data passkey from the isam file. */

int	delrec(int isfd, char *passkey)
{
        int     i;
        char    *reckey;                /* Pointer to work key          */
        char    *datarec;               /* Dynamic data record          */
        struct  keydesc *keydesc;       /* Pointer to key description   */
        struct  idict   *dict = &ftbl[isfd].dptr;       /* Efficiency   */

        if (!(ftbl[isfd].omode & LOCKMASK)) {
                iserrno = EINPUTONLY;
                return(-1);
        }
        datarec = salloc((int)dict->datalength);

        /* sunum is set in fetchdata() */

        if (fetchdata(isfd, passkey, datarec)) {
                free(datarec);
                return(-1);
        }
        for (i = 0; i < dict->dsccnt; ++i) {
                keydesc = &ftbl[isfd].descptr[i];
                reckey = bkey(datarec, keydesc);
                getzapunum(reckey, keydesc, sunum, ZAP);
                modtree(isfd, reckey, keydesc, DELETE);
        }
        free(datarec);
        return(0);
}
