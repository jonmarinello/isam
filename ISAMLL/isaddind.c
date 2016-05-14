#include "iisam.h"

/*      Add a new index tree to a given file    */

int	isaddindex(int isfd, struct keydesc *keydesc)
{
        int     oblocks;                /* Previously required blocks   */
        int     nblocks;                /* Newly required blocks        */
        struct  nodedesc rootnode;      /* Rootnode of new index        */
        char    *datarec;               /* Pointer to data record buffer*/
        char    *reckey;                /* Pointer to work keyval       */
        int     savecount;              /* Uniqueness save area         */
        int     savelive;               /* Livekey save area            */
        char    savekey[INTERNALKEYSZ]; /* 'Current record pointer'     */
                                        /* area                         */
        struct  filetable       *file = &ftbl[isfd];    /* Efficiency   */
        long    *nplace = &file->dptr.dsccnt;   /* Pointer to number of */
                                                /* key descriptions     */
        int     i;

        iserrno = 0;
        if (chkclose(isfd) < 0)
                return(-1);
        if (!(file->omode & ISEXCLLOCK)) {
                iserrno = ENOTEXCL;
                return(-1);
        }
        if (!(file->omode & LOCKMASK)) {
                iserrno = EINPUTONLY;
                return(-1);
        }
        if (chkkdesc((int)file->dptr.datalength, keydesc))
                return(-1);
        for (i = 0; i < file->dptr.dsccnt; ++i)
                if (!kdsccmp((char *)keydesc, (char *)&file->descptr[i])) {
                        iserrno = EKEXISTS;
                        return(-1);
                }
        oblocks = keyblks(isfd) - 1;
        datarec = salloc((int)file->dptr.datalength);
        if ((file->descptr = (struct keydesc *)realloc((char *)file->descptr,
                (unsigned)(KDSIZE * ++(*nplace)))) == NULL) {
                        fprintf(stderr, memerror);
                        abort();
        }
        rootnode.nodeln = NDDSIZE;
        rootnode.keycnt = 0;
        rootnode.leftptr = NILL;
        lseek(ISIFD, keydesc->k_rootnode = getspace(isfd, INDEXFREELIST), 0);
        write(ISIFD, (char *)&rootnode, NDDSIZE);
        fillblk(ISIFD);
	memcpy((char *)&file->descptr[*nplace - 1],
                (char *)keydesc, KDSIZE);
        nblocks = keyblks(isfd) - 1;
        if (oblocks != nblocks) {
                if ((file->kbptr =
                        (struct keyblock *)realloc((char *)file->kbptr,
                        (unsigned)(KBSIZE * (nblocks + 1)))) == NULL) {
                                fprintf(stderr, memerror);
                                abort();
                }
                file->kbptr[oblocks].kchain = getspace(isfd, INDEXFREELIST);
                file->kbptr[nblocks].kchain = NILL;
        }
        dkeydesc(isfd);
        ddiction(isfd);

        /*      Build new B-Tree index structure        */

        savecount = UNIQUECOUNT;
        savelive = file->livekey;
	memcpy(savekey, file->curkey, file->descptr[savelive].k_len);
        isstart(isfd, &file->descptr[PRIMARYKEY], 0, datarec, ISFIRST);
        for (;;) {
                if (isread(isfd, datarec, ISNEXT)) {
                        iserrno = 0;
                        break;
                }
                reckey = bkey(datarec, keydesc);
                UNIQUECOUNT = getzapunum(file->curkey,
                        &file->descptr[PRIMARYKEY], DUMMYGET, GET);
                getzapunum(reckey, keydesc, HIGHUSTART, ZAP);
                if (modtree(isfd, reckey, keydesc, INSERT))
                        break;
        }
        UNIQUECOUNT = savecount;
        free((char *)datarec);
        if (!iserrno) {
                ddiction(isfd);
                isstart(isfd, keydesc, 0, datarec, ISFIRST);
                iserrno = 0;
                autoulock(isfd);
                return(0);
        }
        ddiction(isfd);
        isdelindex(isfd, keydesc);
        iserrno = EDUPL;
        file->livekey = savelive;
	memcpy(file->curkey, savekey, file->descptr[savelive].k_len);
        return(-1);
}
