#include "iisam.h"

/* NOTE - Use of US_LONG allows a maximum of 4,294,967,296 */

#define IBMAT	1

US_LONG getzapunum(char *keystring, struct keydesc *keydesc,
		US_LONG unum, int mode)
{
        int     i;
        char    *un;            /* Character pointer to an US_LONG*/
        US_LONG retunum;/* If GET return thru retunum           */
        US_LONG start;  /* Pointer into keystring where the GET */
                                /* or ZAP will begin                    */
/* Thu Mar 27 10:42:56 EST 1986:baba */
#ifdef  IBMAT
        US_LONG tmp;
#endif

        start = keydesc->k_len - UNIQUESIZE;
        switch (mode) {
                case ZAP :
                        un = (char *)&unum;
                        for (i = 0; i < UNIQUESIZE; ++i) {
#ifdef IBMAT
                                tmp = start + i;
                                keystring[tmp] = *un++;
#else
                                keystring[start + i] = *un++;
#endif
                        }
			break;
		case GET :
			un = (char *)&retunum;
			for (i = 0; i < UNIQUESIZE; ++i)
				*un++ = keystring[start + i];
			return retunum;
	}
	return 0;	/* Dummy return Value */
}
