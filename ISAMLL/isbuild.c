#include "iisam.h"

/*      Build an isam file. Create the '.idx' and '.dat' files.         */

int	isbuild(char *filename, int recordlength, struct keydesc *keydesc,
		int mode)
{
        int     isfd;                   /* Pointer into filetable       */
        struct  nodedesc        rootnode;       /* Primary rootnode     */
        struct  keyblock        keyblock;       /* Primary keyblock     */
        struct  idict           *diction;       /* Efficiency           */

        iserrno = 0;
        if (chkkdesc(recordlength, keydesc))
                return(-1);
        if ((isfd = iopen(filename, ISCREATE+mode)) < 0)
                return(-1);
        diction = &ftbl[isfd].dptr;
        diction->dsccnt = 1L;
        diction->dfree = diction->kfree = NILL;
        diction->unumber = diction->reccount = diction->unique = 0L;
        diction->datalength = (long)recordlength;
        diction->sn = isserial;
        strcpy(diction->vn, isversnumber);
        strcpy(diction->cr, iscopyright);
        strcpy(diction->fu, "123456789012345678901234"); /* Future Use buffer */
        write(ISIFD, (char *)diction, IDSIZE);
        keydesc->k_rootnode = PAGESIZE;
        keyblock.kchain = NILL;
        keyblock.bcount = IDSIZE + KBSIZE + keysize(keydesc);
        write(ISIFD, (char *)&keyblock, KBSIZE);
        dkey(isfd, keydesc);
        fillblk(ISIFD);
        rootnode.nodeln = NDDSIZE;
        rootnode.keycnt = 0;
        rootnode.leftptr = NILL;
        write(ISIFD, (char *)&rootnode, NDDSIZE);
        fillblk(ISIFD);
        lkeydesc(isfd);
        return(isfd);
}
