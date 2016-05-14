#include "iisam.h"

/*      Rewrite a record to a given isam file.                          */

/*      'rwrec' begins by fetching and saving the original data record  */
/*      in case of an error (EDUPL). The rewrite process begins by      */
/*      deleting all occurances of the saved 'datarec' in all index     */
/*      trees. Then the new record is actually written (Not rewritten)  */
/*      to all the index trees. If a duplicate key error (EDUPL) is     */
/*      generated, all the new records written to the trees (so far)    */
/*      are deleted and then the original information contained in      */
/*      'datarec' is restored to all the index trees.                   */

int	rwrec(int isfd, char *passkey, char *record)
{
        int     i, j;
        char    *reckey;                /* Pointer to various keyvalues */
        char    *datarec;               /* If EDUPL, rebuild old keys   */
                                        /* from this old data record    */
        struct  keydesc *keydesc;       /* Pointer to various key       */
                                        /* descriptions                 */
        US_LONG saveunum = UNIQUECOUNT; /* Uniqueness save area */
        struct  filetable       *file = &ftbl[isfd];    /* Efficiency   */

        if (!(file->omode & LOCKMASK)) {
                iserrno = EINPUTONLY;
                return(-1);
        }
        datarec = salloc((int)file->dptr.datalength);
        if (fetchdata(isfd, passkey, datarec)) {
                free(datarec);  /* Save old record in case a duplicates */
                return(-1);     /* error occurs during rewrite operation*/
        }                       /* Sunum set in fetchdata               */
        for (i = 0; i < file->dptr.dsccnt; ++i) {
                keydesc = &file->descptr[i];
                reckey = bkey(datarec, keydesc);
                getzapunum(reckey, keydesc, sunum, ZAP);
                modtree(isfd, reckey, keydesc, DELETE);
        }
        UNIQUECOUNT = sunum;
        for (i = PRIMARYKEY; i < file->dptr.dsccnt; ++i) {
                keydesc = &file->descptr[i];
                reckey = bkey(record, keydesc);
                getzapunum(reckey, keydesc, sunum, ZAP);
                if (modtree(isfd, reckey, keydesc, INSERT)) {
                        for (j = i - 1; j >= PRIMARYKEY; --j) {
                                keydesc = &file->descptr[j];
                                reckey = bkey(record, keydesc);
                                getzapunum(reckey, keydesc, sunum, ZAP);
                                modtree(isfd, reckey, keydesc, DELETE);
                        }
                        for (j = PRIMARYKEY; j < file->dptr.dsccnt; ++j) {
                                keydesc = &file->descptr[j];
                                reckey = bkey(datarec, keydesc);
                                getzapunum(reckey, keydesc, sunum, ZAP);
                                modtree(isfd, reckey, keydesc, INSERT);
                        }
                        UNIQUECOUNT = saveunum;
                        free(datarec);
                        return(-1);
                }
        }
        UNIQUECOUNT = saveunum;
        dumpdata(isfd, record, 0);
        free(datarec);
        return(0);
}
