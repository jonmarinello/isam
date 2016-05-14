#include "iisam.h"

/*      Check the passed key description for possible errors.           */

int	chkkdesc(int recordlength, struct keydesc *keydesc)
{
        int     i;

        if (keydesc->k_nparts > NPARTS || keydesc->k_nparts < 1)
                goto BADDESC;
        keydesc->k_len = 0;
        for (i = 0; i < keydesc->k_nparts; ++i) {
                keydesc->k_len += keydesc->k_part[i].kp_leng;
                if (keydesc->k_len > MAXKEYLENGTH ||
                        (keydesc->k_part[i].kp_start +
                        keydesc->k_part[i].kp_leng) > recordlength)
                                goto BADDESC;
                switch (keydesc->k_part[i].kp_type & TYPEMASK) {
                        case CHARTYPE :
                        case INTTYPE :
                        case LONGTYPE :
                        case DOUBLETYPE :
                        case FLOATTYPE :
                        case SHORTTYPE :
                                break;
                        default :
                                goto BADDESC;
                }
        }
        switch (keydesc->k_flags & COMPMASK) {
                case ISNODUPS :
                        break;
                                        /* NOTE - ALL RDS COMPRESSION
                                                TECHNIQUES ARE CHANGED
                                                TO FULL COMPRESSION     */
                case DCOMPRESS :
                case LCOMPRESS :
                case TCOMPRESS :
                case COMPRESS :
                        keydesc->k_flags = COMPRESS + (keydesc->k_flags & DUPSMASK);
                        break;
                default :
                        goto BADDESC;
        }
        keydesc->k_len += UNIQUESIZE;
        return(0);
BADDESC:
        iserrno = EBADKEY;
        return(-1);
}
