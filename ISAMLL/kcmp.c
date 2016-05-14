#include "iisam.h"

/*      Compare two isam keys.  */

int	kcmp(struct keydesc *keydesc, char *curkey, char *newkey, int length)
{
        int     i, j;
        int     restart = 0;

        char    *cptr;
        char    *nptr;

        int     cint;
        short   cshort;
        long    clong;
        float   cfloat;
        double  cdouble;

        int     nint;
        short   nshort;
        long    nlong;
        float   nfloat;
        double  ndouble;

        US_LONG cunum;
        US_LONG nunum;

        if (kindex < 0)
                return(GREATER);
        for (i = 0; i < keydesc->k_nparts; ++i) {
                if (restart >= length)
                        return(EQUAL);
                switch (keydesc->k_part[i].kp_type & TYPEMASK) {
                case CHARTYPE :
                        for (j = restart; j < restart + keydesc->k_part[i].kp_leng; ++j) {
                                if (j == length)
                                        return(EQUAL);
                                if ((unsigned)newkey[j] > (unsigned)curkey[j])
                                        return(GREATER);
                                else if ((unsigned)newkey[j] < (unsigned)curkey[j])
                                        return(LESS);
                        }
                        break;
                case SHORTTYPE :
                        cptr = (char *)&cshort;
                        nptr = (char *)&nshort;
                        for (j = restart; j < restart + SHORTSIZE; ++j) {
                                *cptr++ = curkey[j];
                                *nptr++ = newkey[j];
                        }
                        if (nshort > cshort)
                                return(GREATER);
                        if (nshort < cshort)
                                return(LESS);
                        break;
                case INTTYPE :
                        cptr = (char *)&cint;
                        nptr = (char *)&nint;
                        for (j = restart; j < restart + INTSIZE; ++j) {
                                *cptr++ = curkey[j];
                                *nptr++ = newkey[j];
                        }
                        if (nint > cint)
                                return(GREATER);
                        if (nint < cint)
                                return(LESS);
                        break;
                case LONGTYPE :
                        cptr = (char *)&clong;
                        nptr = (char *)&nlong;
                        for (j = restart; j < restart + LONGSIZE; ++j) {
                                *cptr++ = curkey[j];
                                *nptr++ = newkey[j];
                        }
                        if (nlong > clong)
                                return(GREATER);
                        if (nlong < clong)
                                return(LESS);
                        break;
                case FLOATTYPE :
                        cptr = (char *)&cfloat;
                        nptr = (char *)&nfloat;
                        for (j = restart; j < restart + FLOATSIZE; ++j) {
                                *cptr++ = curkey[j];
                                *nptr++ = newkey[j];
                        }
                        /* Reverse comparison for ISDESC */
                        if ((keydesc->k_part[i].kp_type & ISDESC)) {
                                if (nfloat < cfloat)
                                        return(GREATER);
                                if (nfloat > cfloat)
                                        return(LESS);
                        }
                        else {
                                if (nfloat > cfloat)
                                        return(GREATER);
                                if (nfloat < cfloat)
                                        return(LESS);
                        }
                        break;
                case DOUBLETYPE :
                        cptr = (char *)&cdouble;
                        nptr = (char *)&ndouble;
                        /* Reverse comparison for ISDESC */
                        for (j = restart; j < restart + DOUBLESIZE; ++j) {
                                *cptr++ = curkey[j];
                                *nptr++ = newkey[j];
                        }
                        if ((keydesc->k_part[i].kp_type & ISDESC)) {
                                if (ndouble < cdouble)
                                        return(GREATER);
                                if (ndouble > cdouble)
                                        return(LESS);
                        }
                        else {
                                if (ndouble > cdouble)
                                        return(GREATER);
                                if (ndouble < cdouble)
                                        return(LESS);
                        }
                        break;
                }
                restart += keydesc->k_part[i].kp_leng;
        }
        if (length == keydesc->k_len) {
                cunum = getzapunum(curkey, keydesc, DUMMYGET, GET);
                nunum = getzapunum(newkey, keydesc, DUMMYGET, GET);
                if (nunum > cunum)
                        return(GREATER);
                if (nunum < cunum)
                        return(LESS);
        }
        return(EQUAL);
}
